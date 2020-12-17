/***************************************************************************/
/**                                                                       **/
/**                 t  e  m  p  _  r  e  s  p  o  n  s  e  .  c           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     DESCRIPTION                                                       **/
/**                                                                       **/
/**     Calculates g(T), response of respiration rate to temperature (T), **/
/**     based on empirical relationship for temperature response of soil  **/
/**     temperature across ecosystems, incorporating damping of Q10       **/
/**     response due to temperature acclimation (Eqn 11, Lloyd & Taylor   **/
/**     1994)                                                             **/
/**                                                                       **/
/**     r    = r10 * g(t)                                                 **/
/**     g(T) = EXP[308.56 * (1 / 56.02 - 1 / (T - 227.13))] (T in Kelvin) **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 09.09.2004                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#define e0 308.56   /* parameter in Arrhenius temp response function */

Real temp_response(Real temp /* air or soil temperature (deg C) */
                  )          /* returns respiration temperature response */
{
  return (temp>=-40.0) ? exp(e0*(1.0/56.02-1.0/(temp+46.02))) : 0.0;
} /* of 'temp_response' */
