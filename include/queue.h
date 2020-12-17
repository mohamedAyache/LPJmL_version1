/***************************************************************************/
/**                                                                       **/
/**                       q  u  e  u  e  .  h                             **/
/**                                                                       **/
/**     C implementation of a delay queue                                 **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-14 09:09:50 +0200 (mar. 14 juil. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef QUEUE_T  /* Already included? */
#define QUEUE_T

/* Definition of datatypes */

typedef struct queue *Queue;

/* Declarations of functions */

extern Queue newqueue(int);
extern void freequeue(Queue);
extern Real getqueue(const Queue,int);
extern void putqueue(Queue,Real);
extern Real sumqueue(const Queue);
extern Bool fwritequeue(FILE *,const Queue);
extern Queue freadqueue(FILE *,Bool);
extern void freequeue(Queue);
extern void fprintqueue(FILE *,const Queue);
extern int queuesize(const Queue);
extern void up_queue(Queue, Real, int); // barrier.n

/* Definition of macros */

#endif
