// Shell.C
// Provides task submission for CGS A5-A1

// To compile , link with memory.c
// gcc shell.c memory.c
#include "memory.h"

int main(){
printf ("shell> start\n");

initialize();
Segment_t* memoryDescriptor = segmenttable->next;

// ALLOCATION CONSISTS OF (SIZE - 1) + 1 LAST CHAR FOR THE '/0' END OF SEGMENT

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
mymalloc(50);

printf("Memory left after third allocation: %lu\n",memoryDescriptor->size);
// free function
myfree(ptr1);
myfree(ptr2);

mymalloc(60);

mymalloc(30);
printf("Total memoroy available: %ld bytes\n", memoryDescriptor->size);
mydefrag();
// Keep track of segments
// Here I just replace the memory descriptor's size variable which previously contained our available memory which we dont use anymore
// With a static variable that increments every time InstanceCount() is called
memoryDescriptor->size = InstanceCount;

// Segment Table
printsegmenttable(segmenttable->next, 0);

//Uncomment to print the memory as a table
printmemory(mymemory);

printf ( "shell> end\n");
return 0;
}
