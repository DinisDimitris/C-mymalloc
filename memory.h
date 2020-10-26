/* mymemory.h
 * 
 * describes structures for memory management
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#ifndef MYMEMORY_H
#define MYMEMORY_H


#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define kMAXMEM       1024
#define kMAXSEGMENTS   100


// this typedef defines a byte
typedef unsigned char Byte ;

extern int InstanceCount;
// this is our memory, we declare it as extern so that it becomes global
extern Byte mymemory [ kMAXMEM ] ;

// the segment descriptor, a node in a linked list
typedef struct segmentdescriptor {
   
   Byte     allocated ;
   void * start ;
   size_t   size  ;
   struct segmentdescriptor * next ;
} Segment_t ;


extern Segment_t * segmenttable;


// forward references, user interface
void        initialize () ;
void      * mymalloc   ( size_t  ) ;
void        myfree     ( void *  ) ;
void        mydefrag   () ;

// forward references, managing segmentation table
Segment_t * findFree    ( Segment_t *, size_t      ) ;
void        insertAfter ( Segment_t *, Segment_t * ) ;
Segment_t * createInstance() ;
void deleteMemoryEntries(size_t size, int index);

// forward references, helper functions
int rearrangeStartPtrs(Segment_t* Seg,void *start);
Segment_t * findSegment ( Segment_t * list, void * ptr );
int isPrintable ( int c ) ;
void printmemory (Byte mymemory []) ;
int printsegmenttable() ;
void printsegmentdescriptor() ;

#endif
