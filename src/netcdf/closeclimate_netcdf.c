/***************************************************************************/
/**                                                                       **/
/**    c  l  o  s  e  c  l  i  m  a  t  e  _  n  e  t  c  d  f  .  c      **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function closes climate data file in NetCDF format                **/
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

void closeclimate_netcdf(Climatefile *file,Bool isroot)
{
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  if(file->oneyear)
    free(file->filename);
  else if(isroot)
    free_netcdf(file->ncid);
#endif
} /* of 'closeclimate_netcdf' */
