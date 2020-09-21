#ifndef PARSER_H
#define PARSER_H

void print_header(int flags[]);

int print_info(int pid, int flags[]);

int iterate_proc();

char getState(int id);

int getUtime(int id);

int getStime(int id);

int getVm(int id);

void printCmd(int id);

#endif
