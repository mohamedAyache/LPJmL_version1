/***************************************************************************/
/**                                                                       **/
/**                       n  e  w  _  t  r  e  e  .  c                    **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**     Function allocates and initializes tree-specific variables        **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-11-04 15:41:03 +0100 (mar. 04 nov. 20#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

void new_tree(Pft *pft /* Parameter of pft */
             )         
{
  Pfttree *tree;
  tree=new(Pfttree);
  check(tree);
  pft->data=tree;
  init_tree(pft);
/*  tree->leafon=FALSE;*/
  /* pft->bm_inc=pft->wscal_mean=pft->phen=0;
     tree->gddtw=tree->aphen_raingreen=0;*/
/* tree->leafondays=tree->leafoffdays=0;*/
   tree->ind.root=tree->ind.sapwood=tree->ind.heartwood=tree->ind.leaf=0.0;
   tree->turn.root=tree->turn.sapwood=tree->turn.leaf=tree->turn_litt.leaf=tree->turn_litt.root=0.0;
   tree->ind.debt=tree->gddtw=tree->aphen_raingreen=0.0;
   tree->height=tree->crownarea=0.0;
   tree->isphen=FALSE;
   pft->flaimax=0;
} /* of 'new_tree' */
