// Author: Connor Hanson
// Process the options input into the program

#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "optproc.h"

int main(int argc, char *argv[]) {

    // display option flags
    pflag = 0; // pid
    sflag = 0; // single state character info
    Uflag = 1; // user time consumed by process
    Sflag = 0; // sys time consumed by proc
    vflag = 0; // VRAM in pages used by proc
    cflag = 1; // CLA which started process

    if (argc > 1) { // check if any options provided
        if (set_flags(argc, argv) == -1) {
            // error
        }
    }

    return 0;
}

