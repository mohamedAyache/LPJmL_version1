/***************************************************************************/
/**                                                                       **/
/**                      i  n  i  t  f  u  e  l  .  c                     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Kirsten Thonicke                                                  **/
/**     Potsdam Institute for Landuse Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 06.03.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void initfuel(Fuel *fuel)
{
  int i;
  fuel->char_moist_factor=fuel->char_alpha_fuel=fuel->char_net_fuel=0; 
  fuel->char_dens_fuel_ave=0.00001;
  fuel->cf=fuel->daily_litter_moist=0;   
  for(i=0;i<=NFUELCLASS;++i)
    fuel->deadfuel_consum[i]=0;
  fuel->gamma=0;
  fuel->moist_1hr=fuel->moist_10_100hr=fuel->mw_weight=1.0;
  fuel->sigma=0.00001; 
} /* of 'initfuel' */
