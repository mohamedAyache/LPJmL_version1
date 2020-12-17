/***************************************************************************/
/**                                                                       **/
/**         m  p  i  _  w  r  i  t  e  _  s  o  c  k  e  t  .  c          **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function writes data from all tasks using socket connection       **/
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

Bool mpi_write_socket(Socket *socket,    /* socket */
                      void *data,        /* data to be written to channel */
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
  }
  MPI_Gatherv(data,counts[rank],type,vec,counts,offsets,type,0,comm);
  if(rank==0)
  {
    rc=write_socket(socket,vec,size*extent);
    free(vec);
  }
  else
    rc=FALSE;
  MPI_Barrier(comm);
  return rc;
} /* of 'mpi_write_socket' */

#endif
