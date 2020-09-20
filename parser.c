//Author: Connor Hanson, Tiger Ji

#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#include "parser.h"
#include "optproc.h"

//Print output categories
void print_header(int flags[]) {
	printf("%s\t\t", "pid");

	if (flags[1])
		printf("%s\t", "State");	
	if (flags[2])
		printf("%s\t", "U_Time");
	if (flags[3])
		printf("%s\t", "S_time");
	if (flags[4])
		printf("%s\t", "VMem");
	if (flags[5])
		printf("%s\t", "CL Arg");
}

// format everything with a tab (or 2) following it
// print according to user specified flags
// return 1 if all good, -1 for error
int print_info(int pid, int flags[]) {
	// align stuff
	if (pid < 100000) {
		printf("%d%s\t\t", pid, ": ");
	} else {
		printf("%d%s\t", pid, ": ");
	}

    if (flags[1]) {
        char resp = getState(pid);
        if (resp != '~') {
            printf("%s %c\t", "s", resp);
        } else {
            printf("Error getting state");
			return -1;
        }
    }

    if (flags[2]) {
		int Utime = getUtime(pid);
		if (Utime != -1) {
			printf("%s %d\t", "U", Utime);
		} else {
			printf("Error with Utime");
			return -1;
		}
    }

    if (flags[3]) {
		int Stime = getStime(pid);
		if (Stime != -1) {
			printf("%s %d\t", "S", Stime);
		} else {
			printf("Error with Stime");
			return -1;
		} 
    }

    if (flags[4]) {
		int vm = getVm(pid);
		if (vm != -1) {
			printf("%s %d\t", "v", vm);
		} else {
			printf("Error with vm!");
			return -1;
		}
    }

    if (flags[5]) {
        printCmd(pid);
    }

	printf("\n");
	return 1;
}

// check if needs iteration through /proc
// FIXME: probably bad practice using optproc flags here
// print header, and call print method for either 1 or all pids
// return -1 if error, else 1
int iterate_proc(int flags[]) {
	print_header(flags);

	// if needing to print for a specific pid
	if (flags[0] != 0) {
		print_info(flags[0], flags);
	} 
	
	else {
		// cite this
		DIR *dir;
		struct dirent *entry;

		if ((dir = opendir("/proc")) == NULL) {
			perror("opendir() error");
		} else {
			// iterate through each entry in directory
			while ((entry = readdir(dir)) != NULL) {
				int pid = atoi(entry->d_name);
				// if 0 is string
				if (!pid) {
					continue;
				}
				if (print_info(pid, flags) == -1) {
					printf("%s%d\n", "Error getting info for pid: ", pid);
					return -1;
				}
			}
		closedir(dir);
		}
	}
	return 1;
}

/* return state charcter of process <id>
 *
 * param:
 * 	int id - pid of target process
 * returns: char in /proc/pid/stat state field
 *	    '~' if an error occurs
 */
char getState(int id) {
	FILE *statFile;
	int pid;
	char comm[50];
	char state;
	char *addressFull;
	
	if((addressFull = malloc(20))) {
		sprintf(addressFull, "/proc/%d", id);
		strcat(addressFull, "/stat");
		
		if((statFile = fopen(addressFull, "r"))) {
			fscanf(statFile, "%d %s %c", &pid, comm, &state);
			fclose(statFile);
		}
		else {
			state = '~'; //error
		}	
		free(addressFull);
	}	
	return state;
}

/* Return user time used by process <id>
 * 
 * param:
 *	int id - pid of target process
 * returns: int in /proc/id/stat utime field
 *	    -1 in case of error
 */
int getUtime(int id) {
	FILE *statFile;
	int i, utime;
	char comm[50]; // alloc more space if stack smashes happen
	char state;
	char *addressFull;
	
	if((addressFull = malloc(20))) {
		sprintf(addressFull, "/proc/%d", id);
		strcat(addressFull, "/stat");
		
		if((statFile = fopen(addressFull, "r"))) {
			fscanf(statFile, "%d %s %c", &utime, comm, &state); //scan through until last non-int field
			for(i = 0; i < 11; i++) {
				fscanf(statFile, "%d", &utime); //scan through ints until utime field
			}
			fclose(statFile);
		}
		else {
			utime = -1;
		}	
		free(addressFull);
	}	
	return utime;
}

/* return System time used by process <id>
 * 
 * param:
 *	int id - pid of target process
 * returns: int in /proc/id/stat stime field
 *	    -1 in case of error
 */
int getStime(int id) {
	FILE *statFile;
	int i, stime;
	char comm[50];
	char state;
	char *addressFull;
	
	if((addressFull = malloc(20))) {
		sprintf(addressFull, "/proc/%d", id);
		strcat(addressFull, "/stat");
		
		if((statFile = fopen(addressFull, "r"))) {
			fscanf(statFile, "%d %s %c", &stime, comm, &state); //scan until last non-int field
			for(i = 0; i < 12; i++) {
				fscanf(statFile, "%d", &stime); //scan through ints until stime field
			}
			fclose(statFile);
		}
		else {
			stime = -1;
		}	
		free(addressFull);
	}	
	return stime;
}

/* return Virtual mem in pages for process <id>
 *
 * param:
 *	int id - pid of target process
 * returns: vm used in /proc/id/statm 'size' field
 *	    -1 in case of error
 */
int getVm(int id) {
	FILE *statFile;
	int vm;
	char *addressFull;
	
	if((addressFull = malloc(20))) {
		sprintf(addressFull, "/proc/%d", id);
		strcat(addressFull, "/statm");
		
		if((statFile = fopen(addressFull, "r"))) {
			fscanf(statFile, "%d", &vm);
			fclose(statFile);
		}
		else {
			vm = -1;
		}	
		free(addressFull);
	}	
	return vm;
}

/* print command starting process <id> found in /proc/id/cmdline
 *
 * param:
 *	int id - pid of target process
 */
void printCmd(int id) {
	FILE *cmdFile;
	char next;
	char *addressFull;
	
	if((addressFull = malloc(20))) {
		sprintf(addressFull, "/proc/%d", id);
		strcat(addressFull, "/cmdline");
		
		if((cmdFile = fopen(addressFull, "r"))) {
			next = fgetc(cmdFile);
			while(next != EOF) {
				if(next != '\0') {
					printf("%c", next);
				}
				next = fgetc(cmdFile);
			}
			printf("\n");
			fclose(cmdFile);
		}
		free(addressFull);
	}
}
