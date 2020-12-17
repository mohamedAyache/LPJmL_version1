/***************************************************************************/
/**                                                                       **/
/**                m  o  i  s  t  f  a  c  t  o  r  .  c                  **/
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

Real moistfactor(const Litter *litter  /* Litter pool */
                )                      /* returns moist factor */
{
  Real moistfactor,litter_sum,sum;
  int i,p;
  litter_sum=litter_ag_sum(litter);
  if(litter_sum==0)
    return 0;
  moistfactor=0;
  for(p=0;p<litter->n;p++)
  {
    sum=litter->ag[p].trait.leaf;
    for(i=0;i<NFUELCLASS;i++)
      sum+=litter->ag[p].trait.wood[i];
    moistfactor+=litter->ag[p].pft->flam*sum;
  }
  return moistfactor/litter_sum;
} /* of 'moistfactor' */
