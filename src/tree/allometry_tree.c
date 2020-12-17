/***************************************************************************/
/**                                                                       **/
/**                 a  l  l  o  m  e  t  r  y  _  t  r  e  e  .  c        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 10.03.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

/*
 *  ALLOMETRY
 *  Should be called to update allometry, FPC and FPC increment whenever 
 *  biomass values for a vegetation individual change.
 */

void allometry_tree(Pft *pft /* Pointer to tree PFT */
                   )
{
  Pfttree *tree;
  const Pfttreepar *treepar;
  Real allometry,sm_ind_temp,stemdiam; 

  tree=pft->data;
  treepar=getpftpar(pft,data);
  tree->height=(tree->ind.sapwood<=0.0 || tree->ind.leaf<=0.0) ? 0 : 
               k_latosa*tree->ind.sapwood/(tree->ind.leaf*pft->par->sla* wooddens);

  if(tree->height>treepar->height_max)
  {
    tree->height=treepar->height_max;
    sm_ind_temp=tree->ind.sapwood;
    tree->ind.sapwood=tree->ind.leaf*treepar->height_max*wooddens*pft->par->sla/
                      k_latosa;

    /* barrier.n: update of uptake for sapwood when respecting
     * allometry */
    tree->uptake.sapwood -= sm_ind_temp-tree->ind.sapwood;

    tree->ind.heartwood+=sm_ind_temp-tree->ind.sapwood;
  } 
  allometry=treepar->allom1*pow(tree->height/treepar->allom2,reinickerp/treepar->allom3);
  /* bark thickness for fire mortality [cm] */
  stemdiam=pow(tree->height/treepar->allom2,1.0/treepar->allom3);
  tree->barkthickness=treepar->barkthick_par1*(stemdiam*100.0)+treepar->barkthick_par2;
  tree->crownarea=min(allometry,treepar->crownarea_max);
} /* of 'allometry_tree' */ 
