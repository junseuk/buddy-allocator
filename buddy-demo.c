#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct freeList {
    unsigned int size;
    unsigned int startPosition;
    unsigned int endPosition;
    unsigned int usedCheck;
    unsigned int splitCheck;
    unsigned int numBlock;
    struct freeList* next;
    struct freeList* prev;
} freeList;

void printMemory(freeList *this);
void buddyAlloc(unsigned int numBlock, freeList *this);
void buddyFree(unsigned int size);
unsigned int checkSize(unsigned int numBlock);
void checkPair(freeList *this);
void printValid(freeList *this);
void splitOnce(unsigned sizeRequested, freeList *root)

int main() {
    freeList root = {
        16, 0, 16, 0, 0, 0, NULL, NULL
    };
    buddyAlloc(100, &root);
    /*char command;
    while(1){
        printMemory();
        scanf("%c",&command);
        if(command=='a'){
        }
        else if(command=='d'){
        }
        else if(command=='s'){
        }
        else if(command=='q'){
            break;
        }
        else {
        printf("Type valid input\n");
        getchar(); //clear buffer                                                                                          
        }
        
    }*/
    printMemory(&root);
    return 0;
}

void buddyAlloc(unsigned int numBlock, freeList *this) {
    freeList *root = this;
    if (numBlock == 0) {
        printf("input valid number\n");
        return;
    }
    unsigned int sizeRequested = checkSize(numBlock);
    while(1) {
        if (this -> next != NULL) {
            if (this -> usedCheck == 0 && this -> size == sizeRequested && this -> splitCheck == 0) {
                this -> usedCheck = 1;
                printf("\nsucessfully allocated!");
                break;
            }
            else {
                this = this -> next;
            }
        }
        else if(this -> next == NULL) {
            splitOnce(sizeRequested, root);
        }
        printf("\nPrinted all!");
    }
}

void splitOnce(unsigned int sizeRequested, freeList *root) {
    unsigned int upperSize = 2 * sizeRequested;
    freeList *this = root;
    freeList *next = root->next;
    freeList *prev = root->prev;
    while(1) {
        if (this -> size == upperSize && this -> splitCheck == 0) {
            createInstance(this, next, prev, sizeRequested);
            
        }
        else if(this -> size != upperSize) {
            this = this -> next;
        }
    }
}

void createInstance(freeList *this, freeList *next, freeList *prev, unsigned int sizeRequested) {
    
}

unsigned int checkSize(unsigned int numBlock) {
    unsigned int compare = 1;
    while (compare < numBlock) {
        compare <<= 1;
    }
    return compare;
}

void printMemory(freeList *this) {
    printf("|");
    while(1) {
        if (this -> next != NULL) {
            if (this -> usedCheck == 1 && this -> splitCheck == 0) {
                printValid(this);
                printf("|");
            }
            else if (this -> usedCheck == 0 && this -> splitCheck == 0) {
                printValid(this);
                printf("|");
            }
            else {
                this = this -> next;
            }
        }
        else if(this -> next == NULL) {
            printValid(this);
            printf("|");
            break;
        }
        printf("\nPrinted all!");
    }
}

void printValid(freeList *this) {
    int _size = this -> size;
    int _numBlocks = this -> numBlock;
    int diff = _size - _numBlocks;
    for (int i=0;i<_numBlocks;i++) {
        printf("#");
    }
    for (int i=0;i<diff;i++) {
        printf("-");
    }
}