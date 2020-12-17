/***************************************************************************/
/**                                                                       **/
/**                     p  n  e  t  _  i  n  i  t  .  c                   **/
/**                                                                       **/
/**     MPI-parallelization of networks                                   **/
/**                                                                       **/
/**     Function initializes pnet datatype                                **/
/**                                                                       **/
/**     Implementation is described in:                                   **/
/**     W. von Bloh, 2008. Sequential and Parallel Implementation of      **/
/**     Networks. In P. beim Graben, C. Zhou, M. Thiel, and J. Kurths     **/
/**     (eds.), Lectures in Supercomputational Neuroscience, Dynamics in  **/
/**     Complex Brain Networks, Springer, 279-318.                        **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 22.02.2010                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#ifdef USE_MPI
#include <mpi.h>
#endif
#include "types.h"
#include "pnet.h"

Pnet *pnet_init(
#ifdef USE_MPI
                MPI_Comm comm,     /* MPI communicator */
                MPI_Datatype type, /* MPI datatype of grid element */
#else
                int size,          /* size of grid element */
#endif
                int n /* size of grid */
               )      /* returns initialized pnet structure or NULL */
{
  int slice,rem,i;
  Pnet *pnet;
  if(n<=0) /* number of grid cells must be greater than zero */
    return NULL;
  pnet=new(Pnet);
  if(pnet==NULL)
    return NULL;
  pnet->n=n;
#ifdef USE_MPI
  /* determine total number of tasks and my task id */
  MPI_Comm_size(comm,&pnet->ntask);
  if(n<pnet->ntask) /* number of grid cells must not be < number of tasks */
  {
    free(pnet);
    return NULL;
  }
  MPI_Comm_rank(comm,&pnet->taskid);
  pnet->type=type;
  pnet->comm=comm;
#else
  /* sequential code */
  pnet->ntask=1;
  pnet->taskid=0;
  pnet->size=size;
  if(size<=0)
  {
    free(pnet);
    return NULL;
  }
#endif
  /* calculate lower and upper bound of local subgrid */
  slice=pnet->n/pnet->ntask;
  rem=pnet->n % pnet->ntask; /* calculate remainder */
  pnet->lo=pnet->taskid*slice;
  pnet->hi=(pnet->taskid+1)*slice-1;
  /* distribute remainder */
  if(pnet->taskid<rem)
  {
    pnet->lo+=pnet->taskid;
    pnet->hi+=pnet->taskid+1;
  }
  else
  {
    pnet->lo+=rem;
    pnet->hi+=rem;
  }
  /* allocate memory for connection list array */
  pnet->connect=newvec2(Intlist,pnet->lo,pnet->hi);
  if(pnet->connect+pnet->lo==NULL) /* was memory allocation sucessful? */
  {
    free(pnet);
    return NULL; /* no, return NULL */
  }
  /* initializes connection lists */
  pnet_foreach(pnet,i)
    initintlist(pnet->connect+i);
  /* allocate length and displacement vectors for input and output buffers */
  pnet->outdisp=newvec(int,pnet->ntask);
  if(pnet->outdisp==NULL)
  {
    freevec(pnet->connect,pnet->lo,pnet->hi);
    free(pnet);
    return NULL;
  }
  pnet->indisp=newvec(int,pnet->ntask);
  if(pnet->indisp==NULL)
  {
    free(pnet->outdisp);
    freevec(pnet->connect,pnet->lo,pnet->hi);
    free(pnet);
    return NULL;
  }
  pnet->outlen=newvec(int,pnet->ntask);
  if(pnet->outlen==NULL)
  {
    free(pnet->outdisp);
    free(pnet->indisp);
    freevec(pnet->connect,pnet->lo,pnet->hi);
    free(pnet);
    return NULL;
  }
  pnet->inlen=newvec(int,pnet->ntask);
  if(pnet->inlen==NULL)
  {
    free(pnet->outlen);
    free(pnet->outdisp);
    free(pnet->indisp);
    freevec(pnet->connect,pnet->lo,pnet->hi);
    free(pnet);
    return NULL;
  }
  return pnet;
} /* of 'pnet_init' */
