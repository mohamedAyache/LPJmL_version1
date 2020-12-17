/***************************************************************************/
/**                                                                       **/
/**          a n n u a l _ a g r i c u l t u r e _ t r e e . c            **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function performs necessary updates after iteration over one      **/
/**     year for agriculture tree stand                                   **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     See file AUTHORS for list of authors contributing to this code    **/
/**                                                                       **/
/**     Last change: $Date:: 2016-02-11 11:10:20 +0100 (jeu., 11 févr.#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"
#include "natural.h"
#include "agriculture.h"
#include "agriculture_tree.h"

Bool annual_agriculture_tree(Stand *stand,         /* Pointer to stand */
                             int npft,             /* number of natural pfts */
                             int ncft,             /* number of crop PFTs */
                             Real UNUSED(popdens), /* population density (capita/km2) */
                             int UNUSED(year),     /* year (AD) */
                             Bool intercrop,
                             const Config *config
                            )                      /* stand has to be killed (TRUE/FALSE) */
{
  int p,pft_len,t;
  Bool *present,isdead;
  int *n_est;
  Pft *pft;
  Real *fpc_inc,*fpc_inc2,*fpc_type;
  Real fpc_total;
  Real acflux_estab=0;
  Real estab_store=0;
  Real yield=0.0;
  Pfttreepar *treepar;
  Irrigation *irrigation;
  Real k_est_thiscell, sapling_C_thiscell;
  irrigation=stand->data;

  isdead=FALSE;

  n_est=newvec(int,config->ntypes);
  check(n_est);
  fpc_type=newvec(Real,config->ntypes);
  check(fpc_type);
  present=newvec(Bool,npft);
  check(present);

  for(p=0;p<npft;p++)
    present[p]=FALSE;
  for(t=0;t<config->ntypes;t++)
    n_est[t]=0;
  pft_len=getnpft(&stand->pftlist); /* get number of established PFTs */
  if(pft_len>0)
  {
    fpc_inc=newvec(Real,pft_len);
    check(fpc_inc);

    foreachpft(pft,p,&stand->pftlist)
    {
#ifdef DEBUG2
      printf("PFT:%s fpc_inc=%g fpc=%g\n",pft->par->name,fpc_inc[p],pft->fpc);
      printf("PFT:%s bm_inc=%g vegc=%g soil=%g\n",pft->par->name,
             pft->bm_inc,vegc_sum(pft),soilcarbon(&stand->soil));
#endif
      //printf("PFT=%s, fpc=%g\n",pft->par->name,pft->fpc); 
      if(istree(pft))
      {
        treepar=pft->par->data;
        /*Allocating cell specified k_est and sapling_C*/        
        if(pft->stand->cell->ml.manage.k_est[pft->par->id]>0)
          k_est_thiscell=pft->stand->cell->ml.manage.k_est[pft->par->id];
        else
          k_est_thiscell=treepar->k_est;
        sapling_C_thiscell=phys_sum_tree(treepar->sapl)*k_est_thiscell;
       
        if (stand->cell->balance.estab_storage_tree[irrigation->irrigation]<sapling_C_thiscell*stand->frac && pft->bm_inc>100)
        {
          estab_store=pft->bm_inc*0.03;
          pft->bm_inc-=estab_store;
          stand->cell->balance.estab_storage_tree[irrigation->irrigation]+=estab_store*stand->frac;
        }
        if(stand->age>treepar->rotation)
        {
          yield=harvest_tree(pft);
          //printf("%s yield %s=%g t/ha, %g indiv/ha, wstress=%g, fpc=%g\n",(irrigation->irrigation) ? "irrigated" :"",pft->par->name,yield*1e4/1e6/0.45,pft->nind*1e4,pft->wscal_mean/365,pft->fpc);
          //printf("index=%d, yield=%g\n",agtree(ncft)+stand->pft_id-npft+nagtree+irrigation->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree),yield);
          if(config->pft_output_scaled)
            stand->cell->output.pft_harvest[agtree(ncft)+stand->pft_id-npft+config->nagtree+irrigation->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)].harvest+=yield*stand->frac;
          else
            stand->cell->output.pft_harvest[agtree(ncft)+stand->pft_id-npft+config->nagtree+irrigation->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)].harvest+=yield;
          stand->cell->balance.biomass_yield+=yield*stand->frac;
          stand->cell->output.dcflux+=yield*stand->frac;
        }
      }

      if(annualpft(stand,pft,fpc_inc+p))
      {
        /* PFT killed, delete from list of established PFTs */
        fpc_inc[p]=fpc_inc[getnpft(&stand->pftlist)-1];
        litter_update(&stand->soil.litter,pft,pft->nind);
        pft->nind=0;
        delpft(&stand->pftlist,p);
        p--; /* adjust loop variable */ 
      }
        
    } /* of foreachpft */
