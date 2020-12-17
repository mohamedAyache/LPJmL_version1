/***************************************************************************/
/**                                                                       **/
/**     l  i  t  t  e  r  _  a  g  _  s  u  m  _  q  u  i  c  k  .  c     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**     Function computes sum of all above-ground litter pools            **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 09.03.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real litter_ag_sum_quick(const Litter *litter)
{
  int i,l;
  Real sum;
  sum=0;
  for(l=0;l<litter->n;l++)
  {
    sum+=litter->ag[l].trait.leaf;
    for(i=0;i<NFUELCLASS-1;i++)
      sum+=litter->ag[l].trait.wood[i];
  }
  return sum;
} /* of 'litter_ag_sum_quick' */
