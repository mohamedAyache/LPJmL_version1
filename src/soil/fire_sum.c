/***************************************************************************/
/**                                                                       **/
/**                f  i  r  e  _  s  u  m  .  c                           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 03.03.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real fire_sum(const Litter *litter, /* Litter pool */
              Real w_surf           /* surface water */
             )                      /* returns fire */
{
  Real moist_factor;
  moist_factor=moistfactor(litter);
  return (moist_factor>0) ? 
            exp(-M_PI*(w_surf/moist_factor)*(w_surf/moist_factor)) : 0;
} /* of 'fire_sum' */
