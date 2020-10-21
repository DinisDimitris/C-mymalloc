/* mymemory.c
 * 
 * provides interface to memory management
 * 
 */
#include "memory.h"


// our memory
Byte mymemory [kMAXMEM] ;
Segment_t* segmenttable = NULL;
int InstanceCount = 0;


void initialize ()
{
   printf ( "initialize> start\n");
   // set memory to 0
   for (int i =0; i < kMAXMEM + 1; i++){
	mymemory[i] = '\0';
   }
	segmenttable = (struct segmentdescriptor *)malloc(sizeof(struct segmentdescriptor));
   	// create segment descriptor for keeping track of segments
	// I use kMAXMEM to initiate it here because I need to keep track of how much memory we have left
	// In shell i switch this with InstanceCount so it prints the number of Segments
        Segment_t * SegmentDescriptor = createInstance(kMAXMEM);
	// Segment Descriptor that descirbes the whole memory
	SegmentDescriptor->allocated = FALSE;
	
        segmenttable->next = SegmentDescriptor;

}	

// INITIALIZE SEGMENTS ( ALLOCATE THEM IN HEAP)
// Since we are working with linked list , I have to use malloc to link segments
// Dinamycally create segments and link them with the previous one
Segment_t * createInstance(size_t size){
  // keep track of head of list
  Segment_t * head = segmenttable;
  Segment_t * SegmentDescriptor = NULL;
  SegmentDescriptor = (struct segmentdescriptor *)malloc(sizeof(struct segmentdescriptor));
  SegmentDescriptor->size = size;
  InstanceCount += 1;
  // link with the previous segment
  for (int i = 0; i < InstanceCount - 1; i++){
   // go to the last node
    segmenttable = segmenttable->next;
  }
  
  // next node of our last is our new node
  segmenttable->next = SegmentDescriptor;
  segmenttable = head;
  return SegmentDescriptor;

}

void * mymalloc ( size_t size )
{
   Segment_t* SegmDescriptor = segmenttable->next;
   Segment_t* AllocateSegm = findFree(segmenttable, size);
   if (AllocateSegm != NULL){
     //Allocate the segment at the point where memory is free by getting offset of 1024 - sum of our allocated  sizes
     // Since we know that MemoryDescrriptor only contains segments which are allocated,
     // We can get index of our first memory block by just substracting the sum of allocated sizes from 102
     int Offset = kMAXMEM - SegmDescriptor->size;
     AllocateSegm->start = &mymemory[Offset];
     // alocated memory slots
     // I just put 1 instead of 0 on the allocated memory slots in the table
     for(int i = 0; i < size; i++){
	// mark head of segment
	if ( i == 0){
	mymemory[Offset + i] = '\2';
	}

	// mark tail of segment
	if ( i == size - 1){
	mymemory[Offset + i] = '\3';
	}
	mymemory[Offset + i] = '\1';
    }
    // alocate the segment
     AllocateSegm->allocated = TRUE;
    // update our available memory
     SegmDescriptor->size = SegmDescriptor->size - size;
     return AllocateSegm->start;
   }   
   
}

void myfree ( void * ptr )
{
   Segment_t* SegmDescriptor = segmenttable->next;
   Segment_t * Seg = findSegment(segmenttable, ptr);
   if (Seg != NULL){
	// Free'd memory
	SegmDescriptor->size += Seg->size;
	printf("You have free'd %ld bytes of memory \n", Seg->size);
	// Free the segment
	Seg->start = NULL;
	Seg->allocated = FALSE;
	Seg->size = 0;

   }

   else{
     printf("Segment not found");
    }

}

void mydefrag ( void ** ptrlist)
{
   

}


// helper functions for management segmentation table
Segment_t * findFree ( Segment_t * list, size_t size )
{
 
   Segment_t * MemoryDescriptor = list->next;
   // Initially, see whether we have enough memory to allocate
   int MemoryCap =  MemoryDescriptor->size - size;
   if (MemoryCap > 0){
    // Create instance and return it
    // it will automatically link with the previous instance created
      Segment_t* SegmentDescriptor = createInstance(size);
      return SegmentDescriptor;
      }
   
    else{
    // All our memory is allocated, we need to deallocate
    MemoryDescriptor->allocated = TRUE;
    printf("No sufficient memory..> Call free memory");
    return NULL;
    }
   
   

}


Segment_t * findSegment ( Segment_t * list, void * ptr )
{
  // Pointer so we dont modify the value in the iterations
  Segment_t* head = list->next;
  for (int i = 0; i < InstanceCount - 1; i++){
	head = head->next;
	if ( head->start == ptr){
		// Found segment
		return head;
	}
	
  }
  // Segment not found
  return NULL;	
}

int isPrintable ( int c )
{
   if ( c >= 0x20 && c <= 0x7e ) return c ;

   return 0 ;
}

void printmemory (Byte mymemory []){
int next_row = 10;

 for(int i = 0; i < kMAXMEM + 1; i++){
	if (i == 0){
	printf("[%i] ", i);
	}
	if ( i == next_row){
	printf("\n");
	printf("[%i]", i);
	next_row += 10;
	}
	
        printf(" %02x ", mymemory[i]);
        printf("|");

 }
}

int printsegmenttable(Segment_t * segmentTable, int Index )
{
  if (segmentTable == NULL){
 	return -1 ;
  }
  else{
  	printf("Segment %i \n", Index);
	printsegmentdescriptor(segmentTable);
	return printsegmenttable(segmentTable->next, Index + 1);
  }
}

void printsegmentdescriptor (Segment_t * descriptor)
{
      printf ( "\tallocated = %s\n" , (descriptor->allocated == FALSE ? "FALSE" : "TRUE" ) );
      printf ( "\tstart     = %p\n" , descriptor->start );
      printf ( "\tsize      = %lu\n", descriptor->size  );
}


