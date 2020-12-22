#include <stdio.h>
#include <stdlib.h>
#include "string.h"

int currentState = 1;
const int endState = 3;

int case1(char x) {
    if (x == '$') {
        currentState = 2;
        return 1;
    }
    if (x == '0') {
        currentState = 4;
        return 1;
    }
    return 0;
}

int case2(char x) {
    if (x >= '0' && x <= '9') {
        currentState = 3;
        return 1;
    }
    if ((x >= 'a' && x <= 'f') || (x >= 'A' && x <= 'F')) {
        currentState = 3;
        return 1;
    }
    return 0;
}

int case3(char x) {
    if (x >= '0' && x <= '9') {
        currentState = 3;
        return 1;
    }
    if ((x >= 'a' && x <= 'f') || (x >= 'A' && x <= 'F')) {
        currentState = 3;
        return 1;
    }
    return 0;
}

int case4(char x) {
    if (x == 'x') {
        currentState = 5;
        return 1;
    }
    return 0;
}

int case5(char x) {
    if (x >= '0' && x <= '9') {
        currentState = 3;
        return 1;
    }
    if ((x >= 'a' && x <= 'f') || (x >= 'A' && x <= 'F')) {
        currentState = 3;
        return 1;
    }
    return 0;
}

int choice(char x) {
    switch (currentState) {
        case 1:
            return case1(x);
        case 2:
            return case2(x);
        case 3:
            return case3(x);
        case 4:
            return case4(x);
        case 5:
            return case5(x);
        default:
            return 0;
    }
}

int main() {
    char* string = malloc(sizeof(char) * 20);
    scanf("%s", string);

    for (int i = 0; i < strlen(string); i++) {
        char *test = malloc(sizeof(char) * 20);
        int length = 0;
        currentState = 1;
        for (int j = i; j < strlen(string); j++) {
            if (choice(string[j]) != 0) {
                test[length] = string[j];
                length++;
            }
            else break;
            if (currentState == endState)
                printf("%s is a correct string\n", test);
        }
    }

    return 0;
}
