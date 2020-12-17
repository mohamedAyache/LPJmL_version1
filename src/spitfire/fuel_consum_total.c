/***************************************************************************/
/**                                                                       **/
/**       f  u  e  l  _  c  o  n  s  u  m  _  t  o  t  a  l  .  c         **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Kirsten Thonicke                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 10.03.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real fuel_consum_total(const Litter *litter,const Fuel *fuel)
{
  int i,l;
  Real fuel_consum;
  fuel_consum=0;
  for(l=0;l<litter->n;l++)
  {
    fuel_consum += fuel->deadfuel_consum[0]*litter->ag[l].trait.leaf;
    for (i=0;i<NFUELCLASS;i++)
      fuel_consum += fuel->deadfuel_consum[i]*litter->ag[l].trait.wood[i];
  }
  return fuel_consum;
} /* of 'fuel_consum_total' */ 
