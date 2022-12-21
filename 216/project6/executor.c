/*
Sanjana Vellanki
UID: 117990881
Directory ID: svellank
Last Updated: December 12, 2022
*/

#define MAX_LINE_SIZE 1024
#define FILE_PERMISSIONS 0664
#include "executor.h"
#include "command.h"
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <unistd.h>


/* static void print_tree(struct tree *t); */
static int process_tree(struct tree *t, int input, int output);

/* calls the process_tree function if tree is not null to traverse */
int execute(struct tree *t) {
    if (t) {
        /* process tree if tree is not null*/
        return process_tree(t, STDIN_FILENO, STDOUT_FILENO);
    }
    return 0;
}

/* traverses the tree for the possible nodes: NONE, AND, PIPE, SUBSHELL */
int process_tree(struct tree *t, int file_input, int file_output) {
    pid_t pid;
    int status, pipe_fd[2];

    /***** CASE 1: NO CONJUNCTION *****/
    if (t->conjunction == NONE) {

        /* 1. cd command */
        if (strcmp(t->argv[0], "cd") == 0) {
            if (chdir(t->argv[1]) == -1) {
                perror(t->argv[0]);
            }

            /* 2. exit command */
        } else if (strcmp(t->argv[0], "exit") == 0) {
            exit(0);

            /* other command: fork */
        } else {
            if ((pid = fork()) < 0) { /* error (-1) */
                perror("fork error.\n");
            } else {
                if (pid) { /* parent (1) */
                    wait(&status);
                    return status;

                } else {
                    /* redirecting stdin to file input */
                    if (t->input) { /* if input exists in tree */

                        if ((file_input = open(t->input, O_RDONLY)) < 0) {
                            perror("File cannot be opened.\n");
                        }
                        if (dup2(file_input, STDIN_FILENO) < 0) {
                            perror("dup2 could not be conducted.\n");
                        }
                        close(file_input);
                    }
                    /* redirecting stdout to file output */
                    if (t->output) {
                        if ((file_output =
                                 open(t->output, O_WRONLY | O_TRUNC | O_CREAT |
                                                     FILE_PERMISSIONS)) < 0) {
                            perror("File cannot be opened.\n");
                        }
                        if (dup2(file_output, STDOUT_FILENO) < 0) {
                            perror("dup2 could not be conducted.\n");
                        }
                        close(file_output);
                    }

                    /* execute command */
                    execvp(t->argv[0], t->argv);
                    fprintf(stderr, "Failed to execute %s\n", t->argv[0]);
                    exit(EX_OSERR);
                }
            }
        }

    /***** CASE 2: AND CONJUNCTION *****/
    } else if (t->conjunction == AND) {
        /* redirecting stdin to file input */
        if (t->input) { /* if input exists in tree */

            if ((file_input = open(t->input, O_RDONLY)) < 0) {
                perror("File cannot be opened.\n");
            }
        }
        /* redirecting stdout to file output */
        if (t->output) {
            if ((file_output = open(t->output, O_WRONLY | O_TRUNC | O_CREAT |
                                                   FILE_PERMISSIONS)) < 0) {
                perror("File cannot be opened.\n");
            }
        }
        /* If left node fails, then execute right node */
        if (process_tree(t->left, file_input, file_output) == 0) {
            process_tree(t->right, file_input, file_output);
        }

    /***** CASE 3: PIPE CONJUNCTION *****/
    } else if (t->conjunction == PIPE) {
        /* ambiguous output in left node */
        if (t->left->output) {
            printf("Ambiguous output redirect.\n");

        /* ambiguous inpur in right node */
        } else
            if(t->right->input) { printf("Ambiguous input redirect.\n"); }
            
        else {
            /* redirecting stdin to file input */
            if (t->input) { /* if input exists in tree */

                if ((file_input = open(t->input, O_RDONLY)) < 0) {
                    perror("File cannot be opened.\n");
                }
            }
            /* redirecting stdout to file output */
            if (t->output) {
                if ((file_output =
                         open(t->output, O_WRONLY | O_TRUNC | O_CREAT |
                                             FILE_PERMISSIONS)) < 0) {
                    perror("File cannot be opened.\n");
                }
            }
            
            /* pipe error */
            if(pipe(pipe_fd) < 0){
                perror("pipe error.\n");
            }

            /* fork error */
            if((pid = fork()) < 0){
                perror("fork error.\n");
            } 

            if(pid){ /* parent executes the right child */
                close(pipe_fd[1]);
                if(dup2(pipe_fd[0], STDIN_FILENO) < 0){
                    perror("dup2 error.\n");
                }
                process_tree(t->right, pipe_fd[0], file_output);
                close(pipe_fd[0]);
                wait(NULL);

            } else {   /* child executes left child */
                close(pipe_fd[0]);
                if(dup2(pipe_fd[1], STDOUT_FILENO) < 0){
                    perror("dup2 error.\n");
                }
                process_tree(t->left, file_input, pipe_fd[1]);
                close(pipe_fd[1]);
                exit(0);
            }
        }

    /***** CASE 4: SUBSHELL CONJUNCTION *****/
    } else if (t->conjunction == SUBSHELL){
        /* redirecting stdin to file input */
        if (t->input) { /* if input exists in tree */

            if ((file_input = open(t->input, O_RDONLY)) < 0) {
                perror("File cannot be opened.\n");
            }
        }
        /* redirecting stdout to file output */
        if (t->output) {
            if ((file_output =
                open(t->output, O_WRONLY | O_TRUNC | O_CREAT |
                                        FILE_PERMISSIONS)) < 0) {
                perror("File cannot be opened.\n");
            }
        }

        if((pid = fork()) < 0){
            perror("fork error.\n");
        } else {
            if (pid){ /* parent */
                wait(NULL);
            } else { /* child */
                process_tree(t->left, file_input, file_output);
                exit(0);
            }
        }
    }
    return 0;
}


/*
static void print_tree(struct tree * t) {
        if (t != NULL) {
            print_tree(t->left);

            if (t->conjunction == NONE) {
                printf("NONE: %s, ", t->argv[0]);
            } else {
                printf("%s, ", conj[t->conjunction]);
            }
            printf("IR: %s, ", t->input);
            printf("OR: %s\n", t->output);

            print_tree(t->right);
        }
}
*/
