/***************************************************************************/
/**                                                                       **/
/**                m  o  r  t  a  l  i  t  y  _  t  r  e  e  .  c         **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 28.09.2004                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

/*#define MORT_MAX 0.03  asymptotic maximum mortality rate (1/year) */
/*#define k_mort 0.5  coefficient of growth efficiency in mortality equation */
#define ramp_gddtw 400.0 /* ramp for heat damage function. Above 200      */
                         /* growing degree days above the upper limit tw, */
                         /* establishment is zero and mortality 100%      */

/*
 *
 *     Function mortality
 *
 *     Tree background and stress mortality
 *
 */

Bool mortality_tree(Litter *litter,   /* Litter                              */
                    Pft *pft,         /* Pointer to pft                      */
                    Real turnover_ind,/* indivudual turnover                 */
                    Real mtemp_max    /* maximum temperature of month (deg C)*/
                   )
{
  Real mort,bm_delta,heatstress,nind_kill,mort_max;
  Pfttree *tree;
  tree=pft->data;
  bm_delta=pft->bm_inc/pft->nind-turnover_ind;
  if(bm_delta<0)
   bm_delta=0;
  if (pft->par->cultivation_type==BIOMASS || pft->par->cultivation_type==ANNUAL_TREE)
	  mort_max=0.005;
  else
	  mort_max=param.k_mort_max;

  mort=mort_max/(1+param.k_mort*bm_delta/tree->ind.leaf/pft->par->sla);
  if(mtemp_max>pft->par->twmax)
  {
    heatstress=tree->gddtw/ramp_gddtw;
    if(heatstress>1)
      heatstress=1;
    mort+=heatstress;
  }
  nind_kill=(mort>1) ? pft->nind : pft->nind*mort;
  pft->nind-=nind_kill;
  litter_update_tree(litter,pft,nind_kill);
  fpc_tree(pft);
  return isneg_tree(pft);
  /**fpc_inc+=fpc_tree(pft);*/
} /* of 'mortality_tree' */