#ifdef DEBUG2
    printf("Number of updated pft: %d\n",stand->pftlist.n);
#endif

    light(stand,config->ntypes,fpc_inc);
    free(fpc_inc);
  } /* END if(pft_len>0) */

  /*
  if(setting.fire==FIRE)
  {
    fire_frac=fire_prob(&stand->soil.litter,stand->fire_sum);
    stand->cell->output.firef+=1.0/fire_frac;
    stand->cell->output.firec+=firepft(&stand->soil.litter,
                                       &stand->pftlist,fire_frac)*stand->frac;
  }
  */

  foreachpft(pft,p,&stand->pftlist)
    present[pft->par->id]=TRUE;
  treepar=config->pftpar[stand->pft_id].data;
  for(p=0;p<npft;p++)
  {
    if(establish(stand->cell->gdd[p],config->pftpar+p,&stand->cell->climbuf) &&
       (config->pftpar[p].id==stand->pft_id ||
        (treepar->with_grass && config->pftpar[p].type==GRASS && config->pftpar[p].cultivation_type==NONE)))
    {
      if(!present[p])
        addpft(stand,config->pftpar+p);
      n_est[config->pftpar[p].type]++;
    }
  }
  fpc_total=fpc_sum(fpc_type,config->ntypes,&stand->pftlist);
  pft_len=getnpft(&stand->pftlist);
  if(pft_len>0){
    fpc_inc2=newvec(Real,pft_len);
    check(fpc_inc2);
  }
  foreachpft(pft,p,&stand->pftlist) fpc_inc2[p]=0;

  foreachpft(pft,p,&stand->pftlist)
    if(establish(stand->cell->gdd[pft->par->id],pft->par,&stand->cell->climbuf))
      if (istree(pft))
      {
        treepar=pft->par->data;
        k_est_thiscell=stand->cell->ml.manage.k_est[pft->par->id];
        if(k_est_thiscell<0)
          k_est_thiscell=treepar->k_est;
        if(pft->nind<k_est_thiscell)
        {
          acflux_estab+=establishment(pft,fpc_total,fpc_type[pft->par->type],
                                        n_est[pft->par->type]);
          fpc_inc2[p]=pft->fpc+fpc_total-1;
        }
      }
      else
        acflux_estab+=establishment(pft,fpc_total,fpc_type[pft->par->type],
                                  n_est[pft->par->type]);
  /*fpc_total=fpc_sum(fpc_type,config->ntypes,&stand->pftlist);
  foreachpft(pft,p,&stand->pftlist)
    adjust(&stand->soil.litter,pft,fpc_type[pft->par->type]);
  fpc_total=fpc_sum(fpc_type,config->ntypes,&stand->pftlist);
  if (fpc_total>1.0)
    foreachpft(pft,p,&stand->pftlist)
      reduce(&stand->soil.litter,pft,fpc_total); */

  fpc_total=fpc_sum(fpc_type,config->ntypes,&stand->pftlist);
  if(fpc_total>1.0) light(stand,config->ntypes,fpc_inc2);
  fpc_total=fpc_sum(fpc_type,config->ntypes,&stand->pftlist);
  if (fpc_total>1.0)
     foreachpft(pft,p,&stand->pftlist)
       adjust(&stand->soil.litter,pft,fpc_type[pft->par->type]);
  fpc_total=fpc_sum(fpc_type,config->ntypes,&stand->pftlist);
  if (fpc_total>1.0)
    foreachpft(pft,p,&stand->pftlist)
      reduce(&stand->soil.litter,pft,fpc_total);
  stand->cell->balance.estab_storage_tree[irrigation->irrigation]-=acflux_estab*stand->frac;
  acflux_estab=0;

  stand->cell->output.flux_estab+=acflux_estab*stand->frac;
  stand->cell->output.dcflux-=acflux_estab*stand->frac;

  foreachpft(pft,p,&stand->pftlist)
    if(istree(pft))
    {
      treepar=pft->par->data;
      if (stand->age>=treepar->max_rotation_length) 
        isdead=TRUE;
    }

  stand->cell->output.cftfrac[agtree(ncft)+stand->pft_id-npft+config->nagtree+irrigation->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]+=stand->frac;

  free(present);
  free(fpc_type);
  if(pft_len>0) free(fpc_inc2);
  free(n_est);
  if(isdead)
  {
    cutpfts(stand);
    stand->age=stand->growing_time=0;
    if(irrigation->irrigation)
    {
      stand->cell->discharge.dmass_lake+=(irrigation->irrig_stor+irrigation->irrig_amount)*stand->cell->coord.area*stand->frac;
      stand->cell->balance.awater_flux-=(irrigation->irrig_stor+irrigation->irrig_amount)*stand->frac;
      stand->cell->discharge.dmass_lake+=(irrigation->irrig_stor+irrigation->irrig_amount)*(1/irrigation->ec-1)*irrigation->conv_evap*stand->cell->coord.area*stand->frac;
      stand->cell->balance.awater_flux-=(irrigation->irrig_stor+irrigation->irrig_amount)*(1/irrigation->ec-1)*irrigation->conv_evap*stand->frac;
      stand->cell->output.mstor_return+=(irrigation->irrig_stor+irrigation->irrig_amount)*stand->frac;
      stand->cell->output.aconv_loss_evap-=(irrigation->irrig_stor+irrigation->irrig_amount)*(1/irrigation->ec-1)*irrigation->conv_evap*stand->frac;
      stand->cell->output.aconv_loss_drain-=(irrigation->irrig_stor+irrigation->irrig_amount)*(1/irrigation->ec-1)*(1-irrigation->conv_evap)*stand->frac;
      if(config->pft_output_scaled)
      {
        stand->cell->output.cft_conv_loss_evap[agtree(ncft)+stand->pft_id-npft+config->nagtree+irrigation->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(irrigation->irrig_stor+irrigation->irrig_amount)*(1/irrigation->ec-1)*irrigation->conv_evap*stand->frac;
        stand->cell->output.cft_conv_loss_drain[agtree(ncft)+stand->pft_id-npft+config->nagtree+irrigation->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(irrigation->irrig_stor+irrigation->irrig_amount)*(1/irrigation->ec-1)*(1-irrigation->conv_evap)*stand->frac;
      }
      else
      {
        stand->cell->output.cft_conv_loss_evap[agtree(ncft)+stand->pft_id-npft+config->nagtree+irrigation->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(irrigation->irrig_stor+irrigation->irrig_amount)*(1/irrigation->ec-1)*irrigation->conv_evap;
        stand->cell->output.cft_conv_loss_drain[agtree(ncft)+stand->pft_id-npft+config->nagtree+irrigation->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(irrigation->irrig_stor+irrigation->irrig_amount)*(1/irrigation->ec-1)*(1-irrigation->conv_evap);
      }
      irrigation->irrig_stor=0;
      irrigation->irrig_amount=0;
    }
    if(setaside(stand->cell,stand,config->pftpar,intercrop,npft,irrigation->irrigation))
      return TRUE;
  }
  else
    stand->cell->output.soil_storage+=(irrigation->irrig_stor+irrigation->irrig_amount)*stand->frac*stand->cell->coord.area;
  stand->age++;
  stand->growing_time++;

  return FALSE;
} /* of 'annual_agriculture_tree' */
