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
char* c = mymalloc(sizeof(char)*20);

printf("Memory left after first allocation: %lu\n",memoryDescriptor->size);

strcpy(c, "I am groot\n");

printf("Content of c: %s\n",c);

mymalloc(20);

printf("Memory left after second allocation: %lu\n",memoryDescriptor->size);
mymalloc(50);

// free function
myfree(c);

printf("Total memoroy available: %ld bytes\n", memoryDescriptor->size);

// Keep track of segments
// Here I just replace the memory descriptor's size variable which previously contained our available memory which we dont use anymore
// With a static variable that increments every time InstanceCount() is called
memoryDescriptor->size = InstanceCount;

// Segment Tables
printsegmenttable(segmenttable->next, 0);

//Uncomment to print the memory as a table
//printmemory(mymemory);





printf ( "shell> end\n");
return 0;
}
