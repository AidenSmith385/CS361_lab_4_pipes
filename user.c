#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // read and write ends of pipe
    int reqfd = atoi(argv[1]);
    int rplyfd = atoi(argv[2]);

    printf("This is the User process (id = %d).\n", getpid());
    printf("    User: Please, enter:   value1   operation   value2\n");

    // variables to hold the expression
    int value1, value2;
    char operator;

    // get the input expression
    scanf("%d %c %d", &value1, &operator, &value2);
    printf("User Sending the following: %d %c % d on FD %d\n", value1, operator, value2, reqfd);

    // feed the buffer the expression and write to the pipe
    char buffer[20];
    int buffer_length = snprintf(buffer, sizeof(buffer), "%d %c %d\n", value1, operator, value2);
    write(reqfd, buffer, buffer_length);
    
    // read in the result from calculator
    int result;
    read(rplyfd, &result, sizeof(result));
    printf("User Received the following result: %d from FD %d\n", result, rplyfd);

    return 0;
}