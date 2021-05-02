#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// this defines the size of the memory
#define MAX 64

typedef struct memoryList {
    unsigned int size;
    unsigned int startPosition;
    unsigned int usedCheck;
    unsigned int splitCheck;
    unsigned int numBlock;
    struct memoryList* next;
    struct memoryList* prev;
} memoryList;

/* 
functions for printing the whole memory and the linked list.
printBlock() is going to check whether it should be printed or not: 
a chunck which is split into small chuncks should not be printed.
*/
void printMemory(memoryList *this);
void printBlock(memoryList *this);
void printList(memoryList *this);

/*
functions for allocation.
It loops through the linked list and find a right place to split if needed.
If not found, it returns an error message.
*/
void buddyAlloc(unsigned int numBlock, memoryList *this);
unsigned int getSizeRequest(unsigned int numBlock);
int findSplitSpot(memoryList *this, unsigned int sizeRequested);
void execSplit(memoryList *this);

/*
functions for freeing blocks.
It has a similar logic as one used in allocation. But to free blocks, it is necessary to find the right pair.
And check whether its pair is also empty. If so, a recursive call of checkPair() occurs.
*/
int allocFree(memoryList* this, unsigned int sizeRequested, unsigned int numBlock);
void buddyFree(unsigned int startPosition, memoryList *root);
void checkPair(memoryList *this);
memoryList* freeMemory(memoryList* this, memoryList* pair);
memoryList* findPair(memoryList *this);

int main() {
    memoryList root = {MAX, 0, 0, 0, 0, NULL, NULL};
    while(1) {
        char buffer[10];
        char command = 0;
        unsigned int input = 0;
        printMemory(&root);
        printf("\nHow many blocks do you want to allocate(a)/free(f) or quit(q)/show linked-list(l)?");
        fgets(buffer,sizeof(buffer),stdin);
        int num_toks = sscanf(buffer, "%c %u", &command, &input);
        switch (num_toks)
        {
        case 2:
            switch (command)
            {
            case 'a': case 'A':
                buddyAlloc(input, &root);
                break;
            case 'f': case 'F':
                buddyFree(input, &root);
                break;
            default:
                printf("Input is not valid\n");
                break;
            }
            break;
        case 1:
            switch (command)
            {
            case 'q': case 'Q':
                return 0;
            case 'l': case 'L':
                printList(&root);
                break;
            default:
                printf("Input is not valid\n");
                break;
            }
            break;
        default:
            printf("Input is not valid\n");
            break;
        }
    }
    return 0;
}

