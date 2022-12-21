#include "calendar.h"
#include "event.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Initializes a Calendar struct*/
int init_calendar(const char *name, int days,
                  int (*comp_func)(const void *ptr1, const void *ptr2),
                  void (*free_info_func)(void *ptr), Calendar **calendar) {

    if ((*calendar) && name && days > 0) {

        (*calendar) = malloc(sizeof(Calendar));
        if (*calendar) {

            /*returns failure if mem allocation fails*/
            (*calendar)->name = malloc(strlen(name) + 1);
            if ((*calendar)->name) {
                strcpy((*calendar)->name, name);
            } else {
                return FAILURE;
            }

            /*returns failure if mem allocation fails*/
            (*calendar)->events = calloc(days, sizeof(Event));
            if ((*calendar)->events == NULL) {
                return FAILURE;
            }

            (*calendar)->days = days;
            (*calendar)->total_events = 0;
            (*calendar)->comp_func = comp_func;
            (*calendar)->free_info_func = free_info_func;

            return SUCCESS;
        }
    }
    return FAILURE;
}

/* Prints contents of the calendar, including the name, days, and total events
of paramter print_all is true */
int print_calendar(Calendar *calendar, FILE *output_stream, int print_all) {
    int i;
    Event *event;

    if (calendar && output_stream) {

        if (print_all) {
            fprintf(output_stream, "Calendar's Name: \"%s\"\n", calendar->name);
            fprintf(output_stream, "Days: %d\n", calendar->days);
            fprintf(output_stream, "Total Events: %d\n\n",
                    calendar->total_events);
        }

        fprintf(output_stream, "**** Events ****\n");

        if (calendar->total_events > 0) {
            for (i = 0; i < calendar->days; i++) {
                printf("Day %d\n", i + 1);
                event = calendar->events[i];

                while (event) {
                    fprintf(output_stream, "Event's Name: \"%s\", ",
                            event->name);
                    fprintf(output_stream, "Start_time: %d, ",
                            event->start_time);
                    fprintf(output_stream, "Duration: %d\n",
                            event->duration_minutes);
                    event = event->next;
                }
            }
        }
        return SUCCESS;
    }
    return FAILURE;
}

/*Adds a new event into a calendar on a specific day granetd that the 
event does not already exist in the calendar*/
int add_event(Calendar *calendar, const char *name, int start_time,
              int duration_minutes, void *info, int day) {
    Event *new_event, *prev, *ptr, *temp;

    if (calendar && name && start_time >= 0 && start_time <= 2400 &&
        duration_minutes > 0 && day > 0 && day <= calendar->days) {
        /* if event does not exist in the calendar */
        if (find_event(calendar, name, &temp) == FAILURE) {
            new_event = malloc(sizeof(Event));
            if (new_event) {
                new_event->name = malloc(strlen(name) + 1);
                if (new_event->name) {
                    strcpy(new_event->name, name);
                    new_event->duration_minutes = duration_minutes;
                    new_event->start_time = start_time;
                    new_event->info = info;

                    ptr = calendar->events[day - 1];

                    if (ptr) {
                        /* first event > new event (adds to front) */
                        if (calendar->comp_func(ptr, new_event) > 0) {
                            calendar->events[day - 1] = new_event;
                            new_event->next = ptr;
                        } else {
                            prev = ptr;
                            ptr = ptr->next;
                            /*goes through list until new event can be added*/
                            while (ptr &&
                                   calendar->comp_func(ptr, new_event) < 0) {
                                prev = ptr;
                                ptr = ptr->next;
                            }
                            new_event->next = ptr;
                            prev->next = new_event;
                        }
                        /*no events exist for that day*/
                    } else {
                        calendar->events[day - 1] = new_event;
                        new_event->next = NULL;
                    }
                    (calendar->total_events)++;
                    return SUCCESS;
                }
            }
        }
    }
    return FAILURE;
}

