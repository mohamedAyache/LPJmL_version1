/***************************************************************************/
/**                                                                       **/
/**                  m  p  i  _  w  r  i  t  e  .  c                      **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function writes output from all tasks using MPI_Gatherv           **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-07-09 13:39:52 +0200 (mer., 09 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#ifdef USE_MPI

int mpi_write(FILE *file,        /* File pointer to binary file */
              void *data,        /* data to be written to disk */
              MPI_Datatype type, /* MPI datatype of data */
              int size,
              int counts[],
              int offsets[],
              int rank,          /* MPI rank */
              MPI_Comm comm      /* MPI communicator */
             )                   /* returns number of items written to disk */
{
  int rc=0;
  MPI_Aint extent;
  MPI_Type_extent(type,&extent);
  void *vec;
  if(rank==0)
  {
    vec=malloc(size*extent); /* allocate receive buffer */
    check(vec);
  }
  MPI_Gatherv(data,counts[rank],type,vec,counts,offsets,type,0,comm);
  if(rank==0)
  {
    rc=fwrite(vec,extent,size,file); /* write data to file */
    if(rc!=size)
      fprintf(stderr,"ERROR204: Error writing output: %s.\n",strerror(errno));
    free(vec);
  }
  MPI_Barrier(comm);
  return rc;
} /* of 'mpi_write' */
#endif
