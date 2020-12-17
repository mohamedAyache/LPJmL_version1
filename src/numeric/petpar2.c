/***************************************************************************/
/**                                                                       **/
/**                   p  e  t  p  a  r  2  .  c                           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function calculates daily photosynthetically active               **/
/**     radiation flux, daylength and daily potential evapotranspiration  **/
/**     given temperature, longwave radiation, shortwave radiation, and   **/
/**     latitude                                                          **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-11-06 11:17:06 +0100 (ven., 06 nov. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include <math.h>
#include "types.h"
#include "date.h"
#include "numeric.h"
#include "units.h"

#define sigma 5.6704e-8  /* Stefan-Boltzmann constant (W m-2 K-4) */
#define dayseconds 86400 /* seconds per day */

#define gamma_t(temp)  (65.05+temp*0.064)
#define lambda(temp) (2.495e6-temp*2380)

void petpar2(Real *daylength, /* daylength (h) */
             Real *par,       /* photosynthetic active radiation flux */
             Real *pet,       /* potential evapotranspiration */
             Real lat,        /* latitude (deg) */
             Real day,        /* day (1..365) */
             Real temp,       /* temperature (deg C) */
             Real lw,         /* longwave net/downward flux (W m-2) */
             Real swdown,     /* shortwave downward flux (W m-2) */
             Bool islwdown    /* LW radiation is downward (TRUE/FALSE) */ 
            )
{
  Real swnet;
  Real delta,u,v,hh,s;
  delta=deg2rad(-23.4*cos(2*M_PI*(day+10.0)/NDAYYEAR));
  u=sin(deg2rad(lat))*sin(delta);
  v=cos(deg2rad(lat))*cos(delta);

  if(u>=v)
  {
    *daylength=24;
  }
  else if(u<=-v)
    *daylength=0;
  else
  {
    hh=acos(-u/v);
    *daylength=24*hh*M_1_PI;
  }

  swnet=(1-beta)*swdown; /* shortwave net flux, downward positive (W m-2) */
  if(islwdown)
    lw-=sigma*pow(degCtoK(temp),4);
  *par=dayseconds*swnet/2;
  s=2.503e6*exp(17.269*temp/(237.3+temp))/
          ((237.3+temp)*(237.3+temp));

  /* calculation of PET respects only longwave flux during daylight  */
  *pet=dayseconds*(s/(s+gamma_t(temp))/lambda(temp))*(swnet+lw*(*daylength)/24);
  if (*pet<0) *pet=0;
} /* of 'petpar2' */
