/* mymemory.c
 * 
 * provides interface to memory management
 * 
 */
#include "memory.h"


// our memory
// Changed variable name to kMAXMEM because I had a weird error
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
	
	// First segment is memory descriptor
        segmenttable->next = SegmentDescriptor;

}	

// INITIALIZE SEGMENTS ( ALLOCATE THEM IN HEAP)
// Since we are working with linked list , I have to use malloc to link segments
// Dinamycally create segments and link them with the previous one
Segment_t * createInstance(size_t size){
  // keep track of head of list
  Segment_t * head = segmenttable;
  // initialize segment
  Segment_t * SegmentDescriptor = NULL;
  SegmentDescriptor = (struct segmentdescriptor *)malloc(sizeof(struct segmentdescriptor));
  // size is what is passed in parameter
  SegmentDescriptor->size = size;
  // increase number of instances created
  InstanceCount += 1;
  // link with the previous segment
  for (int i = 0; i < InstanceCount - 1; i++){
   // go to the last node
    segmenttable = segmenttable->next;
  }
  
  // next node of our last is our new node
  segmenttable->next = SegmentDescriptor;
  //return to head
  segmenttable = head;
  return SegmentDescriptor;

}

void * mymalloc ( size_t size )
{

   Segment_t* SegmDescriptor = segmenttable->next;
   Segment_t* AllocateSegm = findFree(segmenttable, size);
   if (AllocateSegm != NULL){
     //Allocate the segment at the point where memory is free by getting offset of 1024 - sum of our allocated  sizes
     // Formula -> Index = MAXMEM - (MAXMEM - SUM(ALL SEGMENT SIZES) without the segment descriptor of course) 
     // Here our SegmDescriptor contains all our current memory allocated so we can just kmaxmem from it
     int Offset = kMAXMEM - SegmDescriptor->size;
     AllocateSegm->start = &mymemory[Offset];
     // alocated memory slots
     // I just put 1 instead of 0 on the allocated memory slots in the table so we know they are allocated
     // of course they will be overiden by their actual value
     for(int i = 0; i < size; i++){

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
	// Add the freed memory back to our available memory
	SegmDescriptor->size += Seg->size;
	printf("You have free'd %ld bytes of memory \n", Seg->size);
	// Free the segment
	Seg->allocated = FALSE;

   }

   else{
     printf("Segment not found");
    }

}

// In order to avoid multiple looping and pointer arithmetic
// I simply delete the free segments, keeping track of their size
// And I create a bigger segment at the end using CreateInstance
// Containing the size of all free segments
void mydefrag ()
{
   // Total size to allocate for the last segment created after defrag
   size_t total_size = 0;
   // keep track of head of list
   Segment_t* head = segmenttable;
   // Move once, to avoid deleting the memory descriptor 
   segmenttable = segmenttable->next;
   for (int i = 0; i < InstanceCount - 1; i++){
	//keep track of previous segment
	Segment_t* prev = segmenttable;
	segmenttable = segmenttable->next;
	// A free segment is one with allocated -> False
	if (segmenttable->allocated == FALSE){
	total_size += segmenttable->size;
	// link previous segment with the one after the free one
         prev->next = segmenttable->next;
	rearrangeStartPtrs(segmenttable->next, segmenttable->start);
	segmenttable->start = NULL;
	// resume search
	 segmenttable = prev;
	 InstanceCount -= 1;
	
	}
	
   }
   segmenttable = head;
   
   
   // new segment at the end , of size of all free memory segments
   Segment_t* new_seg = createInstance(total_size);
    printf("Defragmentation Succeeded -> your new segment is at the end of the list \n");
   // NEW FREE SEGMENT
   new_seg->allocated = FALSE;
}

int reprinteMemoryTable(Segment_t* CurrSegm, current_index){
	if(Seg == NULL){
	return -1;
	}
	else{
		for(int i = 0; i < CurrSegm->size;i++){
			mymemory[current_index + i] = 
		}
	}

}

// Rearange start pointers from the defragmented segment
int rearrangeStartPtrs(Segment_t* Seg,void * start){
	if(Seg == NULL){
	return -1;
	}

	else{

	// swap pointers, by keeping track of the current pointer
	// basically temp = a 
	// a = b
	// b = temp
	void * currentStart = Seg->start;
	Seg->start = start;
	rearrangeStartPtrs(Seg->next,currentStart);
	}
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
  size_t size = 0;
  for (int i = 0; i < InstanceCount - 1; i++){
	head = head->next;
	int index = 1024 - (1024 - size);
	size += head->size;
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

// print memory as a table
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

// recursive function to print the segments
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


