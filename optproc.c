//Author: Connor Hanson, Tiger Ji

#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "optproc.h"

// return 1 if all good, else -1
// Parses the provided CLA and sets flags
int set_flags(int argc, char* argv[]) {
    int c = 0;
    int pid;
    extern char* optarg;
    extern int optind;

    while ((c = getopt(argc, argv, "p:s::U::S::v::c::")) != -1) {
        switch (c) {
            // unexpected character
            case '?': 
                return -1;

            // TODO allow multiple -p
            case 'p':
                pflag = 1;
                pid = atoi(optarg);
                pflag = pid; // FIXME
                if (pid == 0) { // cannot be converted to int
                    return -1;
                }
                printf("%d \n", pid);
            break;

            case 's': 
                sflag = 1; 

                // check strlen to only allow -s & -s- to be valid
                if (optarg != NULL && (strlen(optarg) == 1)) {
                    sflag = 0;
                } else if (optarg != NULL && strlen(optarg) > 1) {
                    fprintf(stderr, "Syntax error: -s option"); 
                    return -1;
                }                
            break;

            case 'U':
                Uflag = 1;
                if (optarg != NULL && (strlen(optarg) == 1)) {
                    Uflag = 0;
                } else if (optarg != NULL && strlen(optarg) > 1) {
                    fprintf(stderr, "Syntax error: -U option"); 
                    return -1;
                }
            break;

            case 'S': 
                Sflag = 1;
            break;

            case 'v':
                vflag = 1;
            break;

            case 'c':
                cflag = 1;
                if (optarg != NULL && (strlen(optarg) == 1)) {
                    cflag = 0;
                } else if (optarg != NULL && strlen(optarg) > 1) {
                    fprintf(stderr, "Syntax error: -c option"); 
                    return -1;
                }
            break;

                // don't wanna crash if something unexpected
            default: 
                return -1;
        }
    }

    return 1;
}
