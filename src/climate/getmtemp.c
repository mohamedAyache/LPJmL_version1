/***************************************************************************/
/**                                                                       **/
/**                      g  e  t  m  t  e  m  p  .  c                     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function getmtemp gets monthly value of temperature               **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  14.10.2008                                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real getmtemp(const Climate *climate, /* pointer to climate data */
              const Climbuf *climbuf,
              int cell,                /* cell index */
              int month                /* month (0..11) */
             )             /* returns monthly averaged temperature (deg C) */
{
  return (climate->file_temp.isdaily) ? climbuf->mtemp/ndaymonth[month] : (getcelltemp(climate,cell))[month];
} /* of 'getmtemp' */
