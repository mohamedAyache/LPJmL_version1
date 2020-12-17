/***************************************************************************/
/**                                                                       **/
/**                 c  l  o  s  e  _  n  e  t  c  d  f  .  c              **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function closes NetCDF file                                       **/
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

Bool close_netcdf(Netcdf *cdf)
{
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  if(cdf->state==APPEND || cdf->state==CREATE)
    return FALSE;
  return nc_close(cdf->ncid)!=NC_NOERR;
#else
  return TRUE;
#endif
} /* of 'close_netcdf' */
