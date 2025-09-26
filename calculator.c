//---------------------------------------------------------------
// Assignment : Lab_4a-Pipes (calculator)
// Date       : 09/25/25
//
// Author     : Lab_4a-Pipes Team #24 (Aiden Smith, Braden Drake)
// Emails     : smit29ab@dukes.jmu.edu, drakebt@dukes.jmu.edu
//
// File Name  : calculator.c
//---------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(char argc, char *argv[]) {
    // Read and write ends of pipe fds
    int reqfd = atoi(argv[1]);
    int rplyfd = atoi(argv[2]);

    // Print calculator process id and receiving from fd
    printf("This is the Calculator process (id = %d).\n", getpid());
    printf("Calculator Waiting to receive from FD %d\n", reqfd);

    // Read in the expression from user, print error and exit on failure
    char buffer[20];
    if (read(reqfd, buffer, sizeof(buffer) - 1) < 0) {
        perror("failed to read from user");
        exit(1);
    }

    // Assign the values and operator from the buffer
    int value1, value2;
    char operator;
    sscanf(buffer, "%d %c %d", &value1, &operator, &value2);

    printf("        Calculator received the following: %d %c %d from FD %d\n", value1, operator, value2, reqfd);

    // Compute the result depending on the operator
    int result = 0;
    switch(operator) {
        // Addition
        case '+':
            result = value1 + value2;
            break;
        // Subtraction
        case '-':
            result = value1 - value2;
            break;
        // Multiplication
        case '*':
            result = value1 * value2;
            break;
        // Division
        case '/':
            result = value1 / value2;
            break;
        // Anything else
        default:
            printf("Invalid operation\n");
            break;
    }

    // Write back the result to the user
    printf("Calculator sent the following to the User: %d on FD %d\n", result, rplyfd);
    write(rplyfd, &result, sizeof(result));

    return 0;
}