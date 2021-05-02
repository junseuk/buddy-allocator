# buddy-allocator
## Description

This is a demonstration of simple buddy allocation.   
Linked list is used to keep track of chuncks and blocks.   
A struct called `memoryList` contains:

 1. size of chunck
 2. startPosition
 3. usedCheck (0 for unused, 1 for used)
 4. splitCheck (0 for not split, 1 for split block)
 5. numBlock (if used, how many blocks)
 6. a pointer to next memoryList
 7. a pointer to previous memoryList

Root, which is the first element of the linked list, is declared in main() :

    memoryList root = {MAX, 0, 0, 0, 0, NULL, NULL};
        
And as allocate and free blocks, functions will search the right spaces (elements) through a loop through linked list
and do allocate/split/free.

Descriptions for each function is written in the code as comments.

## Command

There are 4 commands that a user can type:

 1. a for allocation
 2. f for free
 3. q for quit
 4. l for showing all linked-lists
 
For instance, in 16 size of memory (`#define MAX 16`)
the result of `l` is shown below after allocating 2 blocks and 2 blocks again:

```
How many blocks do you want to allocate(a)/free(f) or quit(q)/show linked-list(l)?l

In order of
Address = size of chunck, usedChecked, splitChecked, numBlock, next link, prev link

0x7ffee04867c0 = 16,0,0,1,0,0x7feff1604080,0x0
0x7feff1604080 = 8,0,0,1,0,0x7feff16040e0,0x7ffee04867c0
0x7feff16040e0 = 4,0,0,1,0,0x7feff1604140,0x7feff1604080
0x7feff1604140 = 2,0,1,0,2,0x7feff1604170,0x7feff16040e0
0x7feff1604170 = 2,2,1,0,2,0x7feff1604110,0x7feff1604140
0x7feff1604110 = 4,4,0,0,0,0x7feff16040b0,0x7feff1604170
0x7feff16040b0 = 8,8,0,0,0,0x0,0x7feff1604110

|##|##|----|--------|
```
