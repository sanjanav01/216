#include "calendar.h"
#include "event.h"
#include "my_memory_checker_216.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
Sanjana Vellanki
UID: 117990881
Directory ID: svellank
Last Updated: November 7, 2022
*/

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/
typedef struct task_info {
  double cost;
  char *prog_language;
} Task_info;

static int comp_minutes(const void *ptr1, const void *ptr2) {
    return ((Event *)ptr1)->duration_minutes -
           ((Event *)ptr2)->duration_minutes;
}

static Task_info *create_task_info(double cost, const char *prog_language) {
   Task_info *task_info = malloc(sizeof(Task_info));

   if (task_info) {
      task_info->prog_language = malloc(strlen(prog_language) + 1);
      if (task_info->prog_language) {
         task_info->cost = cost;
         strcpy(task_info->prog_language, prog_language);
         return task_info;
      }
   }

   return NULL;
}

static void free_task_info(void *ptr) {
   Task_info *task_info = (Task_info *)ptr;

   free(task_info->prog_language);
   free(task_info);
}

/* test init_calendar && print_calendar */
static int test1() {
    int days = 7;
    Calendar *calendar;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
        if (print_calendar(calendar, stdout, 1) == SUCCESS) {
            return destroy_calendar(calendar);
        }
    }
    return FAILURE;
}

/* test init_calendar for FAILURE cases*/
static int test2() {
    int days = 7, count = 0, stat = SUCCESS;
    Calendar *calendar;

   /* name is null*/
    if (init_calendar(NULL, days, comp_minutes, NULL, &calendar) == FAILURE) {
        count++;
    } else {
        stat = FAILURE;
        printf("Test 2: Failed for name == NULL\n");
    }
    /* calendar is null */
    if (init_calendar("name", days, comp_minutes, NULL, NULL) == FAILURE) {
        count++;
    } else {
        stat = FAILURE;
        printf("Test 2: Failed for calendar == NULL\n");
    }
    /* days is less than 0*/
    if (init_calendar("name", -1, comp_minutes, NULL, &calendar) == FAILURE) {
        count++;
    } else {
        stat = FAILURE;
        printf("Test 2: Failed for days < 0\n");
    }

    if (count != 3) {
        stat = FAILURE;
    }

    return stat;
}

/* test print_calendar for FAILURE cases*/
static int test3() {
    int days = 7, count = 0, stat = SUCCESS;
    Calendar *calendar;
    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      /* calendar is null*/
        if (print_calendar(NULL, stdout, 1) == FAILURE) {
            count++;
        } else {
            stat = FAILURE;
            printf("Test 3: Failed for calendar == NULL\n");
        }
        /* output stream is null*/
        if (print_calendar(calendar, NULL, 1) == FAILURE) {
            count++;
        } else {
            stat = FAILURE;
            printf("Test 3: Failed for output_stream == NULL\n");
        }

        if (count != 2) {
            stat = FAILURE;
        }
        if (destroy_calendar(calendar) == FAILURE) {
            stat = FAILURE;
        }

    } else {
        stat = FAILURE;
    }

    return stat;
}

/*Adding same event to different days*/
static int test4() {
    Calendar *calendar;
    int days = 7, stat = SUCCESS;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {

        if (add_event(calendar, "meeting", 10, 20, NULL, 1) != SUCCESS) {
            stat = FAILURE;
            printf("Add event was not success\n");
            if (add_event(calendar, "meeting", 10, 20, NULL, 2) != FAILURE) {
                stat = FAILURE;
                printf("Add duplicate event was not success\n");
            }
        }
        if (destroy_calendar(calendar) == FAILURE) {
            stat = FAILURE;
        }
    } else {
        stat = FAILURE;
    }
    return stat;
}

/*Adding same event to same day*/
static int test5() {
    Calendar *calendar;
    int days = 7, stat = SUCCESS;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {

        if (add_event(calendar, "meeting", 10, 20, NULL, 1) != SUCCESS) {
            stat = FAILURE;
            printf("Add event was not success\n");
            if (add_event(calendar, "meeting", 10, 20, NULL, 1) != FAILURE) {
                stat = FAILURE;
                printf("Add duplicate event was not success\n");
            }
        }
        if (destroy_calendar(calendar) == FAILURE) {
            stat = FAILURE;
        }
    } else {
        stat = FAILURE;
    }
    return stat;
}

