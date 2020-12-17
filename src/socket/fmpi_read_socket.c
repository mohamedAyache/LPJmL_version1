/***************************************************************************/
/**                                                                       **/
/**         f  s  o  c  k  e  t  _  r  e  a  d  a  l  l  .  c             **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     FORTRAN interface to socket_readall()                             **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-15 12:26:32 +0200 (mer., 15 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "channel.h"

#ifdef USE_MPI

void mpi_read_socket_(Socket *socket,    /* socket */
                     void *data,        /* data to be read from channel */
                     MPI_Datatype *type, /* MPI datatype of data */
                     int *size,
                     int counts[],
                     int offsets[],
                     int *rank,          /* MPI rank */
                     MPI_Comm *comm,     /* MPI communicator */
                     int *err            /* error code */
                    )
{
  *err=mpi_read_socket(socket,data,*type,*size,counts,offsets,*rank,*comm);
} /* of 'mpi_read_socket_' */

#endif
