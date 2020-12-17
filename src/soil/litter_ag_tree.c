/***************************************************************************/
/**                                                                       **/
/**             l  i  t  t  e  r  _  a  g  _  t  r  e  e  .  c            **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function computes sum of all above-ground litter pools for trees  **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 27.08.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real litter_ag_tree(const Litter *litter,int fuel)
{
  int l;
  Real sum;
  sum=0;
  if(fuel==0)
  {
    for(l=0;l<litter->n;l++)
      if(litter->ag[l].pft->type==TREE)
        sum+=litter->ag[l].trait.leaf+litter->ag[l].trait.wood[0];
  }
  else
  {
    for(l=0;l<litter->n;l++)
      if(litter->ag[l].pft->type==TREE)
        sum+=litter->ag[l].trait.wood[fuel];
  }
  return sum;
} /* of litter_ag_tree */
