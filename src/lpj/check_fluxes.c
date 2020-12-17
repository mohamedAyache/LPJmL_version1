/***************************************************************************/
/**                                                                       **/
/**             c  h  e  c  k  _  f  l  u  x  e  s  .  c                  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function checks for water and carbon balance in a cell            **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 24.02.2009                                           **/
/**     Last change: 19.08.2009 by Christoph Mueller & Elke Stehfest      **/
/**     Last change: $Date:: 2015-11-10 12:19:21 +0100 (mar., 10 nov. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "agriculture.h"

void check_fluxes(Cell *cell,          /* cell pointer */
                  int year,            /* simulation year (AD) */
                  int cellid,          /* cell index */
                  const Config *config /* LPJ configuration */
                 )
{
  Real balanceC,totc,delta_totc;
  Real balanceW,totw;
  Stand *stand;
  int s,i;

#ifdef IMAGE
  int p;
  Pft *pft;
#endif
  /* carbon balance check */
  totc=0;

  foreachstand(stand,s,cell->standlist)
    totc+=standcarbon(stand)*stand->frac;
  if(cell->ml.dam)
    totc+=cell->ml.resdata->c_pool;
  totc+=cell->balance.estab_storage_grass[0]+cell->balance.estab_storage_tree[0]+cell->balance.estab_storage_grass[1]+cell->balance.estab_storage_tree[1];
#ifdef IMAGE
  totc+=cell->ml.image_data->timber.slow+cell->ml.image_data->timber.fast;
#endif
  delta_totc=totc-cell->balance.totc;
  cell->balance.totc=totc;

  balanceC=cell->balance.nep-cell->output.firec-cell->output.flux_firewood+cell->output.flux_estab-cell->output.flux_harvest-cell->balance.biomass_yield-delta_totc;
  /* for IMAGE but can also be used without IMAGE */
  balanceC-=cell->output.deforest_emissions+cell->output.prod_turnover+cell->output.trad_biofuel;

  /*fprintf(stdout,"y: %d c: %d (%0.2f/%0.2f) nep: %.2f firec: %.2f flux_estab: %.2f \n"
		  "         flux_harvest: %.2f totc: %.2f delta_totc %.2f\n biomass_yield: %.2f estab_storage_grass: %.2f estab_storage_tree %.2f\n",
                    year,cellid+config.startgrid,cell->coord.lon,cell->coord.lat,
                    cell->nep,cell->output.firec,
                    cell->output.flux_estab,cell->output.flux_harvest,totc,delta_totc,cell->biomass_yield,cell->estab_storage_grass[0]+cell->estab_storage_grass[1],cell->estab_storage_tree[0]+cell->estab_storage_tree[1]);
  fflush(stdout);*/
  /*if((year>config.firstyear+1 && fabs(balanceC)>2) 
     )
  {
    printf("y: %d c: %d (%0.2f/%0.2f) BALANCE_C %.10f nep: %.2f npp: %.2f rh: %.2f firec: %.2f \nflux_firewood %.2f flux_estab: %.2f flux_harvest: %.2f delta_totc: %.2f totc %.2f\ndeforest_emissions: %.2f product_turnover: %.2f trad_biofuel: %.2f product pools %.2f %.2f \ntimber_harvest %.2f ftimber %.2f fburn %.2f biomass_yield %.2f\nestab_storage_grass %g %g estab_storage_tree %g %g\n",
         year,cellid,cell->coord.lon,cell->coord.lat,balanceC,cell->nep,
         cell->ml.image_data->anpp,cell->ml.image_data->arh,
         cell->output.firec,cell->output.flux_firewood,
         cell->output.flux_estab,cell->output.flux_harvest,delta_totc,totc,
         cell->output.deforest_emissions,cell->output.prod_turnover,cell->output.trad_biofuel,
         cell->ml.image_data->timber.slow,cell->ml.image_data->timber.fast,cell->output.timber_harvest,
         cell->ml.image_data->timber_f,cell->ml.image_data->fburnt,cell->biomass_yield,
         cell->estab_storage_grass[0],cell->estab_storage_grass[1],cell->estab_storage_tree[0],cell->estab_storage_tree[1]);
    fflush(stdout);
    foreachstand(stand,s,cell->standlist)
    {
      printf("C-balance on stand %d LUT %s, standfrac %g litter %g timber_frac %g\n",
        s,stand->type->name,stand->frac,littersum(&stand->soil.litter),cell->ml.image_data->timber_frac);
      fflush(stdout);
      foreachpft(pft,p,&stand->pftlist)
        printf("pft %d, %s, vegc %g\n",
               p,pft->par->name,vegc_sum(pft));
      fflush(stdout);
    }
  }*/

  if(year>config->firstyear+1 && fabs(balanceC)>2)
  {
#ifdef IMAGE
    foreachstand(stand,s,cell->standlist)
    {
      printf("C-balance on stand %d LUT %s, standfrac %g litter %g timber_frac %g\n",
        s,stand->type->name,stand->frac,littersum(&stand->soil.litter),cell->ml.image_data->timber_frac);
      fflush(stdout);
      foreachpft(pft,p,&stand->pftlist)
        printf("pft %d, %s, vegc %g\n",
               p,pft->par->name,vegc_sum(pft));
      fflush(stdout);
    } /* of 'foreachstand' */
    fail(INVALID_CARBON_BALANCE_ERR,TRUE,"y: %d c: %d (%0.2f/%0.2f) BALANCE_C-error %.10f nep: %.2f firec: %.2f flux_estab: %.2f flux_harvest: %.2f delta_totc: %.2f\ndeforest_emissions: %.2f product_turnover: %.2f trad_biofuel: %.2f product pools %.2f %.2f timber_harvest %.2f ftimber %.2f fburn %.2f\n",
         year,cellid+config->startgrid,cell->coord.lon,cell->coord.lat,balanceC,cell->balance.nep,
         cell->output.firec,
         cell->output.flux_estab,cell->output.flux_harvest,delta_totc,
         cell->output.deforest_emissions,cell->output.prod_turnover,cell->output.trad_biofuel,
         cell->ml.image_data->timber.slow,cell->ml.image_data->timber.fast,cell->output.timber_harvest,
         cell->ml.image_data->timber_f,cell->ml.image_data->fburnt);
#else
    fail(INVALID_CARBON_BALANCE_ERR,TRUE,"y: %d c: %d (%0.2f/%0.2f) BALANCE_C-error %.10f nep: %.2f\n"
                "                            firec: %.2f flux_estab: %.2f \n"
                "                            flux_harvest: %.2f delta_totc: %.2f biomass_yield: %.2f\n"
                "                            estab_storage_grass: %.2f %.2f estab_storage_tree %.2f %.2f\n"
                "                            deforest_emissions: %.2f product_turnover: %.2f\n",
         year,cellid+config->startgrid,cell->coord.lon,cell->coord.lat,balanceC,cell->balance.nep,cell->output.firec,
         cell->output.flux_estab,cell->output.flux_harvest,delta_totc,cell->balance.biomass_yield,
         cell->balance.estab_storage_grass[0],cell->balance.estab_storage_grass[1],cell->balance.estab_storage_tree[0],cell->balance.estab_storage_tree[1],
         cell->output.deforest_emissions,cell->output.prod_turnover);
#endif
  }
  /* water balance check */
  totw=(cell->discharge.dmass_lake+cell->discharge.dmass_river)/cell->coord.area;
  foreachstand(stand,s,cell->standlist)
  {
    totw+=soilwater(&stand->soil)*stand->frac;
/*    if(&stand->type->name!=NATURAL)
    {
      data=&stand->data;
      totw+=(data->irrig_stor+data->irrig_amount)*stand->frac;
    }*/
  }
  if(cell->ml.dam)
  {
     totw+=cell->ml.resdata->dmass/cell->coord.area;/*+cell->resdata->dfout_irrigation/cell->coord.area; */
     for(i=0;i<NIRRIGDAYS;i++)
       totw+=cell->ml.resdata->dfout_irrigation_daily[i]/cell->coord.area;
  }
  balanceW=totw-cell->balance.totw-cell->balance.aprec+cell->balance.awater_flux;
  if(year>config->firstyear+1 && fabs(balanceW)>1.5)
    fail(INVALID_WATER_BALANCE_ERR,TRUE,"y: %d c: %d (%0.2f/%0.2f) BALANCE_W-error %.2f cell->totw:%.2f totw:%.2f awater_flux:%.2f aprec:%.2f\n",
         year,cellid+config->startgrid,cell->coord.lon,cell->coord.lat,balanceW,cell->balance.totw,totw,
         cell->balance.awater_flux,cell->balance.aprec);
  cell->balance.totw=totw;
} /* of 'check_fluxes' */
