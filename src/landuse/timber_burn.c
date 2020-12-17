/***************************************************************************/
/**                                                                       **/
/**             t  i  m  b  e  r  _  b  u  r  n  .  c                     **/
/**                                                                       **/
/**     extension of LPJ to compute timber harvest / burning              **/
/**                                                                       **/
/**     written by Christoph Mueller                                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 19.08.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

Real timber_burn(const Pft *pft, /* Pointer to tree PFT */
                 Real fburnt,    /* fraction burnt (0..1) */
                 Litter *litter,  /* Litter pools */
                 Real nind       /* cannot use pft->nind, since pft is on different stand */
                )                /* returns fire emissions (gC/m2) */
{
  int i;
  const Pfttree *tree;
  const Pfttreepar *treepar;
  tree=pft->data;
  treepar=pft->par->data;
  if(fburnt<epsilon || nind<epsilon)
    return 0;

  /* reducing carbon in litter pool because this carbon is added to
   * the litter pool in update_litter (next function in reclaim_land().
   * We can't substract it from the vegetation carbon as there
   * is no vegetation on stand2.*/
  for(i=0;i<NFUELCLASS;i++)
    litter->ag[pft->litter].trait.wood[i]-=(tree->ind.heartwood+tree->ind.sapwood*2.0/3.0)*fburnt*nind*treepar->fuelfrac[i];

  /* computing deforestation fire emissions */
  return (tree->ind.heartwood+tree->ind.sapwood*2.0/3.0)*fburnt*nind;

} /* of 'timber_burn' */
