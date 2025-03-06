#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char value;
    int count;
    struct Node *next;
};


void insertOrUpdate(struct Node **head, char ch) {
    struct Node *current = *head;
    struct Node *prev = NULL;


    while (current != NULL) {
        if (current->value == ch) {
            current->count++; 
            return;
        }
        prev = current;
        current = current->next;
    }

    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->value = ch;
    newNode->count = 1;
    newNode->next = NULL;

    if (prev == NULL) {
        *head = newNode;
    } else {
        prev->next = newNode;
    }
}

void printList(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        printf("%c : %d\n", current->value, current->count);
        current = current->next;
    }
}

void freeList(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        struct Node *temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    FILE *file = fopen(__FILE__, "r");
    if (file == NULL) return 1;

    struct Node *head = NULL;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        insertOrUpdate(&head, ch);
    }
    fclose(file);
    printList(head);
    freeList(head);

    return 0;
}
