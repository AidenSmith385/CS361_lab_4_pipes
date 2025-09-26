//---------------------------------------------------------------
// Assignment : Lab_4a-Pipes (go)
// Date       : 09/25/25
//
// Author     : Lab_4a-Pipes Team #24 (Aiden Smith, Braden Drake)
// Emails     : smit29ab@dukes.jmu.edu, drakebt@dukes.jmu.edu
//
// File Name  : go.c
//---------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Wrapper function for fork()
pid_t Fork(void) {
    pid_t pid;
    pid = fork();
    // Print error and exit on fail
    if (pid < 0) {
        perror("Fork failed");
        exit(-1);
    }
    return pid;
}

int main() {
    // User and Calc pids
    pid_t pidUser, pidCalc;

    // Create pipes
    int reqPipe[2];
    int rplyPipe[2];
    if (pipe(reqPipe) == -1 || pipe(rplyPipe) == -1) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    // Print the request and reply pipes
    printf("\nGo started and created these pipes\n");
    printf("        User-to-Calculator Request pipe: read=%d write=%d\n", reqPipe[0], reqPipe[1]);
    printf("        Calculator-to-User Reply pipe: read=%d write=%d\n", rplyPipe[0], rplyPipe[1]);

    // Fork to create the user child
    pidUser = Fork();
    if (pidUser == 0) {
        // Pass in the read/write pipe ends to the file
        // descriptor arguments and execute user
        char fd1[10], fd2[10];
        snprintf(fd1, sizeof(fd1), "%d", reqPipe[1]);
        snprintf(fd2, sizeof(fd2), "%d", rplyPipe[0]);
        
        // Print error and exit on failure
        if (execl("./user", "user", fd1, fd2, NULL) < 0) {
            perror("execl user failed");
            return -1;
        }
    } else {
        // Parent process

        // Fork to create the calculator child
        pidCalc = Fork();
        if (pidCalc == 0) {
            // Pass in the read/write pipe ends to the file
            // descriptor arguments and execute calculator
            char fd1[10], fd2[10];
            snprintf(fd1, sizeof(fd1), "%d", reqPipe[0]);
            snprintf(fd2, sizeof(fd2), "%d", rplyPipe[1]);

            // Print error and exit on failure
            if (execl("./calculator", "calculator", fd1, fd2, NULL) < 0) {
                perror("execl calculator failed");
                return -1;
            }
        } else {
            // Parent process

            // Wait for user
            printf("\nGo is now waiting for User to terminate\n");
            waitpid(pidUser, NULL, 0);

            // Wait for calculator
            printf("\nGo is now waiting for Calculator to terminate\n");
            waitpid(pidCalc, NULL, 0);
        }
    }

    printf("\nThe Go process has terminated\n");
    return 0;
}