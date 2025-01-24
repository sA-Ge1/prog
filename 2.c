#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char *argv[]) {
    printf("Main Function:\n");
    pid_t pid = fork();
    int status;
   
    if (pid < 0) {
        printf("Error in fork operation\n");
        exit(1);
    }

    if (pid == 0) {
        printf("Child PID: %d, Parent PID: %d\n", getpid(), getppid());
        execlp("/bin/ls","ls", "-la",NULL);
        exit(1); 
    }

    printf("Parent PID: %d\n", getpid());
    wait(&status);
    
    if (WIFEXITED(status)) {
        printf("Child terminated normally\n");
    } else {
        printf("Child terminated abnormally\n");
        exit(1);
    }

    return 0;
}
