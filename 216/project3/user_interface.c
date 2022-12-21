#include "document.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#define MAX_FILE_SIZE 1024

/*
Sanjana Vellanki
UID: 117990881
Directory ID: svellank
Last Updated: October 25, 2022
*/


/* helper methods*/
static int find_commands(char line[]);
static int is_command_valid(Document *doc, char *command);
static int end_program(char commands[]);

/*testing commands*/
static int test_add_paragraph_after(Document *doc, char *command);
static int test_add_line_after(Document *doc, char *command);
static int test_print_document(Document *doc, char *command);
static int test_append_line(Document *doc, char *command);
static int test_remove_line(Document *doc, char *command);
static int test_load_file(Document *doc, char *command);
static int test_replace_text(Document *doc, char *command);
static int test_append_line(Document *doc, char *command);
static int test_highlight_text(Document *doc, char *command);
static int test_remove_text(Document *doc, char *command);
static int test_save_document(Document *doc, char *command);
static int test_reset_document(Document *doc, char *command);

/* tests add_paragraph function given the parameters and a document.
Returns FAILURE if:
a. PARAGRAPH_NUMBER does not represent a number
b. PARAGRAPH_NUMBER is a negative value
c. PARAGRAPH_NUMBER is missing
d. Additional information is provided after the PARAGRAPH_NUMBER
If add_paragraph function returns FAILURE, prints failure message to standard
ouput */
static int test_add_paragraph_after(Document *doc, char *command) {
    char name[MAX_FILE_SIZE + 1], garbage[MAX_FILE_SIZE + 1];
    int values, num;

    values = sscanf(command, "%s%d%s", name, &num, garbage);
    if (values == 2 && num >= 0) {
        if (add_paragraph_after(doc, num) == FAILURE) {
            fprintf(stdout, "add_paragraph_after failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/* tests add_line_after function given the parameters and a document.
Returns FAILURE if:
a. PARAGRAPH_NUMBER does not represent a number
b. PARAGRAPH_NUMBER is a negative value or 0
c. PARAGRAPH_NUMBER is missing
d. LINE_NUMBER does not represent a number
e. LINE_NUMBER is a negative value
f. LINE_NUMBER is missing
g. * is missing
If add_line_after function returns FAILURE, prints failure message to standard
ouput */
static int test_add_line_after(Document *doc, char *command) {
    char name[MAX_FILE_SIZE + 1], line[MAX_FILE_SIZE + 1];
    char input[MAX_FILE_SIZE + 1];
    int values, num1, num2;

    values = sscanf(command, "%s%d%d%s", name, &num1, &num2, input);
    if (values == 4 && num1 > 0 && num2 >= 0) {
        if (strchr(command, '*') != NULL) {
            strcpy(line, strchr(command, '*') + 1);

            if (add_line_after(doc, num1, num2, line) == FAILURE) {
                fprintf(stdout, "add_line_after failed\n");
            }
            return SUCCESS;
        }
    }
    return FAILURE;
}

/* tests print_document function given the parameters and a document.
Returns FAILURE if:
a. any extra data is present after print_document */
static int test_print_document(Document *doc, char *command) {
    char name[MAX_FILE_SIZE + 1], garbage[MAX_FILE_SIZE + 1];
    int values;

    values = sscanf(command, "%s%s", name, garbage);
    if (values == 1) {
        if (print_document(doc) == FAILURE) {
            fprintf(stdout, "print_document failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/* tests aappend_line function given the parameters and a document.
Returns FAILURE if:
a. PARAGRAPH_NUMBER does not represent a number
b. PARAGRAPH_NUMBER is a negative value or 0
c. PARAGRAPH_NUMBER is missing
d. * is missing
If append_line function returns FAILURE, prints failure message to standard
ouput */
static int test_append_line(Document *doc, char *command) {
    char name[MAX_FILE_SIZE + 1], line[MAX_FILE_SIZE + 1], input[MAX_FILE_SIZE + 1];
    int values, num;

    values = sscanf(command, "%s%d%s", name, &num, input);
    if (values == 3 && num > 0) {

        if (strchr(command, '*') != NULL) {
            strcpy(line, strchr(command, '*') + 1);
            

            if (append_line(doc, num, line) == FAILURE) {
                fprintf(stdout, "%d\n%s", num, line);
                fprintf(stdout, "append_line failed\n");
            }   
            return SUCCESS;
        }
    }
    return FAILURE;
}

/* tests remove_line function given the parameters and a document.
Returns FAILURE if:
a. PARAGRAPH_NUMBER does not represent a number
b. PARAGRAPH_NUMBER is a negative value or 0
c. PARAGRAPH_NUMBER is missing
d. LINE_NUMBER does not represent a number
e. LINE_NUMBER is a negative value or 0
f. LINE_NUMBER is missing
g. Any data appears after the line number
If remove_line function returns FAILURE, prints failure message to standard
ouput */
static int test_remove_line(Document *doc, char *command) {
    char name[MAX_FILE_SIZE + 1], garbage[MAX_FILE_SIZE + 1];
    int values, num1, num2;

    values = sscanf(command, "%s%d%d%s", name, &num1, &num2, garbage);
    if (values == 3 && num1 > 0 && num2 >= 0) {
        if (remove_line(doc, num1, num2) == FAILURE) {
            fprintf(stdout, "remove_line failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/* tests load_file function given the parameters and a document.
Returns FAILURE if:
a. FILENAME is missing
b. Any data appears after FILENAME
If load_file function returns FAILURE, prints failure message to standard
ouput */
static int test_load_file(Document *doc, char *command) {
    char name[MAX_FILE_SIZE + 1], filename[MAX_FILE_SIZE + 1],
        garbage[MAX_FILE_SIZE + 1];
    int values;

    values = sscanf(command, "%s%s%s", name, filename, garbage);
    if (values == 2) {
        if (load_file(doc, filename) == FAILURE) {
            fprintf(stdout, "load_file failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/* tests replace_text function given the parameters and a document.
Returns FAILURE if:
a. Both "TARGET" and "REPLACEMENT" are missing
b. Only "TARGET" is provided
If replace_text function returns FAILURE, prints failure message to standard
ouput */
static int test_replace_text(Document *doc, char *command) {
    char name[MAX_FILE_SIZE + 1], temp1[MAX_FILE_SIZE + 1],
        temp2[MAX_FILE_SIZE + 1], target[MAX_FILE_SIZE + 1],
        replace[MAX_FILE_SIZE + 1], *ptr1, *ptr2;
    int values;

    values = sscanf(command, "%s%s%s", name, temp1, temp2);
    if (values == 3 && temp1 != NULL && temp2 != NULL) {

        /* extracts target value from quotes*/
        if (strchr(temp1, '\"') != NULL) {
            ptr1 = strchr(command, '\"') + 1;
            strcpy(target, ptr1);
            if (strchr(target, '\"') != NULL) {
                ptr2 = strchr(target, '\"');
                *ptr2 = '\0';
            }
        }

        /* extracts replace value from quotes*/
        if (strchr(temp2, '\"') != NULL) {
            ptr1 = ptr1 + strlen(target) + 1;
            strcpy(replace, strchr(ptr1, '\"') + 1);
            if (strchr(replace, '\"') != NULL) {
                ptr2 = strchr(replace, '\"');
                *ptr2 = '\0';
            }
        }
        if ((replace_text(doc, target, replace)) == FAILURE) {
            fprintf(stdout, "replace_text failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/* tests highlight_text function given the parameters and a document.
Returns FAILURE if:
a. "TARGET" is missing */
static int test_highlight_text(Document *doc, char *command) {
    char name[MAX_FILE_SIZE + 1], temp1[MAX_FILE_SIZE + 1],
        target[MAX_FILE_SIZE + 1], *ptr1, *ptr2;
    int values;

    values = sscanf(command, "%s%s", name, temp1);
    if (values == 2 && temp1 != NULL) {
        if (strchr(temp1, '\"') != NULL) {
            ptr1 = strchr(command, '\"') + 1;
            strcpy(target, ptr1);
            if (strchr(target, '\"') != NULL) {
                ptr2 = strchr(target, '\"');
                *ptr2 = '\0';
            }
            highlight_text(doc, target);
            return SUCCESS;
        }
    }
    return FAILURE;
}

/* tests remove_text function given the parameters and a document.
Returns FAILURE if:
a. "TARGET" is missing */
static int test_remove_text(Document *doc, char *command) {
    char name[MAX_FILE_SIZE + 1], temp1[MAX_FILE_SIZE + 1],
        target[MAX_FILE_SIZE + 1], *ptr1, *ptr2;
    int values;

    values = sscanf(command, "%s%s", name, temp1);
    if (values == 2 && temp1 != NULL) {
        if (strchr(temp1, '\"') != NULL) {
            ptr1 = strchr(command, '\"') + 1;
            strcpy(target, ptr1);
            if (strchr(target, '\"') != NULL) {
                ptr2 = strchr(target, '\"');
                *ptr2 = '\0';
            }
            remove_text(doc, target);
            return SUCCESS;
        }
    }
    return FAILURE;
}

/* tests save_document function given the parameters and a document.
Returns FAILURE if:
a. FILENAME is missing.
b. Any data appears after FILENAME.
If save_document function returns FAILURE, prints failure message to standard
ouput */
static int test_save_document(Document *doc, char *command) {
    char name[MAX_FILE_SIZE + 1], filename[MAX_FILE_SIZE + 1],
        garbage[MAX_FILE_SIZE + 1];
    int values;

    values = sscanf(command, "%s%s%s", name, filename, garbage);
    if (values == 2) {
        if (save_document(doc, filename) == FAILURE) {
            fprintf(stdout, "save_document failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/* tests reset_document function given the parameters and a document.
Returns FAILURE if:
a. Any data appears after reset_document */
static int test_reset_document(Document *doc, char *command) {
    char name[MAX_FILE_SIZE + 1], garbage[MAX_FILE_SIZE + 1];
    int values;

    values = sscanf(command, "%s%s", name, garbage);
    if (values == 1) {
        reset_document(doc);
        return SUCCESS;
    }
    return FAILURE;
}

/* Validates all passed commands based on specified command name. If
designated function to command returns FAILURE or commands does not exist,
a notifying message is printed to standard output (Returns FAILURE in this case)
 */
static int is_command_valid(Document *doc, char *command) {
    if (strstr(command, "add_paragraph_after") != NULL) {
        if (test_add_paragraph_after(doc, command) == SUCCESS) {
            return SUCCESS;
        }
    } else if (strstr(command, "add_line_after") != NULL) {
        if (test_add_line_after(doc, command) == SUCCESS) {
            return SUCCESS;
        }
    } else if (strstr(command, "append_line") != NULL) {
        if (test_append_line(doc, command) == SUCCESS) {
            return SUCCESS;
        }
    } else if (strstr(command, "print_document") != NULL) {
        if (test_print_document(doc, command) == SUCCESS) {
            return SUCCESS;
        }
    } else if (strstr(command, "remove_line") != NULL) {
        if (test_remove_line(doc, command) == SUCCESS) {
            return SUCCESS;
        }
    } else if (strstr(command, "load_file") != NULL) {
        if (test_load_file(doc, command) == SUCCESS) {
            return SUCCESS;
        }
    } else if (strstr(command, "replace_text") != NULL) {
        if (test_replace_text(doc, command) == SUCCESS) {
            return SUCCESS;
        }
    } else if (strstr(command, "highlight_text") != NULL) {
        if (test_highlight_text(doc, command) == SUCCESS) {
            return SUCCESS;
        }
    } else if (strstr(command, "remove_text") != NULL) {
        if (test_remove_text(doc, command) == SUCCESS) {
            return SUCCESS;
        }
    } else if (strstr(command, "save_document") != NULL) {
        if (test_save_document(doc, command) == SUCCESS) {
            return SUCCESS;
        }
    } else if (strstr(command, "reset_document") != NULL) {
        if (test_reset_document(doc, command) == SUCCESS) {
            return SUCCESS;
        }
    }
    fprintf(stdout, "Invalid Command\n");
    return FAILURE;
}

/* Finds commands from given file line. If line is a blank line or comment,
FAILURE is returned. */
static int find_commands(char line[]) {
    int values;
    char c, str[MAX_FILE_SIZE + 1];
    
    if ((values = sscanf(line, "%c%s", &c, str)) != EOF) {

        /*blank line*/
        if (isspace(c)  && values < 2) {
            return FAILURE;
        /* comments */
        } else if (isspace(c) && strstr(str, "#") != NULL){
            return FAILURE;
        } else if (c == '#'){
            return FAILURE;
        }
    }
    return SUCCESS;
    
}

/* terminates program if quit or exit are entered. If data appears
after quit/exit, notifying message is printed to stanard output. */
int end_program(char commands[]) {
    char input[MAX_FILE_SIZE + 1];
    int values;

    values = sscanf(commands, "%s", input);
    if (values != 1){
        fprintf(stdout, "Invalid Command");
    }
    exit(EXIT_SUCCESS);
    
}

/* Reads command line for specified arguments. If a filename is provided,
reads the file (if it exists) to validate commands. If no filenmae is
provided, prints out a '> ' to standarrd output to
prompt manual input of data. */
int main(int argc, char *argv[]) {
    Document doc;
    const char *doc_name = "main_document";
    char commands[MAX_FILE_SIZE + 1];
    FILE *input;

    if (init_document(&doc, doc_name) == FAILURE) {
        fprintf(stdout, "Initialization failed\n");
    } else {
        if (argc == 1) {
            input = stdin;
            fprintf(stdout, "> ");
            while (fgets(commands, MAX_FILE_SIZE + 1, stdin) != NULL) {
                if (strcmp(commands, "quit\n") == 0 ||
                    strcmp(commands, "exit\n") == 0) {
                    break;
                } else {
                    if (find_commands(commands) == SUCCESS) {
                        commands[strcspn(commands, "\n")] = '\0';
                        is_command_valid(&doc, commands);
                    }
                }
                fprintf(stdout, "> ");
            }
        } else if (argc == 2) {
            if ((input = fopen(argv[1], "r")) != NULL) {
                while (fgets(commands, MAX_FILE_SIZE + 1, input) != NULL) {
                    if (strcmp(commands, "quit\n") == 0 ||
                        strcmp(commands, "exit\n") == 0) {
                        break;
                    } else {
                        if (find_commands(commands) == SUCCESS) {
                            commands[strcspn(commands, "\n")] = '\0';
                            is_command_valid(&doc, commands);
                        }
                    }
                }
            } else {
                fprintf(stderr, "%s cannot be opened.\n", argv[1]);
                exit(EX_OSERR);
            }
        } else {
            fprintf(stderr, "Usage: user_interface\n");
            fprintf(stderr, "Usage: user_interface <filename>\n");
            exit(EX_USAGE);
        }
    }
    fclose(input);
    end_program(commands);
    return FAILURE;
}