/***************************************************************************/
/**                                                                       **/
/**        t u r n o v e r _ m o n t h l y _ t r e e . c                  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 21.03.2012                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

void turnover_monthly_tree(Litter *litter,Pft *pft)
{
  Pfttree *tree;
  Pfttreepar *treepar;  
  treepar=pft->par->data;
  tree=pft->data;
  tree->turn.root+=tree->ind.root*treepar->turnover.root/NMONTH;
  tree->turn_litt.root+=tree->ind.root*treepar->turnover.root/NMONTH*pft->nind;
  litter->bg[pft->litter]+=tree->ind.root*treepar->turnover.root/NMONTH*pft->nind;
} /* of 'turnover_monthly_tree' */
