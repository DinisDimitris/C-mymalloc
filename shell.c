// Shell.C
// Provides task submission for CGS A5-A1

// To compile , link with memory.c
// gcc shell.c memory.c
#include "memory.h"

int main(){
printf ("shell> start\n");

initialize();
Segment_t* memoryDescriptor = segmenttable->next; 

// our ptrlist
void * ptrlist[kMAXSEGMENTS];


// I declare my references to malloc inside the ptrlist 
// Because in defrag, I have to use the actual pointer value, not the copy
// First segment allocated
ptrlist[0] = mymalloc(sizeof(char)*30);

// Link segmenttable to first segment allocated
memoryDescriptor->start = &ptrlist[0];

strcpy(ptrlist[0], "I am groot\n");

printf("Content of ptr1: %s\n",(char*)ptrlist[0]);

printf("Memory left after first allocation: %lu\n",memoryDescriptor->size);

ptrlist[1] = mymalloc(sizeof(char)*30);
strcpy	(ptrlist[1], "My name is Shrek \n");

printf("Content of ptr2: %s\n",(char*)ptrlist[1]);

printf("Memory left after second allocation: %lu\n",memoryDescriptor->size);
ptrlist[2] = mymalloc(sizeof(char)*126);

strcpy(ptrlist[2], "A monkey will eventually write Romeo & Juliet given a typewriter and enough time by pressing random buttons on the typewriter");

printf("Content of ptr3: %s\n",(char*)ptrlist[2]);

printf("Memory left after third allocation: %lu\n",memoryDescriptor->size);

// call free function
myfree(ptrlist[0]);
myfree(ptrlist[1]);
// also have to set ptr to null 
ptrlist[0] = NULL;
ptrlist[1]= NULL;

// mydefrag
// to reduce size of for loop, 2nd param in mydefraag is the number of pointers in ptrlist
mydefrag(ptrlist,3);

// After defrag, our ptrlist[2] became ptrlist[0]
// We start allocating again from ptrlist[1]
ptrlist[1] = mymalloc(sizeof(char)*50);
strcpy(ptrlist[1], "Adventure and fuss with Shrek & Puss in the boots");


printf("Total memoroy available: %ld bytes\n", memoryDescriptor->size);
// Keep track of segments
// Here I just replace the memory descriptor's size variable which previously contained our available memory which we dont use anymore
// With a static variable that increments every time InstanceCount() is called
memoryDescriptor->size = InstanceCount -1;

// Segment Table
printsegmenttable(segmenttable->next, 0);

//print the memory as a table
// see header comment in memory.c for source code
DumpHex(mymemory,1024);

printf ( "shell> end\n");
return 0;
}
