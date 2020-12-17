/***************************************************************************/
/**                                                                       **/
/**                       l  a  i  _  t  r  e  e  .  c                    **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 11.06.2004                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

Real lai_tree(const Pft *pft)
{
  Pfttree *tree;
  tree=pft->data;
    
  return (tree->crownarea>0.0) ? 
         tree->ind.leaf*getpftpar(pft,sla)/tree->crownarea : 0;
} /* of 'lai_tree' */
/*
- this function is called in fpc_tree() and returns the maximum lai of a tree (last year's LAI)
*/

Real actual_lai_tree(const Pft *pft)
{
  Pfttree *tree;
  tree=pft->data;
    
  return (tree->crownarea>0.0) ? 
    tree->ind.leaf*getpftpar(pft,sla)/tree->crownarea*pft->phen : 0;
} /* of 'lai_tree' */
/*
- this function is called in interception() and returns the actual lai of a tree (multiplied with phen)
*/
