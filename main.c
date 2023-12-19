#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "conway.h"

void clear_stdin_buffer() {
    int ch;
    do {
        ch = getchar();
    } while (ch != '\n' && ch != EOF);
}

void* listen_break(void* flag) {
    char c = 0;
    while (c != 'b') {
        scanf(" %c", &c);
        scanf("%*[^\n]");
    }
    *(int*)flag = 1;
    return NULL;
}

void automatic_evolve(Conway* c) {
    int flag = 0;
    pthread_t listener;
    pthread_create(&listener, NULL, listen_break, &flag);
    while (flag != 1) {
        evolveConway(&c);
        printConway(c);
        printf("automatically evolving...\n");
        sleep(1);
    }
    pthread_join(listener, NULL);
    clear_stdin_buffer();
    printConway(c);
}

int main() {
    srand((unsigned int)time(NULL));
    Conway* c = NULL;
    char running = 1;
    char input[100];
    printHelp();
    while (running) {  
        printf("Enter command: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input.\n");
            continue;
        }
        char cmd;
        int row, column;
        char path[40];
        sscanf(input, " %c", &cmd);
        switch (cmd) {
            case 'h':
                printHelp();
                break;
            case 'r':
                if (c != NULL) {
                    initConway(c);
                    printConway(c);
                } else {
                    printf("Conway game not initialized. Please enter 'i <row> <col>' first.\n");
                }
                break;
            case 'n':
                if (c != NULL) {
                    automatic_evolve(c);
                } else {
                    printf("Conway game not initialized. Please start with 'i <row> <col>'.\n");
                }
                break;
            case 'i':
                if (sscanf(input, "i %d %d", &row, &column) == 2) {
                    c = createConway(row, column);
                    printConway(c);
                } else {
                    printf("Fail to create the Conway. Please check your input.\n");
                }
                break;
            case 's':
                if (c != NULL && sscanf(input, "s %39s", path) == 1) {
                    saveConway(c, path);
                    printConway(c);
                    printHelp();
                } else {
                    printf("Fail to find path or Conway game not initialized.\n");
                }
                clearscreen();
                printConway(c);
                break;
            case 'l':
                if (sscanf(input, "l %39s", path) == 1) {
                    loadConway(c, path);
                    printConway(c);
                    printHelp();
                } else {
                    printf("Fail to find path. Please check your input.\n");
                }
                break;
            case 'q':
                printf("Thanks for playing. Good bye!\n");
                running = 0;
                break;
            default:
                printf("Unknown command. Please try again.\n");
                break;
        }
    }
    freeConway(c);
    if (c != NULL) {
        printf("The conway is not freed.");
        exit(1);
    }
    return 0;
}
