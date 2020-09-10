#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include "parser.h"

char getState(int id) {
	FILE *statFile;
	int pid;
	char comm[10];
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
			state = 0;
		}	
		free(addressFull);
	}	
	return state;
}

int getUtime(int id) {
	FILE *statFile;
	int i, utime, pid, ppid;
	char comm[10];
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
	char comm[10];
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

int main(int argc, char *argv[]) {
	if(argc == 2) {
		int test = atoi(argv[1]);
		printf("test = %d\n", test);
		int utime = getVm(test);
		printf("utime: %d\n", utime);
	}
	
	return (0);
}
