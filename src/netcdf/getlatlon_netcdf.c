/***************************************************************************/
/**                                                                       **/
/**       g  e  t  l  a  t  l  o  n  _  n  e  t  c  d  f  .  c            **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function gets longitude/latitude data in NetCDF file              **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#if defined(USE_NETCDF) || defined(USE_NETCDF4)
#include <netcdf.h>

#define error(var,rc) if(rc) {if(isroot(*config))fprintf(stderr,"ERROR403: Cannot read '%s' in '%s': %s.\n",var,filename,nc_strerror(rc)); return TRUE;}
#endif

Bool getlatlon_netcdf(Climatefile *file,    /* Climate data */
                      const char *filename, /* filename */
                      const Config *config  /* LPJ configuration */
                     )                      /* returns TRUE on error */
{
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  int rc,var_id,*dimids,ndims,index;
  char name[NC_MAX_NAME+1];
  float *dim;
  size_t len;
  nc_inq_varndims(file->ncid,file->varid,&ndims);
  if(ndims<2)
  {
    if(isroot(*config))
      fprintf(stderr,"ERROR408: Invalid number of dimensions %d in '%s'.\n",
              ndims,filename);
    return TRUE;
  }
  index=ndims-1;
  dimids=newvec(int,ndims);
  if(dimids==NULL)
  {
    printallocerr("dimids");
    return TRUE;
  }
  nc_inq_vardimid(file->ncid,file->varid,dimids);
  nc_inq_dimname(file->ncid,dimids[index],name);
  rc=nc_inq_varid(file->ncid,name,&var_id);
  if(rc)
  {
    if(isroot(*config))
      fprintf(stderr,"ERROR410: Cannot read %s in '%s': %s.\n",
              name,filename,nc_strerror(rc));
    free(dimids);
    return TRUE;
  }
  nc_inq_dimlen(file->ncid,dimids[index],&len);
  file->nlon=len;
  dim=newvec(float,len);
  if(dim==NULL)
  {
    free(dimids);
    printallocerr("dim");
    return TRUE;
  }
  rc=nc_get_var_float(file->ncid,var_id,dim);
  if(rc)
  {
    free(dimids);
    free(dim);
    if(isroot(*config))
      fprintf(stderr,"ERROR411: Cannot read longitude in '%s': %s.\n",
              filename,nc_strerror(rc));
    return TRUE;
  }
  file->lon_min=dim[0];
  if(dim[1]-dim[0]!=config->resolution.lon)
  {
    if(isroot(*config))
      fprintf(stderr,"ERROR412: Incompatible resolution %g for longitude in '%s'.\n",
              dim[1]-dim[0],filename);
    free(dim);
    free(dimids);
    return TRUE;
  }
  free(dim);
  nc_inq_dimname(file->ncid,dimids[index-1],name);
  rc=nc_inq_varid(file->ncid,name,&var_id);
  if(rc)
  {
    if(isroot(*config))
      fprintf(stderr,"ERROR410: Cannot read %s in '%s': %s.\n",
              name,filename,nc_strerror(rc));
    free(dimids);
    return TRUE;
  }
  nc_inq_dimlen(file->ncid,dimids[index-1],&len);
  free(dimids);
  file->nlat=len;
  dim=newvec(float,len);
  if(dim==NULL)
  {
    printallocerr("dim");
    return TRUE;
  }
  rc=nc_get_var_float(file->ncid,var_id,dim);
  if(rc)
  {
    free(dim);
    if(isroot(*config))
      fprintf(stderr,"ERROR404: Cannot read latitude in '%s': %s.\n",
              filename,nc_strerror(rc));
    return TRUE;
  }
  if(dim[1]>dim[0])
  {
    file->lat_min=dim[0];
    file->offset=0;
  }
  else
  {
    file->lat_min=dim[len-1];
    file->offset=len-1;
  }
  if(fabs(dim[0]-dim[1])!=config->resolution.lat)
  {
    if(isroot(*config))
      fprintf(stderr,"ERROR413: Incompatible resolution %g for latitude in '%s'.\n",
              dim[0]-dim[1],filename);
    free(dim);
    return TRUE;
  }
  free(dim);
  return FALSE;
#else
  return TRUE;
#endif
} /* of 'getlatlon_netcdf' */
