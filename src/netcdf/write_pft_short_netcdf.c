/***************************************************************************/
/**                                                                       **/
/**              w r i t e _ p f t _ s h o r t _ n e t c d f . c          **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function writes PFT short output in NetCDF file                   **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-09-29 16:02:37 +0200 (lun., 29 sept. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#if defined(USE_NETCDF) || defined(USE_NETCDF4)
#include <netcdf.h>
#endif

Bool write_pft_short_netcdf(const Netcdf *cdf,const short vec[],int year,
                            int pft,int size)
{
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  int i,rc;
  size_t offsets[4],counts[4];
  short *grid;
  counts[0]=counts[1]=1;
  counts[2]=cdf->index->nlat;
  counts[3]=cdf->index->nlon;
  offsets[0]=year;
  offsets[1]=pft;
  offsets[2]=offsets[3]=0;
  grid=newvec(short,cdf->index->nlon*cdf->index->nlat);
  if(grid==NULL)
  {
    printallocerr("grid");
    return TRUE;
  }
  for(i=0;i<cdf->index->nlon*cdf->index->nlat;i++)
    grid[i]=MISSING_VALUE_SHORT;
  for(i=0;i<size;i++)
    grid[cdf->index->index[i]]=vec[i];
  if(year==NO_TIME)
    rc=nc_put_var_short(cdf->ncid,cdf->varid,grid);
  else 
    rc=nc_put_vara_short(cdf->ncid,cdf->varid,offsets,counts,grid);
  if(rc)
  {
    fprintf(stderr,"ERROR431: Cannot write output data: %s.\n",
            nc_strerror(rc));
    free(grid);
    return TRUE;
  }
  free(grid);
  return FALSE;
#else
  return TRUE;
#endif
} /* of 'write_pft_short_netcdf' */
