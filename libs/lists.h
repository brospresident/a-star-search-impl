#include <stdlib.h>
#include "structs.h"

typedef struct node {
    pPair data;
    struct node* next;
} Node;

void push(Node* prev, pPair newData) {
    Node* newNode = (Node*)malloc(sizeof(Node));

    newNode->data = newData;
    newNode->next = prev->next;
    prev->next = newNode;
}

void printList(Node* head) {
    if (head != NULL) {
        
    }
}