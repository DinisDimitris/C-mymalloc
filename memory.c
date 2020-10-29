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
// pointer to last segment 
// Since defrag will return a free segment at the end of the linked list, 
// we can keep a pointer to the last el to check if its free
Segment_t* last_el = NULL;

void initialize ()
{
   printf ( "initialize> start\n");
   // set memory to 0
   for (int i =0; i < kMAXMEM + 1; i++){
	mymemory[i] = '\0';
   }
	// i need to allocate these segments in heap which are head and tail of the linked list
	segmenttable = (struct segmentdescriptor *)malloc(sizeof(struct segmentdescriptor));
        last_el = (struct segmentdescriptor *)malloc(sizeof(struct segmentdescriptor));
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
// Dinamycally create a segment and link it with the previous one
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
  last_el = SegmentDescriptor;
  return SegmentDescriptor;

}

void * mymalloc ( size_t size )
{

   Segment_t* SegmDescriptor = segmenttable->next;
   Segment_t* AllocateSegm = findFree(segmenttable, size);
   // Allocate a new segment at the end of list
   if (AllocateSegm != NULL && AllocateSegm->start == NULL){
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
   // Allocate to a free segment
   else{
	
 	int Offset = getIndex(AllocateSegm->start);
	for(int i = 0; i < size; i++){

	mymemory[Offset + i] = '\1';
    }
    // alocate the segment
     AllocateSegm->allocated = TRUE;
     return AllocateSegm->start;
   }   
   
}

void myfree ( void * ptr )
{
   Segment_t* SegmDescriptor = segmenttable->next;
   Segment_t * Seg = findSegment(segmenttable, ptr);
   if (Seg != NULL){
	printf("Free'd the segment %p \n", Seg->start);
	// Free the segment
	Seg->allocated = FALSE;
	int index = getIndex(Seg->start);
	for (int i =0; i < Seg->size; i ++){
		mymemory[index + i] = '\0';
	}

   }

   else{
     printf("Segment not found");
    }

}

int getIndex(void * ptr){
	for (int i = 0 ; i < kMAXMEM; i++){
		if ( ptr == &mymemory[i]){
			return i;
		}
	}
}

// F F = 2F
// F A = First iter -> A F; Second iter -> A F;
// F F A = First Iter -> 2F A ; Second Iter -> A 2F ; Third Iter -> A 2F
// F A F F A = First iter -> A F F F A -> Second Iter -> A 2F F A; Third Iter -> A 3F A; Fourth Iter -> A A 3F ; Fifth iter -> A A 3F
void mydefrag(void ** ptrlist, int count){
	int checkStart = 1;
	int loopcnt = 0;
	Segment_t* head = segmenttable;
	Segment_t* SegmDes = segmenttable->next;
	Segment_t* segment = SegmDes->next;
	for (int i =0; i < count ; i++){
		if(segment->next != NULL){
		
		Segment_t* nextSeg = segment->next;
		// First case , combine both segments into a bigger one
		// here we dont need to change any pointer value, since we are just creating a bigger segment from 2 smaller ones
		if( segment->allocated == FALSE && nextSeg->allocated == FALSE){
			Segment_t* nextNextSeg = nextSeg->next;
			segment->next = nextNextSeg;
			segment->size += nextSeg->size;
			for (int ptrCount = 0; ptrCount > count - i; ptrCount++){
				ptrlist[i + ptrCount + 1] = ptrlist[i + ptrCount];
			}
			count -= 1;
			InstanceCount -= 1;
			// next loop should start from the same segment
			checkStart = 0;
			
		}
		// second case, swap the segments
		if ( segment->allocated == FALSE && nextSeg->allocated == TRUE){
			
			// get index in memory entry
			int seg1Entry = getIndex(segment->start);
			int seg2Entry = getIndex(nextSeg->start);

			
			// swap entries in memory
			for (int k = 0; k < nextSeg->size; k++){
				mymemory[seg1Entry + k] = mymemory[seg2Entry + k];
			}
			
			for(int j =0 ; j < segment->size; j++){
				mymemory[nextSeg->size + j] = '\0';
			}
			segment->start = &mymemory[seg1Entry];
			nextSeg->start = &mymemory[nextSeg->size];
			// swap the segments (or just change their attributes, same thing)
			swap(segment,nextSeg);
			// swap reference pointers
			void * temp = ptrlist[i];
			ptrlist[i] = ptrlist[i + 1];
			ptrlist[i + 1] = temp;
			checkStart = 1;

		}
		if(checkStart == 1){
		segment = segment->next;
		}
	} // first if
	loopcnt+= 1;
	} // for loop

	
}
void swap(Segment_t* segment, Segment_t* nextSeg){
	segment->allocated = TRUE;
	nextSeg->allocated = FALSE;
	
	
	// sizes
	size_t tempSize = segment->size;
	segment->size = nextSeg->size;
	nextSeg->size = tempSize;

}

// helper functions for management segmentation table
Segment_t * findFree ( Segment_t * list, size_t size )
{
   Segment_t * MemoryDescriptor = list->next;
   Segment_t * head = MemoryDescriptor;
   // Initially, see whether we have enough memory to allocate
   int MemoryCap =  MemoryDescriptor->size - size;
   if (MemoryCap >= 0){
	if (MemoryCap == 0){
		MemoryDescriptor->allocated = TRUE;
		printf("Memory is full \n");
	}
	//then search for free segment
	else{
	  for (int i = 0 ; i < InstanceCount - 1; i++){
		MemoryDescriptor = MemoryDescriptor->next;
		if (MemoryDescriptor->size >= size && MemoryDescriptor->allocated == FALSE){
			// here we check if our free segment's size == to the size we are looking for
			// then we just return the segment
			if (MemoryDescriptor->size == size){
			return MemoryDescriptor;
			}
			// if our free segment is bigger than the actual size,
			// we create a slice of unallocated memory
			//inserting it after the one we allocated
			else{
			size_t Offset = MemoryDescriptor->size - size;
			MemoryDescriptor->size = size;
			Segment_t* nextSeg = MemoryDescriptor->next;
			Segment_t* SlicedSeg = (struct segmentdescriptor *)malloc(sizeof(struct segmentdescriptor));
			MemoryDescriptor->next = SlicedSeg;
			SlicedSeg->next = nextSeg;
			SlicedSeg->allocated = FALSE;
			SlicedSeg->start = MemoryDescriptor->start + MemoryDescriptor->size;
			SlicedSeg->size = Offset;
			return MemoryDescriptor;
			
			}
		}
	}
	// no free segment, just create a new one 
	Segment_t * segmentDescriptor = createInstance(size);
	return segmentDescriptor;
        }
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
	if (Index == 0){
	  printf("Segment Table \n");
	  printsegmentdescriptor(segmentTable);
	  
	}
	else{
  	printf("Segment %i \n", Index);
	printsegmentdescriptor(segmentTable);
	}
	return printsegmenttable(segmentTable->next, Index + 1);
  }
}

//src : https://gist.github.com/ccbrown/9722406
void DumpHex(const void* data, size_t size) {
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			printf(" ");
			if ((i+1) % 16 == 0) {
				printf("|  %s \n", ascii);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					printf("   ");
				}
				printf("|  %s \n", ascii);
			}
		}
	}
}

void printsegmentdescriptor (Segment_t * descriptor)
{
      printf ( "\tallocated = %s\n" , (descriptor->allocated == FALSE ? "FALSE" : "TRUE" ) );
      printf ( "\tstart     = %p\n" , descriptor->start );
      printf ( "\tsize      = %lu\n", descriptor->size  );
      printf ( "\tnext      = %p\n", descriptor->next );
}


