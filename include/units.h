/***************************************************************************/
/**                                                                       **/
/**                      u  n  i  t  s  .  h                              **/
/**                                                                       **/
/**     Header for unit conversions                                       **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 30.09.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#ifndef UNITS_H /* Already included? */
#define UNITS_H

/* Definition of macros */

#define deg2rad(deg) ((deg)*M_PI*.00555555555555555555) /* Convert degree -> radian */
#define rad2deg(rad) ((rad)*180*M_1_PI) /* Convert radian -> degree */
#define degCtoK(deg) ((deg)+273.15)  /* Convert deg C --> Kelvin */
#define ppm2bar(ppm) ((ppm)*1e-6)      /* Convert ppmv --> bar */
#define ppm2Pa(ppm) ((ppm)*1e-1)      /* Convert ppmv --> Pa */
#define hour2sec(hour) ((hour)*3600)      /* Convert hour --> sec */
#define day2hour(day) ((day)*24)          /* Convert day --> hour */
#define hour2day(hour) ((hour)*.04166666666666666666) /* Convert hour --> day */
#define c2biomass(c) ((c)*2.22222222222222222222) /* convert Carbon to Biomass */
#define biomass2c(c) ((c)*0.45)      /* convert Biomass to carbon */

#endif
