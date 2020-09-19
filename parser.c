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

void print_header() {
	printf("%s\t\t", "pid");

	if (sflag)
		printf("%s\t", "State");	
	if (Uflag)
		printf("%s\t", "U_Time");
	if (Sflag)
		printf("%s\t", "S_time");
	if (vflag)
		printf("%s\t", "VMem");
	if (cflag)
		printf("%s\t", "CL Arg");
}

// format everything with a tab following it
void print_info(int pid) {

	// align stuff
	if (pid < 10000) {
		printf("%d%s\t\t", pid, ": ");
	} else {
		printf("%d%s\t", pid, ": ");
	}

    if (sflag) {
        char resp = getState(pid);
        if (resp != '~') {
            printf("%s %c\t", "s", resp); // needs to be char
        } else {
            printf("Error doing some dumb shit idk");
        }
    }

    if (Uflag) {
        printf("%s %d\t", "U", getUtime(pid));
    }

    if (Sflag) {
        printf("%s %d\t", "S", getStime(pid));
    }

    if (vflag) {
        printf("%s %d\t", "v", getVm(pid));
    }

    if (cflag) {
        printCmd(pid);
    }

	printf("\n");
}

// check if needs iteration through /proc
// probably bad practice using optproc flags here
int iterate_proc() {
	print_header();

	if (pflag != 0) {
		print_info(pflag);
	} else {

		DIR *dir;
		struct dirent *entry;

		if ((dir = opendir("/proc")) == NULL) {
			perror("opendir() error");
		} else {
			// iterate through each entry in directory
			while ((entry = readdir(dir)) != NULL) {
				int pid = atoi(entry->d_name);
				if (!pid) {
					continue;
				}
				print_info(pid);
			}
		closedir(dir);
		}
	}
	return 1;
}

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

int getUtime(int id) {
	FILE *statFile;
	int i, utime, pid, ppid;
	char comm[50]; // alloc more space if stack smashes happen
	char state;
	char *addressFull;
	
	if((addressFull = malloc(20))) {
		sprintf(addressFull, "/proc/%d", id);
		strcat(addressFull, "/stat");
		
		if((statFile = fopen(addressFull, "r"))) {
			fscanf(statFile, "%d %s %c %d", &pid, comm, &state, &ppid);
			for(i = 0; i < 10; i++) {
				fscanf(statFile, "%d", &utime);
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

int getStime(int id) {
	FILE *statFile;
	int i, stime, pid, ppid;
	char comm[50];
	char state;
	char *addressFull;
	
	if((addressFull = malloc(20))) {
		sprintf(addressFull, "/proc/%d", id);
		strcat(addressFull, "/stat");
		
		if((statFile = fopen(addressFull, "r"))) {
			fscanf(statFile, "%d %s %c %d", &pid, comm, &state, &ppid);
			for(i = 0; i < 11; i++) {
				fscanf(statFile, "%d", &stime);
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

/*int main(int argc, char *argv[]) {
	if(argc == 2) {
		int test = atoi(argv[1]);
		printf("test = %d\n", test);
		int utime = getVm(test);
		printf("utime: %d\n", utime);
	}
	
	return (0);
}*/
