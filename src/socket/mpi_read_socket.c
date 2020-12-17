/***************************************************************************/
/**                                                                       **/
/**          m  p  i  _  r  e  a  d  _  s  o  c  k  e  t  .  c            **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads data using socket connection and distributes data  **/
/**     to all tasks.                                                     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 09.03.2010                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "channel.h"
#include "errmsg.h"

#ifdef USE_MPI

Bool mpi_read_socket(Socket *s,         /* Socket */
                     void *data,        /* data to be read from channel */
                     MPI_Datatype type, /* MPI datatype of data */
                     int size,
                     int counts[],
                     int offsets[],
                     int rank,          /* MPI rank */
                     MPI_Comm comm      /* MPI communicator */
                    )
{
  Bool rc;
  MPI_Aint extent;
  void *vec;
  MPI_Type_extent(type,&extent);

  if(rank==0)
  {
    vec=malloc(size*extent); /* allocate receive buffer */
    check(vec);
    if(type==MPI_FLOAT)
      rc=readfloat_socket(s,vec,size); /* read float data from socket */
    else if(type==MPI_INT)
      rc=readint_socket(s,vec,size); /* read int data from socket */
    else if(type==MPI_SHORT)
      rc=readshort_socket(s,vec,size); /* read short data from socket */
    else if(type==MPI_DOUBLE)
      rc=readdouble_socket(s,vec,size); /* read double data from socket */
    else if(type==MPI_LONG_LONG_INT)
      rc=readlong_socket(s,vec,size); /* read long data from socket */
  }
  else
    rc=FALSE;
  MPI_Scatterv(vec,counts,offsets,type,
               data,counts[rank],type,0,comm);
  if(rank==0)
    free(vec);
  MPI_Barrier(comm);
  return rc;
} /* of 'mpi_read_socket' */

#endif
