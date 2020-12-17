/***************************************************************************/
/**                                                                       **/
/**                      g  e  t  c  o  2  .  c                           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function get atmospheric CO2 concentration for specified year.    **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  17.01.2009                                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"


Real getco2(const Climate *climate, /* Pointer to climate data */
            int year                /* year (AD) */
           )                        /* returns atmospheric CO2 (ppm) */
{
  year-=climate->co2.firstyear;
  if(year>=climate->co2.nyear)
    fail(INVALID_YEAR_ERR,FALSE,"Invalid yr=%d in getco2().",
         year+climate->co2.firstyear);
  return (year<0) ? param.co2_p : climate->co2.data[year];
} /* of 'getco2' */
