/***************************************************************************/
/**                                                                       **/
/**                l  i  t  t  e  r  _  a  g  _  g  r  a  s  s  .  c      **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**     Function computes sum of all above-ground litter pools for grass  **/
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

Real litter_ag_grass(const Litter *litter)
{
  int l;
  Real sum;
  sum=0;
  for(l=0;l<litter->n;l++)
    if(litter->ag[l].pft->type==GRASS || litter->ag[l].pft->type==CROP)
      sum+=litter->ag[l].trait.leaf;
  return sum;
} /* of 'litter_ag_grass' */
