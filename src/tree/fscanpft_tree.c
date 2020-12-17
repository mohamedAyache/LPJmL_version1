/***************************************************************************/
/**                                                                       **/
/**               f  s  c  a  n  p  f  t  _  t  r  e  e  .  c             **/
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
#include "tree.h"

#define fscanreal2(isout,file,var,pft,name) \
  if(fscanreal(file,var,name,isout)) \
  { \
    if(isout)\
    fprintf(stderr,"ERROR110: Cannot read PFT '%s' in %s().\n",pft,__FUNCTION__); \
    return TRUE; \
  }
#define fscanint2(isout,file,var,pft,name) \
  if(fscanint(file,var,name,isout)) \
  { \
    if(isout)\
    fprintf(stderr,"ERROR110: Cannot read PFT '%s' in %s().\n",pft,__FUNCTION__); \
    return TRUE; \
  }
#define fscantreephys2(isout,file,var,pft,name)\
  if(fscantreephys(file,var,isout))\
  {\
    if(isout)\
    fprintf(stderr,"ERROR111: Cannot read '%s' of PFT '%s' in %s().\n",name,pft,__FUNCTION__); \
    return TRUE; \
  }

static Bool fscantreephys(FILE *file,Treephys *phys,
                          Bool isout)
{
  if(fscanreal(file,&phys->leaf,"leaf",isout))
    return TRUE;
  if(fscanreal(file,&phys->sapwood,"sapwood",isout))
    return TRUE;
  if(fscanreal(file,&phys->root,"root",isout))
    return TRUE;
  if(phys->leaf<=0 || phys->sapwood<=0 || phys->root<=0)
    return TRUE;
  return FALSE;
} /* of 'fscantreephys' */

Bool fscanpft_tree(FILE *file,           /* file pointer */
                   Pftpar *pft,          /* Pointer to Pftpar array */
                   Bool isout            /* error output is generated */
                  )                      /* returns  FALSE for success */
{
  int i;
  Real stemdiam,height_sapl,wood_sapl;
  Pfttreepar *tree;
  pft->newpft=new_tree;
  pft->npp=npp_tree;
  /*pft->fpc=fpc_tree; */
  pft->fpar=fpar_tree;
  pft->alphaa_manage=alphaa_tree;
  pft->leaf_phenology=phenology_tree;
  pft->fwrite=fwrite_tree;
  pft->mix_veg=mix_veg_tree;
  pft->fprint=fprint_tree;
  pft->fread=fread_tree;
  pft->litter_update=litter_update_tree;
  pft->annual=annual_tree;
  pft->establishment=establishment_tree;
  pft->init=init_tree;
  pft->fire=fire_tree;
  pft->actual_lai=actual_lai_tree;
  pft->free=free_tree;
  pft->vegc_sum=vegc_sum_tree;
  pft->adjust=adjust_tree;
  pft->reduce=reduce_tree;
  pft->fprintpar=fprintpar_tree;
  pft->livefuel_consumption=livefuel_consum_tree;
  pft->turnover_monthly=turnover_monthly_tree;
  pft->albedo=albedo_tree;
  tree=new(Pfttreepar);
  check(tree);
  pft->data=tree;
  fscanint2(isout,file,&tree->leaftype,pft->name,"leaftype");
  fscantreephys2(isout,file,&tree->turnover,pft->name,"turnover");
  tree->turnover.root=1.0/tree->turnover.root;
  tree->turnover.sapwood=1.0/tree->turnover.sapwood;
  tree->turnover.leaf=1.0/tree->turnover.leaf;
  fscantreephys2(isout,file,&tree->cn_ratio,pft->name,"cn_ratio");
  tree->cn_ratio.sapwood=pft->respcoeff*param.k/tree->cn_ratio.sapwood;
  tree->cn_ratio.root=pft->respcoeff*param.k/tree->cn_ratio.root;
  fscanreal2(isout,file,&tree->crownarea_max,pft->name,"crownarea_max");
  fscanreal2(isout,file,&wood_sapl,pft->name,"wood_sapl");
  if(pft->phenology==SUMMERGREEN)
  {
    fscanreal2(isout,file,&tree->aphen_min,pft->name,"aphen_min");
    fscanreal2(isout,file,&tree->aphen_max,pft->name,"aphen_max");
  }
  fscanreal2(isout,file,&tree->reprod_cost,pft->name,"reprod_cost");
  fscanreal2(isout,file,&tree->allom1,pft->name,"allom1");
  fscanreal2(isout,file,&tree->allom2,pft->name,"allom2");
  fscanreal2(isout,file,&tree->allom3,pft->name,"allom3");
  fscanreal2(isout,file,&tree->allom4,pft->name,"allom4");
  fscanreal2(isout,file,&tree->height_max,pft->name,"height_max");
  fscanreal2(isout,file,&tree->scorchheight_f_param,pft->name,"scorchheight_f_param"); /* benp */
  fscanreal2(isout,file,&tree->crownlength,pft->name,"crownlength"); /* benp */
  fscanreal2(isout,file,&tree->barkthick_par1,pft->name,"barkthick_par1"); /* benp */
  fscanreal2(isout,file,&tree->barkthick_par2,pft->name,"barkthick_par2"); /* benp */
  fscanreal2(isout,file,&tree->crown_mort_rck,pft->name,"crown_mort_rck"); /* benp */
  fscanreal2(isout,file,&tree->crown_mort_p,pft->name,"crown_mort_p"); /* benp */
  for(i=0;i<NFUELCLASS;i++)
    fscanreal2(isout,file,tree->fuelfrac+i,pft->name,"fuel fraction");
  fscanreal2(isout,file,&tree->k_est,pft->name,"k_est");
  fscanint2(isout,file,&tree->rotation,pft->name,"rotation");
  fscanint2(isout,file,&tree->max_rotation_length,pft->name,"max_rotation_length");
  fscanreal2(isout,file,&tree->harvest_ratio,pft->name,"harvest ratio");
  fscanint2(isout,file,&tree->with_grass,pft->name,"with_grass");
  tree->sapl.leaf=pow(pft->lai_sapl*tree->allom1*pow(wood_sapl,reinickerp)*
                  pow(4*pft->sla/M_PI/k_latosa,reinickerp*0.5)/pft->sla,
                  2/(2-reinickerp));
  stemdiam=wood_sapl*sqrt(4*tree->sapl.leaf*pft->sla/M_PI/k_latosa);
  height_sapl=tree->allom2*pow(stemdiam,tree->allom3);
  tree->sapl.sapwood=wooddens*height_sapl*tree->sapl.leaf*pft->sla/k_latosa;
  tree->sapl.heartwood=(wood_sapl-1)*tree->sapl.sapwood;
  tree->sapl.root=(1.0/pft->lmro_ratio)*tree->sapl.leaf;
  tree->sapling_C=phys_sum_tree(tree->sapl)*tree->k_est;

  return FALSE;
} /* of 'fscanpft_tree' */
