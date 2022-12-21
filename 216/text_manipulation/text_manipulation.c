#include <stdio.h>
#include <string.h>
#include "text_manipulation.h"

int remove_spaces(const char *source, char *result, int *num_spaces_removed) { 
    int i, last_char, k = 0; 
    const char *p = source;

    if (source == NULL ){
        return FAILURE;
    } 
    if ( strlen(source) == 0){
        return FAILURE;
    }
    i = strlen(source)-1;
    while(source[i] == ' '){
        i--;
        if (num_spaces_removed != NULL){
            (*num_spaces_removed)++;
        }
    }
    last_char = i;
    i = -1;
    
    while(k != last_char + 1){
        if (*p == ' ' && i == -1 ){
            if (num_spaces_removed != NULL){
                (*num_spaces_removed)++;
            }
            ++p;
        } else {
            ++i;
            result[i] = *p;
            p++;
        }
        k++;
    }
    result[i+1] = '\0';
    return SUCCESS;
}

int center(const char *source, int width, char *result) {  
    int i = 0, k = 0, spaces;
    const char *p = source;

    if (source == NULL ){
        return FAILURE;
    } 
    spaces = (width - strlen(source))/2;

    if ( strlen(source) == 0 || width < strlen(source)){
        return FAILURE;
    }

    while(i < spaces){
        result[i] = ' ';
        i++;
    }
    while(*p != '\0'){
        result[i] = *p;
        p++;
        i++;
    }
    while(k < spaces){
        result[i] = ' ';
        k++;
        i++;
    }
    result[i] = '\0';

    return SUCCESS;

}
