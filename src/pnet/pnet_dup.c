/***************************************************************************/
/**                                                                       **/
/**                     p  n  e  t  _  d  u  p  .  c                      **/
/**                                                                       **/
/**     MPI-parallelization of networks                                   **/
/**                                                                       **/
/**     Function duplicates pnet datatype                                 **/
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
/**     Last change: 09.03.2010                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#ifdef USE_MPI
#include <mpi.h>
#endif
#include "types.h"
#include "pnet.h"

Pnet *pnet_dup(const Pnet *pnet
              )      /* returns duplicated pnet structure or NULL */
{
  int i;
  Pnet *ret;
  if(pnet==NULL)
    return NULL;
  ret=new(Pnet);
  if(ret==NULL)
    return NULL;
  ret->n=pnet->n;
#ifdef USE_MPI
  ret->type=pnet->type;
  ret->comm=pnet->comm;
#else
  ret->size=pnet->size;
#endif
  ret->taskid=pnet->taskid;
  ret->ntask=pnet->ntask;
  ret->lo=pnet->lo;
  ret->hi=pnet->hi;
  /* allocate memory for connection list array */
  ret->connect=newvec2(Intlist,pnet->lo,pnet->hi);
  if(ret->connect+pnet->lo==NULL) /* was memory allocation sucessful? */
  {
    free(ret);
    return NULL; /* no, return NULL */
  }
  /* copy connection lists */
  pnet_foreach(pnet,i)
    if(copyintlist(ret->connect+i,pnet->connect+i))
    { /* error occcured duplicating list */
      free(ret);
      return NULL;
    }
  /* allocate length and displacement vectors for input and output buffers */
  ret->outdisp=newvec(int,pnet->ntask);
  if(ret->outdisp==NULL)
  {
    freevec(ret->connect,pnet->lo,pnet->hi);
    free(ret);
    return NULL;
  }
  ret->indisp=newvec(int,pnet->ntask);
  if(ret->indisp==NULL)
  {
    free(ret->outdisp);
    freevec(ret->connect,pnet->lo,pnet->hi);
    free(ret);
    return NULL;
  }
  ret->outlen=newvec(int,pnet->ntask);
  if(ret->outlen==NULL)
  {
    free(ret->outdisp);
    free(ret->indisp);
    freevec(ret->connect,pnet->lo,pnet->hi);
    free(ret);
    return NULL;
  }
  ret->inlen=newvec(int,pnet->ntask);
  if(ret->inlen==NULL)
  {
    free(ret->outdisp);
    free(ret->indisp);
    free(ret->outlen);
    freevec(ret->connect,pnet->lo,pnet->hi);
    free(ret);
    return NULL;
  }
  return ret;
} /* of 'pnet_dup' */
