/***************************************************************************/
/**                                                                       **/
/**     m  i  x  _  v  e  g  _  t  r  e  e  .  c                          **/
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

void mix_veg_tree(Pft *pft,Real scaler)
{
  Pfttree *tree;
  tree=pft->data;
  pft->nind*=scaler;
  tree->turn_litt.leaf*=scaler;
} /* of 'mix_veg_tree' */
