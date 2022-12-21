#include "document.h"
#include <stdio.h>
#include <string.h>

/*
Sanjana Vellanki
UID: 117990881
Directory ID: svellank
Last Updated: October 22, 2022
*/

int is_line_blank( char line[]);

/* initializes the document and makes it empty*/
int init_document(Document *doc, const char *name) {
    if (doc != NULL && name != NULL && strlen(name) <= MAX_STR_SIZE) {
        strcpy(doc->name, name);
        doc->number_of_paragraphs = 0;
        return SUCCESS;
    }
    return FAILURE;
}

/*sets paragraph number to 0*/
int reset_document(Document *doc) {
    if (doc != NULL) {
        doc->number_of_paragraphs = 0;
        return SUCCESS;
    }
    return FAILURE;
}

/* prints out the document name and paragraph*/
int print_document(Document *doc) {
    int i, j;

    if (doc != NULL) {
        printf("Document name: \"%s\"\n", doc->name);
        printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

        for (i = 0; i < doc->number_of_paragraphs; i++) {
            if (doc->paragraphs[i].number_of_lines > 0) {
                for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
                    printf("%s\n", doc->paragraphs[i].lines[j]);
                }
                if (i < doc->number_of_paragraphs - 1) {
                    printf("\n");
                }
            }
        }
        return SUCCESS;
    }
    return FAILURE;
}

/*adds paragraph number after the specified paragraph number*/
int add_paragraph_after(Document *doc, int paragraph_number) {
    int i, paras;
    if (doc != NULL) {
        paras = doc->number_of_paragraphs;
        if (paras < MAX_PARAGRAPHS && paragraph_number <= paras) {
            if (paragraph_number == 0 && paras == 0) {
                doc->paragraphs[0].number_of_lines = 0;
            } else if (paragraph_number == doc->number_of_paragraphs) {
                doc->paragraphs[doc->number_of_paragraphs].number_of_lines = 0;
            } else {
                for (i = doc->number_of_paragraphs; i >= paragraph_number;
                     i--) {
                    doc->paragraphs[i + 1] = doc->paragraphs[i];
                }
                doc->paragraphs[paragraph_number].number_of_lines = 0;
            }
            (doc->number_of_paragraphs)++;
            return SUCCESS;
        }
    }
    return FAILURE;
}

/*adds a new line after the specified line number*/
int add_line_after(Document *doc, int paragraph_number, int line_number,
                   const char *new_line) {

    int i, lines = 0;
    char *line1, *line2;

    if (doc == NULL || new_line == NULL) {
        return FAILURE;
    }
    if (paragraph_number > doc->number_of_paragraphs) {
        return FAILURE;
    }
    lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
    if (lines < MAX_PARAGRAPH_LINES && line_number <= lines) {
        if (line_number == 0 && lines >= 0) {
            strcpy(doc->paragraphs[paragraph_number - 1].lines[0], new_line);
        } else if (line_number == lines) {
            strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number],
                   new_line);
        } else {
            /*pushes the lines forward and adds line in between*/
            for (i = lines; i >= line_number; i--) {

                line1 = doc->paragraphs[paragraph_number - 1].lines[i];
                line2 = doc->paragraphs[paragraph_number - 1].lines[i + 1];
                strcpy(line2, line1);
            }
            strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number],
                   new_line);
        }
        (doc->paragraphs[paragraph_number - 1].number_of_lines)++;
        return SUCCESS;
    }
    return FAILURE;
}

/*returns number of lines for a given paragraph number*/
int get_number_lines_paragraph(Document *doc, int paragraph_number,
                               int *number_of_lines) {
    if (doc != NULL && paragraph_number <= doc->number_of_paragraphs) {
        *number_of_lines =
            doc->paragraphs[paragraph_number - 1].number_of_lines;
        return SUCCESS;
    }
    return FAILURE;
}

/*adds a line to the specified paragraph*/
int append_line(Document *doc, int paragraph_number, const char *new_line) {
    int lines;
    if (doc != NULL && new_line != NULL) {
        lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
        if (paragraph_number <= doc->number_of_paragraphs &&
            lines < MAX_PARAGRAPH_LINES) {
            add_line_after(doc, paragraph_number, lines, new_line);
            return SUCCESS;
        }
    }
    return FAILURE;
}