/*Find an existing and non-exsiting event in a given day*/
static int test6() {
    Calendar *calendar;
    Event *event;
    int days = 7, stat = SUCCESS;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {

        if (add_event(calendar, "meeting", 10, 20, NULL, 1) != SUCCESS) {
            stat = FAILURE;
            printf("Add event was not success\n");
            
            if (find_event(calendar, "meeting", &event) != SUCCESS) {
                stat = FAILURE;
                printf("Find event was not success\n");
            }
            /* find event that doesn't exist*/
            if (find_event(calendar, "random", &event) != FAILURE) {
                stat = FAILURE;
                printf("Find event for non-existing event was not success\n");
            }
        }
        if (destroy_calendar(calendar) == FAILURE) {
            stat = FAILURE;
        }
    } else {
        stat = FAILURE;
    }
    return stat;
}

/* Find event in day test cases for existing & non existing events */
static int test7() {
    Calendar *calendar;
    Event *event;
    int days = 7, stat = SUCCESS;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {

        if (add_event(calendar, "meeting1", 10, 20, NULL, 1) != SUCCESS ||
            add_event(calendar, "meeting2", 10, 20, NULL, 2) != SUCCESS) {
            stat = FAILURE;
            printf("Add event was not success\n");

            if (find_event_in_day(calendar, "meeting1", 1, &event) != SUCCESS) {
                stat = FAILURE;
                printf("Find event in day 1 was not success\n");
            }
            if (find_event_in_day(calendar, "meeting2", 2, &event) != SUCCESS) {
                stat = FAILURE;
                printf("Find event in day 2 was not success\n");
            }
            /* find events in day that do not exist*/
            if (find_event_in_day(calendar, "meeting2", 1, &event) != FAILURE) {
                stat = FAILURE;
                printf("Find non-existing event in day 1 was not success\n");
            }
            if (find_event_in_day(calendar, "meeting1", 2, &event) != FAILURE) {
                stat = FAILURE;
                printf("Find non-existing event in day 2 was not success\n");
            }
        }
        if (destroy_calendar(calendar) == FAILURE) {
            stat = FAILURE;
        }
    } else {
        stat = FAILURE;
    }
    return stat;
}

/* Removing events from the front and back of the list*/
static int test8() {
    Calendar *calendar;
    int days = 7, stat = SUCCESS;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
        add_event(calendar, "meeting1", 10, 20, NULL, 1);
        add_event(calendar, "meeting2", 10, 20, NULL, 1);
        add_event(calendar, "meeting3", 10, 20, NULL, 1);

        if (remove_event(calendar, "meeting1") != SUCCESS) {
            stat = FAILURE;
            printf("Remove event from front failed\n");
        }
        if (remove_event(calendar, "meeting3") != SUCCESS) {
            stat = FAILURE;
            printf("Remove event from end failed\n");
        }
        if (destroy_calendar(calendar) == FAILURE) {
            stat = FAILURE;
        }
    }
    return stat;
}

/* Removing an event from the middle of a list */
static int test9() {
    Calendar *calendar;
    int days = 7, stat = SUCCESS;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
        add_event(calendar, "meeting1", 10, 20, NULL, 1);
        add_event(calendar, "meeting2", 10, 20, NULL, 1);
        add_event(calendar, "meeting3", 10, 20, NULL, 1);

        if (remove_event(calendar, "meeting2") != SUCCESS) {
            stat = FAILURE;
            printf("Remove event from middle failedn\n");
        }
        if (destroy_calendar(calendar) == FAILURE) {
            stat = FAILURE;
        }
    }
    return stat;
}

/* Removing and Finding events from an empty calendar */
static int test10() {
    Calendar *calendar;
    Event *event;
    int days = 7, stat = SUCCESS;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {

        if (find_event(calendar, "meeting1", &event) != FAILURE) {
            stat = FAILURE;
            printf("Find event failed\n");
        }
        if (remove_event(calendar, "meeting1") != FAILURE) {
            stat = FAILURE;
            printf("Remove event failed\n");
        }
        if (find_event_in_day(calendar, "meeting1", 1, &event) != FAILURE) {
            stat = FAILURE;
            printf("Remove event failed\n");
        }

        if (destroy_calendar(calendar) == FAILURE) {
            stat = FAILURE;
        }
    }
    return stat;
}

/* clearing a non-empty calendar */
static int test11() {
    Calendar *calendar;
    int days = 7, stat = SUCCESS;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {

        add_event(calendar, "meeting1", 10, 20, NULL, 1);
        add_event(calendar, "meeting2", 10, 20, NULL, 2);
        add_event(calendar, "meeting3", 10, 20, NULL, 7);

        if (clear_calendar(calendar) != SUCCESS) {
            stat = FAILURE;
            printf("Clear calendar failed\n");
        }
        if (destroy_calendar(calendar) == FAILURE) {
            stat = FAILURE;
        }
    }
    return stat;
}

