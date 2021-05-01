#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 16

typedef struct memoryList {
    unsigned int size;
    unsigned int startPosition;
    unsigned int usedCheck;
    unsigned int splitCheck;
    unsigned int numBlock;
    struct memoryList* next;
    struct memoryList* prev;
} memoryList;

void printMemory(memoryList *this);
void buddyAlloc(unsigned int numBlock, memoryList *this);
void buddyFree(unsigned int startPosition, memoryList *root);
unsigned int getSizeRequest(unsigned int numBlock);
void checkPair(memoryList *this);
memoryList* freeMemory(memoryList* this, memoryList* pair);
memoryList* findPair(memoryList *this);
void printValid(memoryList *this);
void execSplit(memoryList *this);
void testPrint(memoryList *this);
int findSplitSpot(memoryList *this, unsigned int sizeRequested);
int allocFree(memoryList* this, unsigned int sizeRequested, unsigned int numBlock);

int main() {
    memoryList root = {
        MAX, 0, 0, 0, 0, NULL, NULL
    };
    memoryList* a = &root;
    buddyAlloc(7, &root);
    buddyAlloc(1,&root);
    buddyAlloc(4,&root);
    buddyAlloc(2,&root);
    testPrint(&root);
    printMemory(&root);
    // printf("\nFree Called\n");
    // buddyFree(14,&root);
    // printMemory(a);
    // testPrint(a);
    // buddyFree(12,a);
    return 0;
}

void testPrint(memoryList *this) {
    while(1) {
        printf("%p = %d,%d,%d,%d,%d,%p,%p\n", this, this->size, this -> startPosition, this->usedCheck, this->splitCheck, this->numBlock, this->next, this->prev);
        if (this -> next == NULL) {
            return;
        }
        this = this -> next;
    }
}

//Allocate Memory
void buddyAlloc(unsigned int numBlock, memoryList *this) {
    memoryList *root = this; //for findSplit();
    unsigned int sizeRequested = getSizeRequest(numBlock);
    if (numBlock == 0) {
        printf("input valid number\n");
        return;
    }
    if (numBlock > MAX) {
        printf("input valid number\n");
        return;
    }
    while(1){
        printf("sizeRequested = %d\n",sizeRequested);
        printf("Checking from %p\n", this);
        int errCheckallocFree = allocFree(root, sizeRequested, numBlock);
        if (errCheckallocFree == 0){
            return;
        }
        else if (errCheckallocFree == 1) {
            printf("findSplitSpot called\n");
            int errCheckfindSplitSpot = findSplitSpot(root, sizeRequested);
            if (errCheckfindSplitSpot == 1) {
                printf("No space in memory\n");
                return;
            }
        }
    }
}

int allocFree(memoryList* this, unsigned int sizeRequested, unsigned int numBlock) {
    while(1) {
        if (this -> usedCheck == 0 && this -> size == sizeRequested && this -> splitCheck == 0) {
            this -> usedCheck = 1;
            this -> numBlock = numBlock;
            printf("sucessfully allocated %d blocks!\n", this -> numBlock);
            return 0;
        }
        else if (this -> next == NULL) {
            return 1;
        }
        this = this -> next;
    }
}

int findSplitSpot(memoryList *root, unsigned int sizeRequested) {
    for(int i = sizeRequested;i <= MAX;i*=2) {
        memoryList* this = root;
        while(1) {
            unsigned int upperSize = i * 2;
            if (upperSize == MAX * 2) return 1;
            printf("upperSize = %d\n", upperSize);
            printf("upperSize == %d from %p\n", this -> size, this);
            if (this -> size == upperSize && this -> splitCheck == 0 && this -> usedCheck == 0) {
                execSplit(this);
                this -> splitCheck = 1;
                printf("Executed Split!\n");
                return 0;
            }
            else if (this -> next != NULL) this = this -> next;
            else break;
        }
    }
    return 0;
}

void execSplit(memoryList *this) {
    unsigned int newSize = (this -> size)/2;
    memoryList* tempNext = this -> next;
    memoryList* split1;
    memoryList* split2;
    split1 = (memoryList*) malloc(sizeof(memoryList));
    split2 = (memoryList*) malloc(sizeof(memoryList));
    split1 -> size = newSize;
    split1 -> startPosition = this -> startPosition;
    split1 -> usedCheck = 0;
    split1 -> splitCheck = 0;
    split1 -> numBlock = 0;
    split1 -> next = split2;
    split1 -> prev = this;
    split2 -> size = newSize;
    split2 -> startPosition = (this -> startPosition) +  newSize;
    split2 -> usedCheck = 0;
    split2 -> splitCheck = 0;
    split2 -> numBlock = 0;
    split2 -> next = tempNext;
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

//Free Memory
void buddyFree (unsigned int startPosition, memoryList *root) {
    memoryList* this = root;
    while(1) {
        if (root -> usedCheck == 1) {
            this -> usedCheck = 0;
            this -> numBlock = 0;
        }
        else if (this -> startPosition == startPosition && this -> usedCheck == 1) {
            checkPair(this);
            return;
        }
        else if (this -> next == NULL) {
            printf("input a valid startPosition\n");
            return;
        }
        else {
            this = this -> next;
        }
    }
    
}

void checkPair (memoryList *this) {
    memoryList *pair = findPair(this);
    if (pair -> usedCheck == 1 || pair -> splitCheck == 1) {
        this -> usedCheck = 0;
        this -> numBlock = 0;
    }
    else if (pair -> usedCheck == 0) {
        memoryList* merged = freeMemory(this, pair);
        checkPair(merged);
    }
}

memoryList* findPair(memoryList *this) {
    memoryList *prev = this -> prev;
    memoryList *next = this -> next;
    if (prev -> size == this -> size) {
        return prev;
    }
    else if (next -> size == this -> size) {
        return next;
    }
    return NULL;
}

memoryList* freeMemory (memoryList* this, memoryList* pair) {
    memoryList* merged;
    memoryList* mergedNext;
    if (this -> next == pair) {
        merged = this -> prev;
        mergedNext = pair -> next;
    }
    else {
        merged = pair -> prev;
        mergedNext = this -> next;
    }
    merged -> next = mergedNext;
    mergedNext -> prev = merged;
    merged -> splitCheck = 0;
    free(this);
    free(pair);
    return merged;
}

//Print Memory
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