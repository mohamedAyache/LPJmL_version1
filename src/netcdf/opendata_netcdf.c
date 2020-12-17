/***************************************************************************/
/**                                                                       **/
/**          o  p  e  n  d  a  t  a  _  n  e  t  c  d  f  .  c            **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function opens data file in NetCDF format                         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-12-17 16:47:49 +0100 (mer., 17 déc. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#if defined(USE_NETCDF) || defined(USE_NETCDF4)
#include <netcdf.h>

#define error(var,rc) if(rc) {if(isroot(*config))fprintf(stderr,"ERROR403: Cannot read '%s' in '%s': %s.\n",var,filename,nc_strerror(rc)); nc_close(file->ncid); return TRUE;}

#endif

Bool opendata_netcdf(Climatefile *file,    /* data file */
                     const char *filename, /* filename */
                     const char *var,      /* variable file name or NULL */
                     const char *units,    /* units or NULL */
                     const Config *config  /* LPJ configuration */
                    )                      /* returns TRUE on error */
{
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  int rc;
  if(isroot(*config))
    rc=openfile_netcdf(file,filename,var,units,config);
#ifdef USE_MPI
  MPI_Bcast(&rc,1,MPI_INT,0,config->comm);
#endif
  if(rc)
    return TRUE;
#ifdef USE_MPI
  MPI_Bcast(&file->type,1,MPI_INT,0,config->comm);
  MPI_Bcast(&file->var_len,sizeof(size_t),MPI_BYTE,0,config->comm);
  MPI_Bcast(&file->isdaily,1,MPI_INT,0,config->comm);
  MPI_Bcast(&file->oneyear,1,MPI_INT,0,config->comm);
  MPI_Bcast(&file->n,1,MPI_INT,0,config->comm);
  MPI_Bcast(&file->firstyear,1,MPI_INT,0,config->comm);
  MPI_Bcast(&file->nyear,1,MPI_INT,0,config->comm);
  MPI_Bcast(&file->nlon,1,MPI_INT,0,config->comm);
  MPI_Bcast(&file->nlat,1,MPI_INT,0,config->comm);
  MPI_Bcast(&file->lon_min,1,MPI_FLOAT,0,config->comm);
  MPI_Bcast(&file->lat_min,1,MPI_FLOAT,0,config->comm);
  MPI_Bcast(&file->slope,1,MPI_DOUBLE,0,config->comm);
  MPI_Bcast(&file->intercept,1,MPI_DOUBLE,0,config->comm);
  MPI_Bcast(&file->offset,sizeof(size_t),MPI_BYTE,0,config->comm);
  MPI_Bcast(&file->missing_value,sizeof(file->missing_value),MPI_BYTE,0,config->comm);
#endif
  return FALSE;
#else
  if(isroot(*config))
    fputs("ERROR401: NetCDF input is not supported by this version of LPJmL.\n",stderr);
  return TRUE;
#endif
} /* of 'opendata_netcdf' */
