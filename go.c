#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// wrapper function for fork()
pid_t Fork(void) {
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(-1);
    }
    return pid;
}

int main() {
    // user and calc pids
    pid_t pidUser, pidCalc;

    // create pipes
    int reqPipe[2];
    int rplyPipe[2];
    if (pipe(reqPipe) == -1 || pipe(rplyPipe) == -1) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    printf("Go started and created these pipes\n");
    printf("    User-to-Calculator Request pipe: read=%d write=%d", reqPipe[0], reqPipe[1]);
    printf("    Calculator-to-User Reply pipe: read=%d write=%d", rplyPipe[0], rplyPipe[1]);

    // fork to create the user child
    pidUser = Fork();
    if (pidUser == 0) {
        // pass in the read/write pipe ends to the file
        // descriptor arguments and execute user
        char fd1[10], fd2[10];
        snprintf(fd1, sizeof(fd1), "%d", reqPipe[1]);
        snprintf(fd2, sizeof(fd2), "%d", rplyPipe[0]);
        if (execl("./user", "user", fd1, fd2, NULL) < 0) {
            perror("execl user failed");
            return -1;
        }
    } else {
        // Parent process

        // fork to create the calculator child
        pidCalc = Fork();
        if (pidCalc == 0) {
            // pass in the read/write pipe ends to the file
            // descriptor arguments and execute calculator
            char fd1[10], fd2[10];
            snprintf(fd1, sizeof(fd1), "%d", reqPipe[0]);
            snprintf(fd2, sizeof(fd2), "%d", rplyPipe[1]);
            if (execl("./calculator", "calculator", fd1, fd2, NULL) < 0) {
                perror("execl calculator failed");
                return -1;
            }
        } else {
            // Parent process

            printf("Go is now waiting for user to terminate\n");
            // wait for user
            waitpid(pidUser, NULL, 0);
            printf("Go is now waiting for Calculator to terminate\n");
            // wait for calculator
            waitpid(pidCalc, NULL, 0);
        }
    }

    printf("The Go process has terminated\n");
    return 0;
}