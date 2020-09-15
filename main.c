// Author: Connor Hanson
// Process the options input into the program

#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "optproc.h"
#include "parser.h"

// rn assuming that pid is given
int print_ps() {
    int pid = 0;

    if (pid == 0) {
        // iterate through all pid
    }

    if (pflag) {
        pid = pflag; // fix to allow several -p
    }

    if (sflag) {
        printf("%c\n", getState(pid)); // needs to be char
    }

    if (Uflag) {
        printf("%d\n", getUtime(pid));
    }

    if (Sflag) {
        printf("%d\n", getStime(pid));
    }

    if (vflag) {
        printf("%d\n", getVm(pid));
    }

    if (cflag) {
        printCmd(pid);
    }

    return 0;
}

int main(int argc, char *argv[]) {

    // display option flags
    pflag = 0; // pid
    sflag = 0; // single state character info
    Uflag = 1; // user time consumed by process
    Sflag = 0; // sys time consumed by proc
    vflag = 0; // VRAM in pages used by proc
    cflag = 1; // CLA which started process

    int resp = set_flags(argc, argv);
    
    if (resp == -1) { //error
        printf("Error setting flags\n");
    }

    // check flags
    print_ps();
    return 0;
}

