#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char c;
    struct Node* n;
} Node;

Node* buildList(const char* s) {
    Node *h = NULL, *t = NULL;
    for (size_t i = 0; i < strlen(s); i++) {
        Node* p = (Node*)malloc(sizeof(Node));
        if (!p) {
            printf("Memory error\n");
            exit(1);
        }
        p->c = s[i];
        p->n = NULL;
        if (!h) h = t = p;
        else t = t->n = p;
    }
    return h;
}

void printList(Node* h) {
    while (h) {
        putchar(h->c);
        h = h->n;
    }
}

void freeList(Node* h) {
    Node* t;
    while (h) {
        t = h;
        h = h->n;
        free(t);
    }
}

int main() {
    FILE* f = fopen(__FILE__, "r");
    if (!f) {
        printf("File error\n");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    rewind(f);

    char* buf = (char*)malloc(sz + 1);
    if (!buf) {
        printf("Memory error\n");
        fclose(f);
        return 1;
    }
    fread(buf, 1, sz, f);
    buf[sz] = '\0';
    fclose(f);

    Node* h = buildList(buf);
    free(buf);

    printList(h);
    freeList(h);

    return 0;
}
