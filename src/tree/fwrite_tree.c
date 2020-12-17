/***************************************************************************/
/**                                                                       **/
/**                 f  w  r  i  t  e  _  t  r  e  e  .  c                 **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 27.09.2004                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

Bool fwrite_tree(FILE *file,const Pft *pft)
{
  const Pfttree *tree;
  tree=pft->data;
  fwrite1(&tree->height,sizeof(Real),file);
  fwrite1(&tree->crownarea,sizeof(Real),file);
  fwrite1(&tree->barkthickness,sizeof(Real),file);
  fwrite1(&tree->gddtw,sizeof(Real),file);
  fwrite1(&tree->aphen_raingreen,sizeof(Real),file);
  fwrite1(&tree->isphen,sizeof(int),file);
  fwrite1(&tree->turn.leaf,sizeof(Real),file);
  fwrite1(&tree->turn.sapwood,sizeof(Real),file);
  fwrite1(&tree->turn.root,sizeof(Real),file);
  fwrite1(&tree->ind.leaf,sizeof(Real),file);
  fwrite1(&tree->ind.sapwood,sizeof(Real),file);
  fwrite1(&tree->ind.heartwood,sizeof(Real),file);
  fwrite1(&tree->ind.root,sizeof(Real),file);
  fwrite1(&tree->ind.debt,sizeof(Real),file);

  return FALSE;

/*  return fwrite(tree,sizeof(Pfttree),1,file)!=1;*/
} /* of 'fwrite_tree' */
