/***************************************************************************/
/**                                                                       **/
/**               a  d  j  u  s  t  _  t  r  e  e  .  c                   **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 04.08.2010                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

void adjust_tree(Litter *litter,Pft *pft,Real tree_fpc)
{

  Real frac,fpc_end,nind_old;
  int i;

  if(tree_fpc>FPC_TREE_MAX)
  {
    fpc_tree(pft);
    fpc_end=pft->fpc-(tree_fpc-FPC_TREE_MAX)*pft->fpc/tree_fpc;
    nind_old=pft->nind;
    for(i=0; fpc_end<pft->fpc && i<40;i++)
    {
      frac=fpc_end/pft->fpc;
      pft->nind*=frac;
      fpc_tree(pft);
    }
    litter_update_tree(litter,pft,nind_old-pft->nind);
  }
} /* of 'adjust_tree' */

/***************************************************************************/
/**                                                                       **/
/** CALLED FROM:                                                          **/
/**   establishment.c (foreachpft)                                        **/
/** PURPOSE:                                                              **/
/**   reduce nind and fpc if total fpc for all trees exceeds              **/
/**   0.95 (FPC_TREE_MAX)                                                 **/
/** DETAILED DESCRIPTION:                                                 **/
/**   litter_update_tree is used to transfer the "waste" carbon into the  **/
/**   litter pool in order to the maintain carbon balance                 **/
/**                                                                       **/
/***************************************************************************/
