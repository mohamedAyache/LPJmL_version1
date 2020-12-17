/***************************************************************************/
/**                                                                       **/
/**     l  i  t  t  e  r  _  u  p  d  a  t  e  _  t  r   e  e  .  c       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-10-28 10:14:19 +0100 (mer., 28 oct. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

void litter_update_tree(Litter *litter, /* Litter pool */
                        Pft *pft,     /* PFT variables */
                        Real frac
                       )
{
  int i;
  const Pfttreepar *treepar;
  Pfttree *tree;
  tree=pft->data;
  treepar=pft->par->data;

  litter->ag[pft->litter].trait.leaf+=tree->ind.leaf*frac;
  for(i=0;i<NFUELCLASS;i++)
  {
    litter->ag[pft->litter].trait.wood[i]+=(tree->ind.sapwood+tree->ind.heartwood-
                                  tree->ind.debt)*frac*treepar->fuelfrac[i];
    update_fbd_tree(litter,pft->par->fuelbulkdensity,
                    (tree->ind.sapwood+tree->ind.heartwood-tree->ind.debt)
               *frac*treepar->fuelfrac[i],i);
  }
  litter->bg[pft->litter]+=tree->ind.root*frac;

} /* of 'litter_update_tree' */
