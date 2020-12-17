/***************************************************************************/
/**                                                                       **/
/**                p  n  e  t  _  s  t  r  e  r  r  o  r  .  c            **/
/**                                                                       **/
/**     MPI-parallelization of networks                                   **/
/**                                                                       **/
/**     Function gets error message string                                **/
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

char *pnet_strerror(int err)
{
  static char *errstr[]={"No error",
                         "Could not allocate memory",
                         "Invalid to index",
                         "Invalid from index",
                         "Invalid pointer",
                         "unknown error"};
  return errstr[(err<0 || err>PNET_NULL_PTR_ERR) ? 5 : err];
} /* of 'pnet_strerror' */
