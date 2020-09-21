//Author: Connor Hanson, Tiger Ji
// Processes the options given in the command line by the user

#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "optproc.h"

/* set flags based on arguments
 *
 * param:
 *	int argc - argc from command input
 *	char* argv[] - argv from command input
 *	int flags[] - flags to be set
 * returns: 1 if flags set successfully
 *	    -1 otherwise
 */
int set_flags(int argc, char* argv[], int flags[]) {
    int c = 0;
    int pid;
    extern char* optarg;
    extern int optind;

    while ((c = getopt(argc, argv, "p:s::U::S::v::c::")) != -1) {
        switch (c) {
            case 'p':       
                flags[0] = 1;
                pid = atoi(optarg);
                flags[0] = pid;
                if (pid == 0) { // cannot be converted to int, includes spaces
                    printf("Error, pid could not be converted to int");
                    return -1;
                }
                printf("%d \n", pid);
            break;

            case 's': 
                flags[1] = 1; 
                // check strlen to only allow -s & -s- to be valid
                if (optarg != NULL && (strlen(optarg) == 1)) {
                    flags[1] = 0;
                } else if (optarg != NULL && strlen(optarg) > 1) {
                    fprintf(stderr, "Syntax error: -s option"); 
                    return -1;
                }                
            break;

            case 'U':
                flags[2] = 1;
                if (optarg != NULL && (strlen(optarg) == 1)) {
                    flags[2] = 0;
                } else if (optarg != NULL && strlen(optarg) > 1) {
                    fprintf(stderr, "Syntax error: -U option"); 
                    return -1;
                }
            break;

            case 'S': 
                flags[3] = 1;
                if (optarg != NULL && strlen(optarg) > 1) {
                    fprintf(stderr, "Syntax error: -S option");
                    return -1;
                }
            break;

            case 'v':
                flags[4] = 1;
                if (optarg != NULL && strlen(optarg) > 1) {
                    fprintf(stderr, "Syntax error: -v option");
                    return -1;
                }
            break;

            case 'c':
                flags[5] = 1;
                if (optarg != NULL && (strlen(optarg) == 1)) {
                    flags[5] = 0;
                } else if (optarg != NULL && strlen(optarg) > 1) {
                    fprintf(stderr, "Syntax error: -c option"); 
                    return -1;
                }
            break;

            // unexpected character
            case '?': 
                return -1;

                // don't wanna crash if something unexpected
            default: 
                return -1;
        }
    }

    return 1;
}
