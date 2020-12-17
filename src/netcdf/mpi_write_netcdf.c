/***************************************************************************/
/**                                                                       **/
/**                m p i _ w r i t e _ n e t c d f . c                    **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function writes output from all tasks using MPI_Gatherv into      **/
/**     NetCDF file                                                       **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 05.10.2011                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#ifdef USE_MPI

Bool mpi_write_netcdf(const Netcdf *cdf, /* Pointer to Netcdf */
                      void *data,        /* data to be written to file */
                      MPI_Datatype type, /* MPI datatype of data */
                      int size,
                      int year,
                      int counts[],
                      int offsets[],
                      int rank,          /* MPI rank */
                      MPI_Comm comm      /* MPI communicator */
                     )                   /* returns TRUE on error */
{
  Bool rc;
  MPI_Aint extent;
  void *vec;
  MPI_Type_extent(type,&extent);
  if(rank==0)
  {
    vec=malloc(size*extent); /* allocate receive buffer */
    if(vec==NULL)
    {
      printallocerr("vec");
      rc=TRUE;
    }
    else
      rc=FALSE;
  }
  MPI_Bcast(&rc,1,MPI_INT,0,comm);
  if(rc)
    return TRUE;
  MPI_Gatherv(data,counts[rank],type,vec,counts,offsets,type,0,comm);
  if(rank==0)
  {
    if(type==MPI_FLOAT)
      rc=write_float_netcdf(cdf,vec,year,size); /* write data to file */
    else if(type==MPI_SHORT)
      rc=write_short_netcdf(cdf,vec,year,size); /* write data to file */
    else if(type==MPI_INT)
      rc=write_int_netcdf(cdf,vec,year,size); /* write data to file */
    free(vec);
  }
  /* broadcast return code to all other tasks */
  MPI_Bcast(&rc,1,MPI_INT,0,comm);
  MPI_Barrier(comm);
  return rc;
} /* of 'mpi_write_netcdf' */
#endif
