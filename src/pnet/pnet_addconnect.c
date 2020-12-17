/***************************************************************************/
/**                                                                       **/
/**               p  n  e  t  _  a  d  d  c  o  n  n  e  c  t  .  c       **/
/**                                                                       **/
/**     MPI-parallelization of networks                                   **/
/**                                                                       **/
/**     Function adds connection to network                               **/
/**                                                                       **/
/**     Implementation is described in:                                   **/
/**     W. von Bloh, 2008. Sequential and Parallel Implementation of      **/
/**     Networks. In P. beim Graben, C. Zhou, M. Thiel, and J. Kurths     **/
/**     (eds.), Lectures in Supercomputational Neuroscience, Dynamics in  **/
/**     Complex Brain Networks, Springer, New York, 279-318.              **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 08.07.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#ifdef USE_MPI
#include <mpi.h>
#endif
#include "types.h"
#include "pnet.h"

int pnet_addconnect(Pnet *pnet, /* Pointer to pnet structure */
                    int to,     /* target index of connection */
                    int from    /* source index of connection */
                   )            /* returns error code */
{
  if(pnet==NULL)
    return PNET_NULL_PTR_ERR;
  /* check whether from and to index are valid */
  if(to<pnet->lo || to>pnet->hi)
    return PNET_TO_INDEX_ERR;
  if(from<0 || from>=pnet->n)
    return PNET_FROM_INDEX_ERR;
  return (addintlistitem(pnet->connect+to,from)==0) ? PNET_ALLOC_ERR : PNET_OK;
} /* of 'pnet_addconnect' */
