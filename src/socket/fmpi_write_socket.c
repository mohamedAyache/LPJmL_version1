/***************************************************************************/
/**                                                                       **/
/**         f  s  o  c  k  e  t  _  w  r  i  t  e  a  l  l  .  c          **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     FORTRAN interface to socket_writeall()                            **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 12.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "channel.h"

#ifdef USE_MPI

void mpi_write_socket_(Socket *socket,    /* socket */
                      void *data,        /* data to be written to channel */
                      MPI_Datatype *type, /* MPI datatype of data */
                      int *size,
                      int counts[],
                      int offsets[],
                      int *rank,          /* MPI rank */
                      MPI_Comm *comm,     /* MPI communicator */
                      int *err            /* error code */
                     )
{
  *err=mpi_write_socket(socket,data,*type,*size,counts,offsets,*rank,*comm);
} /* of 'socket_writeall_' */

#endif
