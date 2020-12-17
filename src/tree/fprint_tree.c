/***************************************************************************/
/**                                                                       **/
/**                 f  p  r  i  n  t  _  t  r  e  e  .  c                 **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function prints tree-specific state variables in ASCII file       **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-09-29 16:02:37 +0200 (lun., 29 sept. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

void fprint_tree(FILE *file,const Pft *pft)
{
  Pfttree *tree;
  tree=pft->data;
  fprintf(file,"Height:\t\t%g (m)\n"
               "Crown area:\t%g (m2)\n"
               "Bark thickness:\t%g (cm)\n"
               "Gddtw:\t\t%g\n",
               tree->height,tree->crownarea,tree->barkthickness,
               tree->gddtw);
  if(pft->par->phenology==RAINGREEN)
    fprintf(file,"Aphen raingreen:\t%g\n",tree->aphen_raingreen);
  fprintf(file,"Cmass:\t\t");
  fprinttreephys2(file,tree->ind,pft->nind);
  fprintf(file,"\n");
} /* of 'fprint_tree' */
