/***************************************************************************/
/**                                                                       **/
/**                i  n  i  t  m  p  i  c  o  n  f  i  g  .  c            **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function initialize Config struct for parallel version            **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 02.12.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#ifdef USE_MPI

void initmpiconfig(Config *config, /* LPJ configuration */
                   MPI_Comm comm   /* MPI communicator */
                  )
{
  config->comm=comm;
  MPI_Comm_rank(comm,&config->rank); /* get my rank: 0..ntask-1 */
  MPI_Comm_size(comm,&config->ntask); /* get number of tasks */
} /* of 'initmpiconfig' */

#endif
