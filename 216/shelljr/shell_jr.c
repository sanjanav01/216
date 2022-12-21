#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <string.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>

#define MAX 1024 /* max input length */

/*
Sanjana Vellanki
UID: 117990881
Directory ID: svellank
Last Updated: December 4, 2022
*/

int main(){
    char *argv[MAX + 1], line[MAX + 1], 
          arg1[MAX + 1], arg2[MAX + 1];

    pid_t pid;
    int values, cd_val;

    printf("shell_jr: ");
    fflush(stdout);
    
    while(fgets(line, MAX + 1, stdin) != NULL ) {
 
        /* READING COMMANDS */
        line[strcspn(line, "\n")] = '\0';  /* replace new line char with null*/

        values = sscanf(line, "%s%s", arg1, arg2);

        if(values == 1){ /* command line has only one arg */
            argv[0] = arg1;
            argv[1] = NULL;
        } else { /* takes first two command line args */
            argv[0] = arg1;
            argv[1] = arg2;
            argv[2] = NULL;
        }

        /* PROCESSING COMMANDS */
        if(strcmp(argv[0], "exit") == 0 || 
           strcmp(argv[0], "hastalavista") == 0){ /* exit or hastalavista */

            printf("See you\n");
            fflush(stdout);
            exit(0);
        } else if (strcmp(argv[0], "cd") == 0){ /* cd */
            if(argv[1]){ /* if directory arg is given */
                cd_val = chdir(argv[1]);
                if(cd_val == -1){ /* invalid directory */
                    err(EX_OSERR, "Cannot change to directory %s", argv[1]);
	                fflush(stdout);
                }
            } else { /* invalid command - no directory given*/
                printf("Failed to execute %s", argv[0]);
                fflush(stdout);
            }
        } else { /* other commands */

            if ((pid = fork()) < 0){ /* error (-1) */
                err(EX_OSERR, "fork error");
            }
            
            if (pid) { /* parent (>0) */
                wait(NULL);
            } else { /* child (0) */
                execvp(argv[0], argv);
	            printf("Failed to execute %s\n", argv[0]); /* invaldi command */
	            fflush(stdout);
	            exit(EX_OSERR);
            }
        }

        printf("shell_jr: ");
        fflush(stdout);
    }

    return 0;
}

