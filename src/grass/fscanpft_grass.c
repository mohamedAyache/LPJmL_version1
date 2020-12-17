/***************************************************************************/
/**                                                                       **/
/**               f  s  c  a  n  p  f  t  _  g  r  a  s  s  .  c          **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 26.02.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"

#define fscanreal2(isout,file,var,pft,name) \
  if(fscanreal(file,var,name,isout)) \
  { \
    if(isout)\
    fprintf(stderr,"ERROR110: Cannot read PFT '%s' in %s().\n",pft,__FUNCTION__); \
    return TRUE; \
  }

#define fscangrassphys2(isout,file,var,pft,name) \
  if(fscangrassphys(file,var,isout))\
  { \
    if(isout)\
    fprintf(stderr,"ERROR111: Cannot read '%s' of PFT '%s' in %s().\n",name,pft,__FUNCTION__); \
    return TRUE; \
  }


static Bool fscangrassphys(FILE *file,Grassphys *phys,Bool isout)
{
  if(fscanreal(file,&phys->leaf,"leaf",isout))
    return TRUE;
  if(fscanreal(file,&phys->root,"root",isout))
    return TRUE;
  if(phys->leaf<=0 ||  phys->root<=0)
    return TRUE;
  return FALSE;
} /* of 'fscangrassphys' */

Bool fscanpft_grass(FILE *file,          /* file pointer */
                    Pftpar *pft,         /* Pointer to Pftpar array */
                    Bool isout
                   )                     /* returns FALSE for success  */
{
  Pftgrasspar *grass;
  pft->newpft=new_grass;
  pft->npp=npp_grass;
  /*pft->fpc=fpc_grass; */
  pft->fpar=fpar_grass;
  pft->alphaa_manage=alphaa_grass;
  pft->leaf_phenology=phenology_grass;
  pft->fwrite=fwrite_grass;
  pft->mix_veg=mix_veg_grass;
  pft->fire=fire_grass;
  pft->fprint=fprint_grass;
  pft->fread=fread_grass;
  pft->litter_update=litter_update_grass;
  pft->annual=annual_grass;
  pft->establishment=establishment_grass;
  pft->reduce=reduce_grass;
  pft->actual_lai=actual_lai_grass;
  pft->init=init_grass;
  pft->free=free_grass;
  pft->vegc_sum=vegc_sum_grass;
  pft->fprintpar=fprintpar_grass;
  pft->livefuel_consumption=livefuel_consum_grass;
  pft->turnover_monthly=turnover_monthly_grass;
  pft->albedo=albedo_grass;
  grass=new(Pftgrasspar);
  check(grass);
  pft->data=grass;
  fscangrassphys2(isout,file,&grass->turnover,pft->name,"turnover");
  grass->turnover.leaf=1.0/grass->turnover.leaf;
  grass->turnover.root=1.0/grass->turnover.root;
  fscangrassphys2(isout,file,&grass->cn_ratio,pft->name,"cn_ratio");
  fscanreal2(isout,file,&grass->reprod_cost,pft->name,"reprod_cost");
  grass->cn_ratio.leaf=pft->respcoeff*param.k/grass->cn_ratio.leaf;
  grass->cn_ratio.root=pft->respcoeff*param.k/grass->cn_ratio.root;
  grass->sapl.leaf=pft->lai_sapl/pft->sla;
  grass->sapl.root=(1.0/pft->lmro_ratio)*grass->sapl.leaf;
  grass->sapling_C=phys_sum_grass(grass->sapl);

  return FALSE;
} /* of 'fscanpft_grass' */