/* clear a non-empty calendar's days */
static int test12() {
    Calendar *calendar;
    int days = 7, stat = SUCCESS;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {

        add_event(calendar, "meeting1", 10, 20, NULL, 1);
        add_event(calendar, "meeting2", 10, 20, NULL, 1);
        add_event(calendar, "meeting3", 10, 20, NULL, 2);

        if (clear_day(calendar, 1) != SUCCESS) {
            stat = FAILURE;
            printf("Clear calendar day 1 failed\n");
        }
        if (clear_day(calendar, 2) != SUCCESS) {
            stat = FAILURE;
            printf("Clear calendar day 2 failed\n");
        }
        if (destroy_calendar(calendar) == FAILURE) {
            stat = FAILURE;
        }
    }
    return stat;
}

/* clearing calendar and calendar days when calendar is empty */
static int test13() {
    Calendar *calendar;
    int days = 7, stat = SUCCESS;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {

        if (clear_calendar(calendar) != SUCCESS) {
            stat = FAILURE;
            printf("Clear calendar when calendar is empty failed ");
        }
        if (clear_day(calendar, 1) && clear_day(calendar, 7) != FAILURE) {
            stat = FAILURE;
            printf("Clear calendar days when calendar is empty failed ");
        }

        if (destroy_calendar(calendar) == FAILURE) {
            stat = FAILURE;
        }
    }
    return stat;
}

/* clears calendar days and adding events immediately after*/
static int test14() {
    Calendar *calendar;
    int days = 7, stat = SUCCESS;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {

        add_event(calendar, "meeting1", 10, 20, NULL, 1);
        add_event(calendar, "meeting2", 10, 20, NULL, 1);
        add_event(calendar, "meeting3", 10, 20, NULL, 2);

        if (clear_day(calendar, 1) != SUCCESS) {
            stat = FAILURE;
            printf("Clear calendar day 1 failed\n");
        }
        if (clear_day(calendar, 2) != SUCCESS) {
            stat = FAILURE;
            printf("Clear calendar day 2 failed\n");
        }

        if (add_event(calendar, "meeting1", 10, 20, NULL, 1) != SUCCESS) {
            stat = FAILURE;
            printf("Add event after day 1 cleared failed\n");
        }

        if (add_event(calendar, "meeting2", 10, 20, NULL, 2) != SUCCESS) {
            stat = FAILURE;
            printf("Add event after day 7 cleared failed\n");
        }

        if (destroy_calendar(calendar) == FAILURE) {
            stat = FAILURE;
        }
    }
    return stat;
}

/* clears calendar and adds events immediately after */
static int test15() {
    Calendar *calendar;
    int days = 7, stat = SUCCESS;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {

        add_event(calendar, "meeting1", 10, 20, NULL, 1);
        add_event(calendar, "meeting2", 10, 20, NULL, 1);
        add_event(calendar, "meeting3", 10, 20, NULL, 2);

        if (clear_calendar(calendar) != SUCCESS) {
            stat = FAILURE;
            printf("Clear calendar failed\n");
        }
        if (add_event(calendar, "meeting1", 10, 20, NULL, 1) != SUCCESS) {
            stat = FAILURE;
            printf("Add event after day 1 cleared failed\n");
        }

        if (add_event(calendar, "meeting2", 10, 20, NULL, 2) != SUCCESS) {
            stat = FAILURE;
            printf("Add event after day 7 cleared failed\n");
        }

        if (destroy_calendar(calendar) == FAILURE) {
            stat = FAILURE;
        }
    }
    return stat;
}

/* Adding events immediately after a calendar is destroyed */
static int test16() {
    Calendar *calendar;
    int days = 7, stat = SUCCESS;

    if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {

        add_event(calendar, "meeting1", 10, 20, NULL, 1);
        add_event(calendar, "meeting2", 10, 20, NULL, 1);
        add_event(calendar, "meeting3", 10, 20, NULL, 2);

        if (destroy_calendar(calendar) == FAILURE) {
            stat = FAILURE;
        }

        if (add_event(calendar, "meeting2", 10, 20, NULL, 7) != FAILURE) {
            stat = FAILURE;
            printf("Add event after day 7 cleared failed\n");
        }
    }
    return stat;
}

