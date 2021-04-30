#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct memoryList {
    unsigned int size;
    //unsigned int startPosition;
    //unsigned int endPosition;
    unsigned int usedCheck;
    unsigned int splitCheck;
    unsigned int numBlock;
    struct memoryList* next;
    struct memoryList* prev;
} memoryList;

void printMemory(memoryList *this);
void buddyAlloc(unsigned int numBlock, memoryList *this);
void buddyFree(unsigned int size);
unsigned int getSizeRequest(unsigned int numBlock);
void checkPair(memoryList *this);
void printValid(memoryList *this);
int findSplit(unsigned sizeRequested, memoryList *root);
void createInstance(memoryList *this, memoryList *next, memoryList *prev, unsigned int sizeRequeseted);
void testPrint(memoryList *this);

int main() {
    memoryList root = {
        16, 0, 0, 0, NULL, NULL
    };
    buddyAlloc(5, &root);
    buddyAlloc(5, &root);
    memoryList *test = &root;
    testPrint(test);
    test = test -> next;
    testPrint(test);
    test = test -> next;
    testPrint(test);
    printMemory(&root);
    return 0;
}

void testPrint(memoryList *this) {
    printf("%d,%d,%d,%d,%p,%p\n", this->size, this->usedCheck, this->splitCheck, this->numBlock, this->next, this->prev);
}

void buddyAlloc(unsigned int numBlock, memoryList *this) {
    memoryList *root = this; //for findSplit();
    unsigned int sizeRequested = getSizeRequest(numBlock);
    if (numBlock == 0) {
        printf("input valid number\n");
        return;
    }
    while(1){
        if (this -> usedCheck == 0 && this -> size == sizeRequested && this -> splitCheck == 0) {
            this -> usedCheck = 1;
            this -> numBlock = numBlock;
            printf("sucessfully allocated!\n");
            return;
        }
        else if(this -> next == NULL) {
            int errFindSplit = findSplit(sizeRequested, root);
            if(errFindSplit == 0) this = this -> next;
            else return;
        }
        else (this = this -> next);
    }
}

int findSplit(unsigned int sizeRequested, memoryList *root) {
    unsigned int upperSize = 2 * sizeRequested;
    memoryList *this = root;
    memoryList *next = root->next;
    memoryList *prev = root->prev;
    while(1) {
        if (this -> size == upperSize && this -> splitCheck == 0 && this -> usedCheck == 0) {
            createInstance(this, next, prev, sizeRequested);
            this -> splitCheck = 1;
            return 0;
        }
        this = this -> next;
        if (this -> next == NULL) {
            printf("\ncannot allocate");
            return 1;
        }
    }
    return 1;
}

void createInstance(memoryList *this, memoryList *next, memoryList *prev, unsigned int sizeRequested) {
    unsigned int newSize = (this -> size)/2;
    memoryList* split1;
    memoryList* split2;
    split1 = (memoryList*) malloc(sizeof(memoryList));
    split2 = (memoryList*) malloc(sizeof(memoryList));
    split1 -> size = newSize;
    split1 -> usedCheck = 0;
    split1 -> splitCheck = 0;
    split1 -> numBlock = 0;
    split1 -> next = split2;
    split1 -> prev = this;
    split2 -> size = newSize;
    split2 -> usedCheck = 0;
    split2 -> splitCheck = 0;
    split2 -> numBlock = 0;
    split2 -> next = next;
    split2 -> prev = split1;
    this -> next = split1;
}

unsigned int getSizeRequest(unsigned int numBlock) {
    unsigned int compare = 1;
    while (compare < numBlock) {
        compare <<= 1;
    }
    return compare;
}

void printMemory(memoryList *this) {
    printf("|");
    while(1) {
        if (this -> next == NULL) {
            printValid(this);
            return;
        }
        else if (this -> usedCheck == 1 && this -> splitCheck == 0) {
            printValid(this);
        }
        else if (this -> usedCheck == 0 && this -> splitCheck == 0) {
            printValid(this);
            printf("|");
        }
        this = this -> next;
    }
}

void printValid(memoryList *this) {
    int _size = this -> size;
    int _numBlocks = this -> numBlock;
    int diff = _size - _numBlocks;
    for (int i=0;i<_numBlocks;i++) {
        printf("#");
    }
    for (int i=0;i<diff;i++) {
        printf("-");
    }
    printf("|");
}