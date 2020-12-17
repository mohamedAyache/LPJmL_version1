/***************************************************************************/
/**                                                                       **/
/**                c  l  o  s  e  c  l  i  m  a  t  e  .  c               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function closes open file                                         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 19.02.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void closeclimatefile(Climatefile *file, /* pointer to climate data file */
                      Bool isroot        /* task is root task (TRUE/FALSE) */
                     )
{
  if(file->fmt!=FMS)
  {
    if(file->fmt==CDF)
      closeclimate_netcdf(file,isroot);
    else
      fclose(file->file);
  }
} /* of 'closeclimatefile' */
