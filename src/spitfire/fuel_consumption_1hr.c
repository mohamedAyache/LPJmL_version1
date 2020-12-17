/***************************************************************************/
/**                                                                       **/
/**               f u e l _ c o n s u m p t i o n _ 1 h r . c             **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Kirsten Thonicke                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 12.03.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real fuel_consumption_1hr(Real moist_par,Real fire_frac)
{
  Real fuel_consum_1hr;

  if(moist_par <= 0.18) 
    fuel_consum_1hr = 1.0*(1.0-MINER_TOT)* fire_frac;
  else if(moist_par <= 0.73) 
    fuel_consum_1hr=(1.10-0.62*moist_par)*(1.0-MINER_TOT)*fire_frac;
  else if(moist_par<1.0) 
    fuel_consum_1hr=(2.45-2.45*moist_par)*(1.0-MINER_TOT)*fire_frac;
  else
    fuel_consum_1hr=0.0;
  /*printf("fuel_consum_1hr= %.5f\n",fuel_consum_1hr);*/
  return fuel_consum_1hr;
} /* of 'fuel_consumption_1hr' */
