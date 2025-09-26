#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // read and write ends of pipe
    int reqfd = atoi(argv[1]);
    int rplyfd = atoi(argv[2]);

    printf("This is the Calculator process (id = %d).\n", getpid());
    printf("Calculator Waiting to receive from FD %d\n", reqfd);

    // read in the expression from user
    char buffer[20];
    if (read(reqfd, buffer, sizeof(buffer) - 1) < 0) {
        perror("failed to read from user");
        exit(1);
    }

    // assign the values and operator from the buffer
    int value1, value2;
    char operator;
    sscanf(buffer, "%d %c %d", &value1, &operator, &value2);

    printf("Calculator received the following: %d %c %d from FD %d\n", value1, operator, value2, reqfd);

    // compute the result depending on the operator
    int result;
    switch(operator) {
        case '+':
            result = value1 + value2;
            break;
        case '-':
            result = value1 - value2;
            break;
        case '*':
            result = value1 * value2;
            break;
        case '/':
            result = value1 / value2;
            break;
        default:
            printf("Invalid operation\n");
            break;
    }

    // write back to the user the result
    printf("Calculator sent the following to the User: %d on FD %d\n", result, rplyfd);
    write(rplyfd, &result, sizeof(result));

    return 0;
}