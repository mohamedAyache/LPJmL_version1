/***************************************************************************/
/**                                                                       **/
/**     a  n  n  u  a  l  _  b  i  o  m  a  s  s  _  g  r  a  s  s  .  c  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function performs necessary updates after iteration over one      **/
/**     year for grassland stand                                          **/
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
#include "grass.h"
#include "agriculture.h"
#include "biomass_grass.h"

Bool annual_biomass_grass(Stand *stand,         /* Pointer to stand */
                          int npft,             /* number of natural pfts */
                          int UNUSED(ncft),     /* number of crop PFTs */
                          Real UNUSED(popdens), /* population density (capita/km2) */
                          int UNUSED(year),
                          Bool UNUSED(intercrop),
                          const Config *config
                         )
{
  int p;
  Bool *present;
  Pft *pft;
  Real fpc_inc,excess,estab_store;
  Real acflux_estab;
  int n_est=0;
  Real fpc_total,*fpc_type;
  Irrigation *irrigation;
  Pftgrasspar *grasspar;

  irrigation=stand->data;
  fpc_type=newvec(Real,config->ntypes);
  check(fpc_type);
  present=newvec(Bool,npft);
  check(present);
  acflux_estab=estab_store=0;
  for(p=0;p<npft;p++)
    present[p]=FALSE;

  foreachpft(pft,p,&stand->pftlist)
  {
#ifdef DEBUG2
    printf("PFT:%s fpc=%g\n",pft->par->name,pft->fpc);
    printf("PFT:%s bm_inc=%g vegc=%g soil=%g\n",pft->par->name,
           pft->bm_inc,vegc_sum(pft),soilcarbon(&stand->soil));
#endif

    present[pft->par->id]=TRUE;
    grasspar=pft->par->data;
    if (stand->cell->balance.estab_storage_grass[irrigation->irrigation]<grasspar->sapling_C*0.01)
    {
      estab_store=pft->bm_inc*0.001;
      pft->bm_inc-=estab_store;
      stand->cell->balance.estab_storage_grass[irrigation->irrigation]+=estab_store*stand->frac;
    }
    if(annual_grass(stand,pft,&fpc_inc))
    {
      /* PFT killed, delete from list of established PFTs */
      litter_update_grass(&stand->soil.litter,pft,pft->nind);
      delpft(&stand->pftlist,p);
      p--; /* adjust loop variable */
    }

  } /* of foreachpft */
  fpc_sum(fpc_type,config->ntypes,&stand->pftlist);
  if(fpc_type[GRASS]>1.0)
    foreachpft(pft,p,&stand->pftlist)
    {
      excess=(fpc_type[GRASS]-1.0)*(pft->fpc/fpc_type[GRASS]);
      light_grass(&stand->soil.litter,pft,excess);
    }

  for(p=0;p<npft;p++)
  {
    if(config->pftpar[p].type==GRASS && config->pftpar[p].cultivation_type==BIOMASS
       && establish(stand->cell->gdd[p],config->pftpar+p,&stand->cell->climbuf))
    {
      if(!present[p])
       addpft(stand,config->pftpar+p);
      n_est++;
    }
  }

  fpc_total=fpc_sum(fpc_type,config->ntypes,&stand->pftlist);
  foreachpft(pft,p,&stand->pftlist)
   if(establish(stand->cell->gdd[pft->par->id],pft->par,&stand->cell->climbuf))
    acflux_estab+=establishment_grass(pft,fpc_total,fpc_type[pft->par->type],n_est);

  stand->cell->balance.estab_storage_grass[irrigation->irrigation]-=acflux_estab*stand->frac;
  acflux_estab=0;

  stand->cell->output.flux_estab+=acflux_estab*stand->frac;
  stand->cell->output.dcflux-=acflux_estab*stand->frac;

  stand->cell->output.soil_storage+=(irrigation->irrig_stor+irrigation->irrig_amount)*stand->frac*stand->cell->coord.area;

  free(fpc_type);
  free(present);
  return FALSE;
} /* of 'annual_biomass_grass' */
