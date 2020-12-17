/***************************************************************************/
/**                                                                       **/
/**                    n  p  p  _  t  r  e  e  .  c                       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 11.06.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

Real npp_tree(Pft *pft, /* PFT variables */
              Real gtemp_air, /* value of air temperature response function */
              Real gtemp_soil, /* value of soil temperature response function */
              Real assim  /* assimilation (gC/m2) */
             ) /* returns net primary productivity (gC/m2) */
{
  Pfttree *tree;
  const Pfttreepar *par;
  Real mresp,npp; 
  tree=pft->data;
  par=pft->par->data;
  mresp=pft->nind*(tree->ind.sapwood*par->cn_ratio.sapwood*gtemp_air+
                   tree->ind.root*par->cn_ratio.root*gtemp_soil*pft->phen);
  npp=(assim<mresp) ? assim-mresp : (assim-mresp)*(1-param.r_growth);
  pft->bm_inc+=npp;
  return npp;
} /* of 'npp_tree' */