/* Finds the given event in the calendar and returns a pointer to it 
if the event exists*/
int find_event(Calendar *calendar, const char *name, Event **event) {
    int i;
    Event *ptr;

    if (calendar && name && event) {
        for (i = 0; i < calendar->days; i++) {
            ptr = calendar->events[i];

            while (ptr && strcmp(ptr->name, name) != 0) {
                ptr = ptr->next;
            }

            if (ptr) {
                (*event) = ptr;
                return SUCCESS;
            }
        }
    }
    return FAILURE;
}

/* Finds the given event in the specific day and returns a pointer to it if the 
events exists */
int find_event_in_day(Calendar *calendar, const char *name, int day,
                      Event **event) {
    int i;
    Event *ptr;

    if (calendar && name && day > 0 && day <= calendar->days && event) {
        for (i = 0; i < calendar->days; i++) {
            if ((i + 1) == day) {
                ptr = calendar->events[i];

                while (ptr && strcmp(ptr->name, name) != 0) {
                    ptr = ptr->next;
                }

                if (ptr) {
                    (*event) = ptr;
                    return SUCCESS;
                }
            }
        }
    }
    return FAILURE;
}

/* Removes specified event from the calendar (if it exists) and returns any
allocated memory */
int remove_event(Calendar *calendar, const char *name) {
    Event *temp, *ptr, *prev;
    int i;

    if(calendar && name){
        if (find_event(calendar, name, &temp) == SUCCESS){
            for(i = 0; i < calendar->days; i++){
                ptr = calendar->events[i];

                if(ptr){
                    /* if event being removed is the head*/
                    if(strcmp(ptr->name, name) == 0){
                        /*only event in the list*/
                        if(ptr->next == NULL){
                            clear_day(calendar, i+1);
                            return SUCCESS;
                        } else {
                            calendar->events[i] = ptr->next;
                            ptr->next = NULL;
                        }
                    /*remove event from middle of the list*/
                    } else {
                        while(ptr && strcmp(ptr->name, name) != 0){
                            prev = ptr;
                            ptr = ptr->next;
                        }
                        prev->next = ptr->next;
                        ptr->next = NULL;
                    }

                    free(ptr->name);
                    ptr->name = NULL;
                    if(calendar->free_info_func){
                        calendar->free_info_func(ptr->info);
                    }
                    free(ptr);
                    ptr = NULL;

                    (calendar->total_events)--;
                    return SUCCESS;
                } 
            }
        }
    }
    return FAILURE;
}

/* Returns info of the event if it exists in the calendar */
void *get_event_info(Calendar *calendar, const char *name) {
    Event *event;

    if (find_event(calendar, name, &event) == SUCCESS) {
        return event->info;
    } else {
        return NULL;
    }
}

/* Removes all events in the calendar and returns all allocated memory */
int clear_calendar(Calendar *calendar) {
    int i;

    if (calendar) {
        for (i = 0; i < calendar->days; i++) {
            clear_day(calendar, (i + 1));
        }
        calendar->total_events = 0;

        return SUCCESS;
    }
    return FAILURE;
}

/* Removes all events in the calendar for a given day */
int clear_day(Calendar *calendar, int day) {
    Event *ptr, *temp;

    if (calendar && day > 0 && day <= calendar->days &&
        calendar->total_events > 0) {
            
        ptr = calendar->events[day - 1];
        while (ptr) {
            temp = ptr;
            ptr = ptr->next;

            free(temp->name);
            temp->name = NULL;

            if (calendar->free_info_func && temp->info) {
                calendar->free_info_func(temp->info);
            }

            free(temp);
            temp = NULL;

            (calendar->total_events)--;
        }
        calendar->events[day - 1] = NULL;
        return SUCCESS;
    }
    return FAILURE;
}

/* Frees all memory allocated for the calendar */
int destroy_calendar(Calendar *calendar) {
    if (calendar) {

        free(calendar->name);
        calendar->name = NULL;

        clear_calendar(calendar);
        free(calendar->events);
        calendar->events = NULL;

        free(calendar);
        calendar = NULL;

        return SUCCESS;
    }
    return FAILURE;
}