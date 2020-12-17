/***************************************************************************/
/**                                                                       **/
/**               f i r e d a n g e r i n d e x . c                       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Ben Poulter, Kirsten Thonicke                          **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 23.5.08 Kirsten Thonicke                             **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define alpha_fuelp 0.000337

Real firedangerindex(Real char_moist_factor,
                     Real char_alpha_fuel,
                     Real nesterov_accum
                    )
{
  Real d_fdi;
#ifdef SAFE
  if(char_alpha_fuel < 0)
    fprintf(stderr,"char_alpha_fuel %f in firedangerindex\n",char_alpha_fuel);
#endif
  /* Calculate Fire Danger Index */
  if(nesterov_accum <= 0 || char_moist_factor <=0)
    d_fdi = 0;
  else
    d_fdi = (0.0 > (1.0-(1.0 / char_moist_factor * (exp(-alpha_fuelp * nesterov_accum)))) ?
             0 : (1.0-(1.0 / char_moist_factor * (exp(-alpha_fuelp * nesterov_accum)))));
  return d_fdi;
} /* of 'firedangerindex' */