/*
Allocate Memory:
buddyAlloc() takes arguments of numBlock and the pointer to root. It passes the address of root to findSplit().
*/
void buddyAlloc(unsigned int numBlock, memoryList *this) {
    memoryList *root = this; //for findSplit();
    unsigned int sizeRequested = getSizeRequest(numBlock);
    if (numBlock == 0) {
        printf("Input valid number for allocation\n");
        return;
    }
    if (numBlock > MAX) {
        printf("Input valid number: too big\n");
        return;
    }
    while(1){
        int errCheckallocFree = allocFree(root, sizeRequested, numBlock);
        //if succeeded to allocate, errCheckallocFree = 0
        if (errCheckallocFree == 0){
            return;
        }
        //if failed, errCheckallocFree = 1. It means no available space in the memory.
        else if (errCheckallocFree == 1) {
            int errCheckfindSplitSpot = findSplitSpot(root, sizeRequested);
            //if failed to find a split spot 
            if (errCheckfindSplitSpot == 1) {
                printf("No space available in memory\n");
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
            unsigned int upperSize = i * 2; //size of split spot
            if (upperSize == MAX * 2) return 1; //No split spot in the memory
            if (this -> size == upperSize && this -> splitCheck == 0 && this -> usedCheck == 0) {
                execSplit(this);
                this -> splitCheck = 1;
                return 0;
            }
            else if (this -> next != NULL) this = this -> next;
            else break;
        }
    }
    return 1;
}

void execSplit(memoryList *this) {
    unsigned int splitSize = (this -> size)/2;
    memoryList* nextOfsplit2 = this -> next;
    memoryList* split1;
    memoryList* split2;
    split1 = (memoryList*) malloc(sizeof(memoryList));
    split2 = (memoryList*) malloc(sizeof(memoryList));
    split1 -> size = splitSize;
    split1 -> startPosition = this -> startPosition;
    split1 -> usedCheck = 0;
    split1 -> splitCheck = 0;
    split1 -> numBlock = 0;
    split1 -> next = split2;
    split1 -> prev = this;
    split2 -> size = splitSize;
    split2 -> startPosition = (this -> startPosition) +  splitSize;
    split2 -> usedCheck = 0;
    split2 -> splitCheck = 0;
    split2 -> numBlock = 0;
    split2 -> next = nextOfsplit2;
    split2 -> prev = split1;
    this -> next = split1;
    if (nextOfsplit2 != NULL) nextOfsplit2 -> prev = split2; //when split occurs in the middle of others.
}

//Find the nearest and smallest power of 2
unsigned int getSizeRequest(unsigned int numBlock) {
    unsigned int compare = 1;
    while (compare < numBlock) {
        compare <<= 1;
    }
    return compare;
}

/*
Free Memory:
buddyFree() takes startPosition and the address of root for the loop.
Before freeing blocks, it has to find the right pair and if the pair, another one, exists after or before.
*/
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
            printf("Input a valid startPosition\n");
            return;
        }
        else {
            this = this -> next;
        }
    }
    
}

void checkPair (memoryList *this) {
    memoryList *pair = findPair(this);
    //if no pair found
    if (pair == NULL) {
        this -> usedCheck = 0;
        this -> numBlock = 0;
        return;
    }
    //if another one is used or split
    else if (pair -> usedCheck == 1 || pair -> splitCheck == 1) {
        this -> usedCheck = 0;
        this -> numBlock = 0;
        return;
    }
    //if the pair is both free, merge. Recursion to check if further merging is needed.
    else if (pair -> usedCheck == 0) {
        memoryList* merged = freeMemory(this, pair);
        checkPair(merged);
    }
}

//Find a pair using size
memoryList* findPair(memoryList *this) {
    memoryList *prev = this -> prev;
    memoryList *next = this -> next;
    if (prev != NULL && prev -> size == this -> size) {
        return prev;
    }
    else if (next != NULL && next -> size == this -> size) {
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
        if(mergedNext != NULL) mergedNext -> prev = merged;
    }
    else {
        merged = pair -> prev;
        mergedNext = this -> next;
        if(mergedNext != NULL) mergedNext -> prev = merged;
    }
    merged -> next = mergedNext;
    merged -> splitCheck = 0;
    free(this);
    free(pair);
    return merged;
}

//Print Memory
void printMemory(memoryList *this) {
    printf("|");
    while(1) {
        //End of the linked list
        if (this -> next == NULL) {
            printBlock(this);
            return;
        }
        //Used ones
        else if (this -> usedCheck == 1 && this -> splitCheck == 0) {
            printBlock(this);
        }
        //Empty ones
        else if (this -> usedCheck == 0 && this -> splitCheck == 0) {
            printBlock(this);
        }
        this = this -> next;
    }
}

void printBlock(memoryList *this) {
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

//Print the whole linked list with description.
void printList(memoryList *this) {
    printf("\n");
    printf("In order of\n");
    printf("Address = size of chunck, usedChecked, splitChecked, numBlock, next link, prev link\n");
    printf("\n");
    while(1) {
        printf("%p = %d,%d,%d,%d,%d,%p,%p\n", this, this->size, this -> startPosition, this->usedCheck, this->splitCheck, this->numBlock, this->next, this->prev);
        if (this -> next == NULL) {
            printf("\n");
            return;
        }
        this = this -> next;
    }
}