//---------------------------------------------------------------
// Assignment : Lab_4a-Pipes (user)
// Date       : 09/25/25
//
// Author     : Lab_4a-Pipes Team #24 (Aiden Smith, Braden Drake)
// Emails     : smit29ab@dukes.jmu.edu, drakebt@dukes.jmu.edu
//
// File Name  : user.c
//---------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(char argc, char *argv[]) {
    // Read and write ends of pipe fds
    int reqfd = atoi(argv[1]);
    int rplyfd = atoi(argv[2]);

    // Print user process id and prompt user for an expression
    printf("This is the User process (id = %d).\n", getpid());
    printf("        User: Please, enter:   value1   operation   value2\n");

    // Variables to hold the expression
    int value1, value2;
    char operator;

    // Get the input expression
    scanf("%d %c %d", &value1, &operator, &value2);

    // Feed the buffer the expression and write to the pipe
    char buffer[20];
    int buffer_length = snprintf(buffer, sizeof(buffer), "%d %c %d\n", value1, operator, value2);
    printf("        User Sending the following: %d %c % d on FD %d\n", value1, operator, value2, reqfd);
    write(reqfd, buffer, buffer_length);
    
    // Read in the result from calculator and print result
    int result;
    read(rplyfd, &result, sizeof(result));
    printf("User Received the following result: %d from FD %d\n", result, rplyfd);

    return 0;
}