/* Testing get_info when info parameter exists and is NULL*/
static int test17() {
   int days = 7, start_time_mil = 10, duration_minutes = 20, stat = FAILURE;
   Calendar *calendar;
   Task_info *info;

   init_calendar("Winter Calendar", days, comp_minutes, free_task_info, &calendar);

   info = create_task_info(35000, "Java");
   add_event(calendar, "debugging", start_time_mil, duration_minutes, 
             info, 1);

   info = create_task_info(10000, "JUnit");
   add_event(calendar, "testing", 1600, 100, info, 2);

   add_event(calendar, "random", 1600, 100, NULL, 2);

   info = get_event_info(calendar, "debugging"); 
   if(info->cost || info->prog_language){
      stat = SUCCESS;
   }
   info = get_event_info(calendar, "testing");
      if(info->cost || info->prog_language){
      stat = SUCCESS;
   }

   /* info param is NULL*/
   info = get_event_info(calendar, "random");
   if(info == NULL){
      stat = SUCCESS;
   }

   destroy_calendar(calendar);

   return stat;
}

int main() {
    int result = SUCCESS;

    /***** Starting memory checking *****/
    start_memory_check();
    /***** Starting memory checking *****/

    printf("Running Test 1:\n");
    if (test1() == FAILURE) {
        printf("Test 1 failed\n");
        result = FAILURE;
    } else {
        printf("passed\n");
    }

    printf("Running Test 2: ");
    if (test2() == FAILURE) {
        printf("Test 2 failed\n");
        result = FAILURE;
    } else {
        printf("passed\n");
    }

    printf("Running Test 3: ");
    if (test3() == FAILURE) {
        printf("Test 3 failed\n");
        result = FAILURE;
    } else {
        printf("passed\n");
    }

    printf("Running Test 4: ");
    if (test4() == FAILURE) {
        printf("Test 4 failed\n");
        result = FAILURE;
    } else {
        printf("passed\n");
    }

    printf("Running Test 5: ");
    if (test5() == FAILURE) {
        printf("Test 5 failed\n");
        result = FAILURE;
    } else {
        printf("passed\n");
    }

    printf("Running Test 6: ");
    if (test6() == FAILURE) {
        printf("Test 6 failed\n");
        result = FAILURE;
    } else {
        printf("passed\n");
    }

    printf("Running Test 7: ");
    if (test7() == FAILURE) {
        printf("Test 7 failed\n");
        result = FAILURE;
    } else {
        printf("passed\n");
    }
    printf("Running Test 8: ");
    if (test8() == FAILURE) {
        printf("Test 8 failed\n");
        result = FAILURE;
    } else {
        printf("passed\n");
    }

    printf("Running Test 9: ");
    if (test9() == FAILURE) {
        printf("Test 9 failed\n");
        result = FAILURE;
    } else {
        printf("passed\n");
    }

    printf("Running Test 10: ");
    if (test10() == FAILURE) {
        printf("Test 10 failed\n");
        result = FAILURE;
    } else {
        printf("passed\n");
    }
    printf("Running Test 11: ");
    if (test11() == FAILURE) {
        printf("Test 11 failed\n");
        result = FAILURE;
    } else {
        printf("passed\n");
    }
    printf("Running Test 12: ");
    if (test12() == FAILURE) {
        printf("Test 12 failed\n");
        result = FAILURE;
    } else {
        printf("passed\n");
    }

    printf("Running Test 13: ");
    if (test13() == FAILURE) {
        printf("Test 13 failed\n");
        result = FAILURE;
    } else {
        printf("passed\n");
    }

    printf("Running Test 14: ");
    if (test14() == FAILURE) {
        printf("Test 14 failed\n");
        result = FAILURE;
    } else {
        printf("passed\n");
    }

    printf("Running Test 15: ");
    if (test15() == FAILURE) {
        printf("Test 15 failed\n");
        result = FAILURE;
    } else {
        printf("passed\n");
    }

    printf("Running Test 16: ");
    if (test16() == FAILURE) {
        printf("Test 16 failed\n");
        result = FAILURE;
    } else {
        printf("passed\n");
    }

    printf("Running Test 17: ");
    if (test17() == FAILURE) {
        printf("Test 17 failed\n");
        result = FAILURE;
    } else {
        printf("passed\n");
    }

    /****** Gathering memory checking info *****/
    stop_memory_check();
    /****** Gathering memory checking info *****/

    if (result == FAILURE) {
        printf("Some student tests failed.\n");
        exit(EXIT_FAILURE);
    }
        printf("All tests passed.\n");
    return EXIT_SUCCESS;
}
