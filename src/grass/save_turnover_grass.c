/***************************************************************************/
/**                                                                       **/
/**           t  u  r  n  o  v  e  r  _  g  r  a  s  s  .  c              **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 27.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"

/*
 *
 *  Function reproduction + turnover
 *
 *  Turnover of PFT-specific fraction from each living C pool
 *  Leaf and root C transferred to litter, sapwood C to heartwood
 *
 */

Real turnover_grass(Litter *litter, /* Litter pool */
                    Pft *pft,       /* Pointer to PFT variables */
                    Real fraction   /* fraction of annual turnover (0..1) */
                   )                /* returns turnover (gC/m2) */
{
  Pftgrass *grass;
  const Pftgrasspar *grasspar;
  Real reprod;
  Grassphys gturn;
  grass=pft->data;
  grasspar=getpftpar(pft,data);
  /* reproduction */
  if(pft->bm_inc>0)
  {
    reprod=pft->bm_inc*grasspar->reprod_cost*fraction;
    litter->ag[pft->litter].trait.leaf+=reprod;
    pft->bm_inc     -= reprod;
  }
  /* turnover */
  if (pft->stand->type->landusetype==NATURAL)
  {
    gturn.root=grass->turn.root;
    gturn.leaf=grass->turn.leaf;
    grass->turn.root=grass->turn.leaf=0.0;

  }
  else
  {
    gturn.root=grass->ind.root*grasspar->turnover.root*fraction;
    gturn.leaf=grass->ind.leaf*grasspar->turnover.leaf*fraction;
    litter->ag[pft->litter].trait.leaf+=gturn.leaf*pft->nind;
    update_fbd_grass(litter,pft->par->fuelbulkdensity,gturn.leaf*pft->nind);
    litter->bg[pft->litter]+=gturn.root*pft->nind;
  }
  grass->ind.leaf-= gturn.leaf;
  grass->ind.root-= gturn.root;
  
  return gturn.leaf+gturn.root;
} /* of 'turnover_grass' */
