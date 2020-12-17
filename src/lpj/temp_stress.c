/***************************************************************************/
/**                                                                       **/
/**                t  e  m  p  _  s  t  r  e  s  s  .  c                  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function calculates temperature stress for photosynthesis         **/
/**     Adapted from Farquhar (1982) photosynthesis model, as simplified  **/
/**     by  Collatz et al 1991, Collatz et al 1992 and Haxeltine &        **/
/**     Prentice 1996                                                     **/ 
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 11.06.2004                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define tmc3 45.0   /* maximum temperature for C3 photosynthesis (deg C) */
#define tmc4 55.0   /* maximum temperature for C4 photosynthesis (deg C) */

Real temp_stress(const Pftpar *pftpar, /* PFT parameter */
                 Real temp,            /* Temperature (deg C) */
                 Real daylength        /* day length (h) */
                )                      /* returns temperature stress */
{
  Real low,high;
  if(daylength<0.01 || (pftpar->path==C3 && temp>tmc3) 
                    || (pftpar->path==C4 && temp>tmc4))
    return 0.0;
  if(temp<pftpar->temp_co2.high)
  {
    low=1/(1+exp(pftpar->k1*(pftpar->k2-temp)));
    high=1-0.01*exp(pftpar->k3*(temp-pftpar->temp_photos.high));
    return low*high;
  }
  return 0.0;
} /* of 'temp_stress' */
