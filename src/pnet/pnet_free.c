/***************************************************************************/
/**                                                                       **/
/**                     p  n  e  t  _  f  r  e  e  .  c                   **/
/**                                                                       **/
/**     MPI-parallelization of networks                                   **/
/**                                                                       **/
/**     Function frees allocated memory of Pnet structure                 **/
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
/**     Last change: 17.05.2010                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#ifdef USE_MPI
#include <mpi.h>
#endif
#include "types.h"
#include "pnet.h"

void pnet_free(Pnet *pnet)
{
  int i;
  if(pnet!=NULL)
  {
    free(pnet->outdisp);
    free(pnet->indisp);
    free(pnet->outlen);
    free(pnet->inlen);
    free(pnet->inbuffer);
    free(pnet->outbuffer);
    free(pnet->outindex);
    /* empty connection lists */
    for(i=pnet->lo;i<=pnet->hi;i++)
      emptyintlist(pnet->connect+i);
    freevec(pnet->connect,pnet->lo,pnet->hi);
    free(pnet);
  }
} /* of 'pnet_free' */
