/***************************************************************************/
/**                                                                       **/
/**               l i t t e r _ u p d a t e _ f i r e . c                 **/
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

Real litter_update_fire(Litter *litter,const Fuel *fuel)
{
  int i,j;
  Real fuel_consum;
  /* litter update [gC/m2]*/
  fuel_consum=0;
  for(i=0;i<litter->n;i++)
  {
    fuel_consum+=litter->ag[i].trait.leaf*fuel->deadfuel_consum[0];
    litter->ag[i].trait.leaf*=(1-fuel->deadfuel_consum[0]);
    for(j=0;j<NFUELCLASS;j++)
    {
      fuel_consum+=litter->ag[i].trait.wood[j]*fuel->deadfuel_consum[j];
      litter->ag[i].trait.wood[j]*=(1-fuel->deadfuel_consum[j]);
    }
  }
  return fuel_consum;
} /* of litter_update_fire' */
