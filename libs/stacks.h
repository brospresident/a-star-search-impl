#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct stack {
    int capacity;
    int occupied;
    Pair* data;
} Stack;

Stack* createStack(int capacity) {
    Stack* newStack = (Stack*)malloc(sizeof(Stack) * capacity);

    if (!newStack) {
        printf("memory bug!");
        return NULL;
    }

    newStack->capacity = capacity;
    newStack->data = (Pair*)malloc(sizeof(Pair) * newStack->capacity);
    newStack->occupied = -1;

    return newStack;
}

bool isFull(Stack* stack) {
    return stack->capacity - 1 == stack->occupied;
}

bool isEmpty(Stack* stack) {
    return stack->occupied == -1;
}

Pair top(Stack* stack) {
    return stack->data[stack->occupied - 1];
}

void push(Stack* stack, Pair data) {
    if (isFull(stack)) {
        printf("StackOverflow");
        return;
    }

    stack->data[++stack->occupied] = data;
}

Pair pop(Stack* stack) {
    if (isEmpty(stack)) {
        printf("StackUnderflow");
        exit(1);
    }

    Pair deleted = top(stack);
    stack->occupied--;

    return deleted;
}
