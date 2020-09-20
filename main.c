// Author: Connor Hanson, Tiger Ji
// Process the options input into the program

#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "optproc.h"
#include "parser.h"

void print_usage() {
    printf("Usage: \n");
    printf("\tps [options]\n");
    printf("Options: \n\t-p <pid>: print process info for <pid>\n");
    printf("\t-s: print single character state info about process\n");
    printf("\t-U: print user time consumed by process. Enabled by default. -U- to disable\n");
    printf("\t-S: print system time consumed by process\n");
    printf("\t-v: print virtual memory consumed by process (in pages)\n");
    printf("\t-c: print command line which started process. Enabled by default. -c- to disable\n");
}

int main(int argc, char *argv[]) {

    // display option flags
    // p, s, U, S, v, c, -p counter
    int flags[] = {0, 0, 1, 0, 0, 1};

    int resp = set_flags(argc, argv, flags);
    
    if (resp == -1) { //error
        print_usage();
        printf("Error setting flags\n");
        return -1;
    }

    // check flags
    if (iterate_proc(flags) == -1) {
        printf("Error iterating through processes\n");
    }
    return 0;
}

