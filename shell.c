// Shell.C
// Provides task submission for CGS A5-A1

// To compile , link with memory.c
// gcc shell.c memory.c
#include "memory.h"

int main(){
printf ("shell> start\n");

initialize();
Segment_t* memoryDescriptor = segmenttable->next;

// Last element of array is marked by '\0' 

// First segment allocated
char* ptr1 = mymalloc(sizeof(char)*20);

memoryDescriptor->start = &ptr1;
printf("Memory left after first allocation: %lu\n",memoryDescriptor->size);

strcpy(ptr1, "I am groot\n");

printf("Content of ptr1: %s\n",ptr1);

char* ptr2 = mymalloc(sizeof(char)*20);
strcpy	(ptr2, "My name is Shrek \n");

printf("Content of ptr2: %s\n",ptr2);

printf("Memory left after second allocation: %lu\n",memoryDescriptor->size);
char* ptr3 = mymalloc(sizeof(char)*126);

strcpy(ptr3, "A monkey will eventually write Romeo & Juliet given a typewriter and enough time by pressing random buttons on the typewriter");

printf("Content of ptr3: %s\n",ptr3);

printf("Memory left after third allocation: %lu\n",memoryDescriptor->size);


myfree(ptr1);
myfree(ptr2);
void *ptrlist[kMAXSEGMENTS] = {ptr1,ptr2,ptr3};

// mydefrag
// 2nd parameter is size of ptrlist
mydefrag(ptrlist,3);

char* ptr4 = mymalloc(sizeof(char)*40);
strcpy(ptr4, "My name is Shrek \n");


char* ptr5 = mymalloc(sizeof(char)*40);
strcpy(ptr5, "Puss in the boots \n");


printf("Total memoroy available: %ld bytes\n", memoryDescriptor->size);
// Keep track of segments
// Here I just replace the memory descriptor's size variable which previously contained our available memory which we dont use anymore
// With a static variable that increments every time InstanceCount() is called
memoryDescriptor->size = InstanceCount -1;

// Segment Table
printsegmenttable(segmenttable->next, 0);

//Uncomment to print the memory as a table
DumpHex(mymemory,1024);

printf ( "shell> end\n");
return 0;
}
