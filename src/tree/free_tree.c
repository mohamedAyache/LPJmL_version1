/***************************************************************************/
/**                                                                       **/
/**              f  r  e  e  _  t  r  e  e  .  c                          **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**     Function deallocates tree-specific variables                      **/
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

void free_tree(Pft *pft)
{
    Pfttree *tree;
    tree=pft->data;
    pft->stand->soil.litter.ag[pft->litter].trait.leaf+=tree->turn.leaf*pft->nind-tree->turn_litt.leaf;

    tree->residue.leaf += tree->turn.leaf*pft->nind-tree->turn_litt.leaf;

    update_fbd_tree(&pft->stand->soil.litter,pft->par->fuelbulkdensity,tree->turn.leaf*pft->nind-tree->turn_litt.leaf,0);
    pft->stand->soil.litter.bg[pft->litter]+=tree->turn.root*pft->nind-tree->turn_litt.root;

    tree->residue.root += tree->turn.root*pft->nind-tree->turn_litt.root;

    free(pft->data);
} /* of 'free_tree' */
