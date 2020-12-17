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
/**     Last change: $Date:: 2014-11-04 15:41:03 +0100 (mar., 04 nov. 2#$ **/
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

    tree->ind.root=tree->ind.sapwood=tree->ind.heartwood=tree->ind.leaf=0.0;
    tree->turn.root=tree->turn.sapwood=tree->turn.leaf=tree->turn_litt.leaf=tree->turn_litt.root=0.0;
    tree->ind.debt=tree->gddtw=tree->aphen_raingreen=0.0;
    tree->height=tree->crownarea=0.0;
    tree->isphen=FALSE;
    pft->flaimax=0;

    tree->uptake.root=tree->uptake.sapwood=tree->uptake.heartwood=tree->uptake.leaf=0.0;
    tree->annual_uptake.root=tree->annual_uptake.sapwood=tree->annual_uptake.heartwood=tree->annual_uptake.leaf=0.0;
    tree->residue.root=tree->residue.sapwood=tree->residue.heartwood=tree->residue.leaf=0.0;


} /* of 'new_tree' */
