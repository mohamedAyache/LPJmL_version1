/***************************************************************************/
/**                                                                       **/
/**                       p  n  e  t  .  h                                **/
/**                                                                       **/
/**     Header file for Pnet library: MPI-parallelization of networks     **/
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
/**     Last change: 21.10.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#ifndef PNET_H /* Already included? */
#define PNET_H

#include "intlist.h"

/* Definition of constants */

#define PNET_VERSION "1.0.003"

/* Return codes for Pnet functions */

#define PNET_ALLOC_ERR 1      /* error allocating memory */
#define PNET_TO_INDEX_ERR 2   /* invalid to index */
#define PNET_FROM_INDEX_ERR 3 /* invalid from index */
#define PNET_NULL_PTR_ERR 4   /* NULL pointer error */
#define PNET_OK 0             /* code for successful operation */

/* Definition of datatypes */

typedef struct
{
#ifdef USE_MPI
  MPI_Comm comm;     /* MPI communicator */
  MPI_Datatype type; /* MPI datatype of grid element */
#else
  int size;          /* size of grid element */
#endif
  int n;             /* total size of array */
  int lo;            /* lower bound of subarray */
  int hi;            /* upper bound of subarray */
  int ntask;         /* number of tasks */
  int taskid;        /* my task identifier (0..ntask-1) */
  int outsize;       /* size of output buffer */
  int *outdisp;      /* displacement vector for output */
  int *indisp;       /* displacement vector for input */
  int *inlen;        /* array sizes for input */
  int *outlen;       /* array sizes for output */
  void *outbuffer;   /* output buffer */
  void *inbuffer;    /* input buffer */
  int *outindex;     /* output index vector */
  Intlist *connect;  /* connection lists */
} Pnet;

/* Declaration of functions */

#ifdef USE_MPI
extern Pnet *pnet_init(MPI_Comm,MPI_Datatype,int);
#else
extern Pnet *pnet_init(int,int);
#endif
extern Pnet *pnet_dup(const Pnet *);
extern int pnet_setup(Pnet *);
extern int pnet_reverse(Pnet *);
extern void pnet_free(Pnet *);
extern int pnet_addconnect(Pnet *,int,int);
extern char *pnet_strerror(int);

/* Definitions of macros */

#ifdef USE_MPI
#define pnet_exchg(pnet) MPI_Alltoallv(pnet->outbuffer,pnet->outlen,pnet->outdisp,pnet->type,pnet->inbuffer,pnet->inlen,pnet->indisp,pnet->type,pnet->comm)
#else
#define pnet_exchg(pnet) memcpy(pnet->inbuffer,pnet->outbuffer,pnet->size*pnet->inlen[0])
#endif
#define pnet_foreach(pnet,i) for(i=pnet->lo;i<=pnet->hi;i++)
#define pnet_taskid(pnet) pnet->taskid
#define pnet_lo(pnet) pnet->lo
#define pnet_hi(pnet) pnet->hi
#define pnet_nodes(pnet) pnet->n
#define pnet_input(pnet) pnet->inbuffer
#define pnet_output(pnet) pnet->outbuffer
#define pnet_outlen(pnet) pnet->outsize
#define pnet_inindex(pnet,i,j) pnet->connect[i].index[j]
#define pnet_outindex(pnet,i) pnet->outindex[i]
#define pnet_inlen(pnet,i) pnet->connect[i].n
#define pnet_isin(pnet,i) (pnet->lo<=i && pnet->hi>=i)

#endif
