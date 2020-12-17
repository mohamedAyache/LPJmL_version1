/***************************************************************************/
/**                                                                       **/
/**                l  i  t  t  e  r  s  u  m  .  c                        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**     Function computes sum of all litter pools                         **/
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

Real littersum(const Litter *litter)
{
  int i,l;
  Real sum;
  sum=0;
  for(l=0;l<litter->n;l++)
  {
    sum+=litter->ag[l].trait.leaf;
    for(i=0;i<NFUELCLASS;i++)
      sum+=litter->ag[l].trait.wood[i];
    sum+=litter->bg[l];
  }
  return sum;
} /* of 'littersum' */
