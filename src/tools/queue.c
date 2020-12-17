/***************************************************************************/
/**                                                                       **/
/**                       q  u  e  u  e  .  c                             **/
/**                                                                       **/
/**     C implementation of a Real valued delay queue                     **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-14 09:09:50 +0200 (mar., 14 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "swap.h"
#include "queue.h"

struct queue
{
  Real *data; /* data array */
  int size;   /* size of queue */
  int first;  /* index of first element in queue */
}; /* definition of opaque datatype Queue */


/* barrier.n */
void up_queue(Queue queue, Real val, int i)
{
    queue->data[(queue->first+i) % queue->size]=val;
}
int queuefirst(const Queue queue) /* returns size of queue */
{
  return queue->first;
} /* of 'queuesize' */

Queue newqueue(int size /* size of queue */
              )         /* returns pointer to queue or NULL on error */
{
  Queue queue;
  int i;
  /* size of queue must be >0 */
  if(size<1)
    return NULL;
  /* allocate data */
  queue=new(struct queue);
  if(queue==NULL)
    return NULL;
  queue->data=newvec(Real,size);
  if(queue->data==NULL)
  {
    free(queue);
    return NULL;
  }
  /* initialize queue with zeros */
  for(i=0;i<size;i++)
    queue->data[i]=0;
  queue->size=size;
  queue->first=size-1;
  return queue;
} /* of 'newqueue' */

Bool fwritequeue(FILE *file,const Queue queue)
{
  fwrite(&queue->size,sizeof(int),1,file);
  fwrite(&queue->first,sizeof(int),1,file);
  return fwrite(queue->data,sizeof(Real),queue->size,file)!=queue->size;
} /* of 'fwritequeue' */

void fprintqueue(FILE *file,const Queue queue)
{
  int i;
  for(i=0;i<queue->size;i++)
    fprintf(file," %g",queue->data[(queue->first+i) % queue->size]);
} /* of 'fprintqueue' */

void printqueue(const Queue queue)
{
  int i;
  for(i=0;i<queue->size;i++)
    printf("index= %i data= %g \n",i,queue->data[(queue->first+i) % queue->size]);
} /* of 'fprintqueue' */

Queue freadqueue(FILE *file,Bool swap)
{
  Queue queue1,queue2;
  queue1=new(struct queue);
  queue2=new(struct queue);
  int i;
  if(queue1==NULL)
    return NULL;
  if(freadint1(&queue1->size,swap,file)!=1)
  {
    free(queue1);
    return NULL;
  }
  if(freadint1(&queue1->first,swap,file)!=1)
  {
    free(queue1);
    return NULL;
  }
  if(queue1->first<0 || queue1->first>=queue1->size)
  {
    free(queue1);
    return NULL;
  }
  queue1->data=newvec(Real,queue1->size);

  if(queue1->data==NULL)
  {
    free(queue1);
    return NULL;
  }
  if(freadreal(queue1->data,queue1->size,swap,file)!=queue1->size)
  {
    free(queue1->data);
    free(queue1);
    return NULL;
  }
  return queue1;
} /* of 'freadqueue' */

Real getqueue(const Queue queue, /* pointer to queue */
              int i              /* index of requested queue element */
             )                   /* returns first element in queue */
{
  return queue->data[(queue->first+i) % queue->size];
} /* of 'getqueue' */

int queuesize(const Queue queue) /* returns size of queue */
{
  return queue->size;
} /* of 'queuesize' */

void putqueue(Queue queue, /* pointer to queue */
              Real val     /* value to be put in queue */
             )
{
  /*
   * move index of first element and store val there
   */
  queue->first=(queue->first-1+queue->size) % queue->size;
  queue->data[queue->first]=val;
} /* of 'putqueue' */

Real sumqueue(const Queue queue /* pointer to queue */
             )                  /* returns total sum */
{
  int i;
  Real sum;
  sum=0;

  for(i=0;i<queue->size;i++)
    sum+=queue->data[i];
  return sum;
} /* of 'sumqueue' */

void freequeue(Queue queue)
{
  if(queue!=NULL)
  {
    free(queue->data);
    free(queue);
  }
} /* of 'freequeue' */