/*removes a line from the specified paragraph*/
int remove_line(Document *doc, int paragraph_number, int line_number) {
    int i, j, lines = 0;
    char *line1, *line2;

    if (doc == NULL) {
        return FAILURE;
    }
    lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
    if (paragraph_number <= doc->number_of_paragraphs && line_number <= lines) {
        if (line_number < lines) {
            for (i = line_number - 1; i < lines - 1; i++) {
                line1 = doc->paragraphs[paragraph_number - 1].lines[i];
                line2 = doc->paragraphs[paragraph_number - 1].lines[i + 1];
                for (j = 0; j < MAX_STR_SIZE; j++) {
                    line1[j] = line2[j];
                }
            }
        }
        (doc->paragraphs[paragraph_number - 1].number_of_lines)--;
        return SUCCESS;
    }
    return FAILURE;
}

/*adds 'data_lines' number of lines from data to the document*/
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1],
                  int data_lines) {
    int i;
    if (doc != NULL && data != NULL && data_lines >= 0) {
        add_paragraph_after(doc, doc->number_of_paragraphs);
        for (i = 0; i < data_lines; i++) {
            if (strcmp(data[i], "") == 0) {
                add_paragraph_after(doc, doc->number_of_paragraphs);
            } else {
                append_line(doc, doc->number_of_paragraphs, data[i]);
            }
        }
        return SUCCESS;
    }
    return FAILURE;
}

/*replaces the target text with the replacement text in the document*/
int replace_text(Document *doc, const char *target, const char *replacement) {
    int i, j, lines = 0;
    char *start, *end;
    char copy[MAX_STR_SIZE + 1] = {0};

    if (doc != NULL && target != NULL && replacement != NULL) {

        for (i = 0; i < doc->number_of_paragraphs; i++) {
            lines = doc->paragraphs[i].number_of_lines;
            for (j = 0; j < lines; j++) {
                start = doc->paragraphs[i].lines[j];
                end = start + strlen(start);
                strcpy(copy, "");
                while (start< end && strstr(start, target) != NULL) {
                    strncat(copy, start, strstr(start, target) - start);
                    strcat(copy, replacement);
                    start = strstr(start, target) + strlen(target);
                }
                strcat(copy, start);
                strcpy(doc->paragraphs[i].lines[j], copy);
            }
        }
        return SUCCESS;
    }
    return FAILURE;
}

/*highlights the target text in the document*/
int highlight_text(Document *doc, const char *target) {
    char highlight[MAX_STR_SIZE + 1] = "";
    if (doc != NULL && target != NULL) {
        strcat(highlight, HIGHLIGHT_START_STR);
        strcat(highlight, target);
        strcat(highlight, HIGHLIGHT_END_STR);

        replace_text(doc, target, highlight);
        return SUCCESS;
    }
    return FAILURE;
}

/*removes the target text from everywhere in the document*/
int remove_text(Document *doc, const char *target) {
    if (doc != NULL && target != NULL) {
        replace_text(doc, target, "");
        return SUCCESS;
    }
    return FAILURE;
}

int load_file(Document *doc, const char *filename){
    FILE *input; 
    char line[MAX_STR_SIZE + 1];
    int paras = 1;
    if(doc != NULL && filename != NULL){
        if ((input = fopen(filename, "r")) != NULL){
                add_paragraph_after(doc, 0); /* default paragraph*/
                while(fgets(line, MAX_STR_SIZE + 1, input) != NULL && paras < MAX_PARAGRAPHS){
                    if (is_line_blank(line) == SUCCESS){
                        add_paragraph_after(doc, paras);
                        paras++;
                    } else {
                        line[strcspn(line, "\n")] = '/0';
                        append_line(doc, paras, line);
                    }
                }
                fclose(input);
                return SUCCESS; 
        }
    }
    return FAILURE;
}

int save_document(Document *doc, const char *filename){
    FILE *output;
    int i, j, lines;
    if(doc != NULL && filename != NULL){
        if ((output = fopen(filename, "w")) != NULL) {
            for (i = 0; i <= doc->number_of_paragraphs; i++){
                lines = doc->paragraphs[i].number_of_lines;
                for (j = 0; j <= lines; j++){
                    fputs(doc->paragraphs[i].lines[j], output);
                        if (j < lines) {
                            fputs("\n", output);
                        }
                }
                if ((i+1) < doc->number_of_paragraphs){
                    fputs("\n", output);
                }
            }
            fclose(output);
            return SUCCESS;
        }
    }
    return FAILURE;
    
}

int is_line_blank( char line[]){
    int values;
    char c, str[MAX_STR_SIZE + 1];
    if ((values = sscanf(line, "%c%s", &c, str)) != EOF){
        if (c != '#' && values < 2){
            return SUCCESS;
        }
    }
    return FAILURE;
}