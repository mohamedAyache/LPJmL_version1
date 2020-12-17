/***************************************************************************/
/**                                                                       **/
/**      c  o  p  p  i  c  e  _  t  r  e  e  .  c                         **/
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

#define HARVEST_EFFICIENCY 0.9
/* Sapwood reduction needs to be lower to avoid fatal numerical instabilities in allometry */
/* Makes sense as part (~30 %) of sapwood carbon is located underground */
#define HARVEST_EFFICIENCY_SAP 0.65


Real coppice_tree(Pft *pft)
{
  Pfttree *tree;
  const Pfttreepar *treepar;
  Real harvest=0.0;
  Real leaf_old;
  tree=pft->data;
  treepar=getpftpar(pft,data);

  harvest=(((tree->ind.sapwood*HARVEST_EFFICIENCY_SAP+tree->ind.heartwood*HARVEST_EFFICIENCY)-tree->ind.debt)*pft->nind);
  leaf_old=tree->ind.leaf;
 /*printf("%d biomass harvest is %.2f, %.2f  tC/ha/yr or %.2f tDM/ha/yr\n",*/
/*  printf("coppice %d %.2f %.2f %.2f %.2f %.2f\n",
    year,harvest/100,harvest/100/treepar->rotation,harvest/100*2/treepar->rotation,
    (tree->ind.heartwood+tree->ind.sapwood+tree->ind.leaf+tree->ind.root)*pft->nind/100,
    (tree->ind.heartwood+(tree->ind.sapwood*0.7)+tree->ind.leaf)*pft->nind/100);*/
  tree->ind.heartwood*=1-HARVEST_EFFICIENCY;
  tree->ind.sapwood*=1-HARVEST_EFFICIENCY_SAP;
  tree->ind.debt=0;
  tree->ind.leaf=8000*tree->ind.sapwood/(wooddens*tree->height*pft->par->sla);
  harvest+=(leaf_old-tree->ind.leaf)*pft->nind;
  /* Call allometry to adjust height and crownarea */
  allometry_tree(pft);
  fpc_tree(pft);

  return harvest;
} /* of 'coppice_tree' */

/* Simulate coppicing of SRWC tree
 * put all leafmass into litter
 * put 90 % of heart- and sapwood into harvest
 * fine roots remain unchanged
 * sapwood and heartwood are reduced by 90 % */
