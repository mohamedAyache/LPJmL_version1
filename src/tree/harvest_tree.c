/***************************************************************************/
/**                                                                       **/
/**           h  a  r  v  e  s  t  _  t  r  e  e  .  c                    **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

Real harvest_tree(Pft *pft)
{
  Real harvest;
  Pfttree *tree;
  tree=pft->data;
  harvest=tree->c_fruit;
  //printf("harvest(%s)=%g\n",pft->par->name,harvest);
  pft->bm_inc-=harvest;
  return harvest;
} /* of 'harvest_tree' */
