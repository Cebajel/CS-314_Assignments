//Author: Cebajel Tanan (210010055)
//Title: CS 314 Assignment 2 - Part 1
//Date: January 17th, 2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    char str[] = "Hello World";
    for (int i = 0; i < strlen(str); i++) {
        int pid = fork();
        if (pid == 0) {
            printf("%c %d\n", str[i], (int) getpid());
            // fflush(stdout);
            int rand_num = (rand() % 4) + 1; 
            sleep(rand_num);
            continue;
        } else {
            wait(NULL);
            return 0;
        }
    }
    return 0;
}