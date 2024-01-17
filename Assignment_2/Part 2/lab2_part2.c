//Author: Cebajel Tanan (210010055)
//Title: CS 314 Assignment 2 - Part 2
//Date: January 17th, 2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 0;
    }
    char * myargs[argc];
    for (int i = 0; i < argc-1; i++) {
        myargs[i] = argv[i+1];
    }
    myargs[argc - 1] = NULL;
    execvp(myargs[0], myargs);
    return 0;
}