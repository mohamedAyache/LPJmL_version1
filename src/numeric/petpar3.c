/***************************************************************************/
/**                                                                       **/
/**                   p  e  t  p  a  r  3  .  c                           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function calculates daily photosynthetically active               **/
/**     radiation flux, daylength and daily potential evapotranspiration  **/
/**     given temperature, shortwave radiation, and latitude              **/
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

#define dayseconds 86400	/* seconds per day */
#define a 107.0
#define b 0.2
#define qoo 1376.0	/* solar constant (1360 W/m2) */
#define c 0.25
#define d 0.5
#define k 13750.98708	/* conversion factor from solar angular units to
                           seconds (12/pi*3600) */

#define gamma_t(temp)  (65.05+temp*0.064)
#define lambda(temp) (2.495e6-temp*2380)

void petpar3(Real *daylength, /* daylength (h) */
             Real *par,       /* photosynthetic active radiation flux */
             Real *pet,       /* potential evapotranspiration */
             Real lat,        /* latitude (deg) */
             Real day,        /* day (1..365) */
             Real temp,       /* temperature (deg C) */
             Real swdown      /* shortwave downward flux (W m-2) */
            )
{
  Real swnet,sun,lw_net;
  Real delta,u,v,hh,s,w;
  delta=deg2rad(-23.4*cos(2*M_PI*(day+10.0)/NDAYYEAR));
  u=sin(deg2rad(lat))*sin(delta);
  v=cos(deg2rad(lat))*cos(delta);
  swnet=(1-beta)*swdown; /* shortwave net flux, downward positive (W m-2) */
  if(u>=v)
  {
    *daylength=24;
    w=swnet*dayseconds/(2*(u*M_PI)*k);
  }
  else if(u<=-v)
  {
    w=0;
    *daylength=0;
  }
  else
  {
    hh=acos(-u/v);
    *daylength=24*hh*M_1_PI;
    w=swnet*dayseconds/(2*(u*hh+v*sin(hh))*k);
  }
  sun=((w/((1-beta)*qoo*(1.0+2.0*0.01675*cos(2.0*M_PI*day/365))))-c)/d;
  if(sun<0)
    sun=0;
  else if (sun>1)
    sun=1;
  lw_net=(b+(1-b)*sun)*(a-temp)*(*daylength)/24*k;

  *par=dayseconds*swnet/2;
  s=2.503e6*exp(17.269*temp/(237.3+temp))/
          ((237.3+temp)*(237.3+temp));
  *pet=2*(s/(s+(65.05+temp*0.064))/(2.495e6-temp*2380))*(swnet*dayseconds/2-lw_net);
    //fprintf(stdout,"swnet=%.2f sun=%.2f pet=%.2f\n",swnet,sun,*pet);

} /* of 'petpar3' */
