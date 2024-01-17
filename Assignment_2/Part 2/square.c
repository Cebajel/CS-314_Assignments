#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    float num = atof(argv[argc - 1]);
    num *= num;
    printf("Square: Current process id: %d, Current result: %f\n", (int)getpid(), num);
    if (argc <= 2) {
        return 0;
    }
    char * myargs[argc];
    for (int i = 0; i < argc-1; i++) {
        myargs[i] = argv[i+1];
    }
    myargs[argc - 1] = NULL;
    char str_num[50];
    sprintf(str_num, "%f", num);
    myargs[argc - 2] = str_num;
    execvp(myargs[0], myargs);
    return 0;
}