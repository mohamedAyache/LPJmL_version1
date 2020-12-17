/***************************************************************************/
/**                                                                       **/
/**                d a i l y _ a g r i c u l t u r e _ t r e e . c        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function of daily update of natural stand                         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     See file AUTHORS for list of authors contributing to this code    **/
/**                                                                       **/
/**     Last change: $Date::                                            $ **/
/**     By         : $Author::                                 $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "natural.h"
#include "tree.h"
#include "agriculture.h"
#include "agriculture_tree.h"

Real daily_agriculture_tree(Stand *stand, /* stand pointer */
                            const Pftpar * UNUSED(pftpar),
                            Real co2,     /* atmospheric CO2 (ppmv) */
                            Dailyclimate climate, /* Daily climate values */
                            int day,    /* day (1..365) */
                            Real daylength, /* length of day (h) */
                            const Real gp_pft[], /* pot. canopy conductance for PFTs & CFTs*/
                            Real gtemp_air,  /* value of air temperature response function */
                            Real gtemp_soil, /* value of soil temperature response function */
                            Real gp_stand,
                            Real gp_stand_leafon, /* pot. canopy conduct.at full leaf cover */
                            Real pet,   /* potential evapotranspiration (mm) */
                            Real par,   /* photosynthetic active radiation flux */
                            Real melt,  /* melting water (mm) */
                            int npft,   /* number of natural PFTs */
                            int nbiomass, /* number of biomass PFTs */
                            int nagtree, /* number of agriculture tree PFTs */
                            int ncft,   /* number of crop PFTs   */
                            int UNUSED(ntypes), /* number of different PFT classes */
                            Setting setting, /* LPJ settings */
                            int UNUSED(year)
                           ) /* returns runoff (mm) */
{
  int p,l;
  Pft *pft;
  Pfttree *tree;
  Pfttreepar *treepar;
  Output *output;
  Real aet_stand[LASTLAYER];
  Real evap; /* evaporation (mm) */
  Real wet,rd,gpp,frac_g_evap,runoff,return_flow_b,wet_all;
  Real cover_stand,intercep_stand;
  Real npp; /* net primary productivity (gC/m2) */
  Real wdf; /* water deficit fraction */
  Real gc_pft;
  Real frac_g[NSOILLAYER];
  Irrigation *data;
  Real frac_g_in;
  Real rootdepth=0;

  data=stand->data;
  forrootmoist(l) rootdepth+=soildepth[l];
  output=&stand->cell->output;
  stand->growing_days++;
  cover_stand=intercep_stand=wet=runoff=return_flow_b=wet_all=0;
  if(!setting.river_routing)
    irrig_amount(stand,setting.pft_output_scaled,npft,nagtree,ncft);

  if(setting.pft_output_scaled)
    stand->cell->output.pft_airrig[agtree(ncft)+stand->pft_id-npft+nagtree+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->irrig_amount*stand->cell->ml.landfrac[1].ag_tree[stand->pft_id-npft+nagtree];
  else
    stand->cell->output.pft_airrig[agtree(ncft)+stand->pft_id-npft+nagtree+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->irrig_amount;
  
  for(l=0;l<LASTLAYER;l++)
    aet_stand[l]=0;

  /* apply irrigation water */
  if(data->irrigation)
    runoff=irrigation(stand,&return_flow_b);

  foreachpft(pft,p,&stand->pftlist)
  {
    leaf_phenology(pft,climate.temp,day);
    cover_stand+=pft->fpc*pft->phen;

    intercep_stand+=interception(&wet,pft,pet,climate.prec);
    wet_all+=wet*pft->fpc;
/*
 *  Calculate net assimilation, i.e. gross primary production minus leaf
 *  respiration, including conversion from FPC to grid cell basis.
 *
 */

    gpp=water_stressed(pft,aet_stand,gp_stand,gp_stand_leafon,
                       gp_pft[getpftpar(pft,id)],&gc_pft,&rd,
                       &wet,pet,co2,climate.temp,par,daylength,&wdf,setting.permafrost);
   if(stand->cell->ml.landfrac[data->irrigation].ag_tree[stand->pft_id-npft+nagtree]>0.0 &&
      gp_pft[pft->par->id]>0.0)
   {
     output->gcgp_count[(npft-nbiomass-nagtree)+agtree(ncft)+stand->pft_id-npft+nagtree+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]++;
     output->pft_gcgp[(npft-nbiomass-nagtree)+agtree(ncft)+stand->pft_id-npft+nagtree+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=gc_pft/gp_pft[getpftpar(pft,id)];
   }
   npp=npp(pft,gtemp_air,gtemp_soil,gpp-rd);
   if(istree(pft) && npp>0)
   {
     tree=pft->data;
     treepar=pft->par->data;
     switch(pft->par->phenology)
     {
       case SUMMERGREEN:
         if(pft->phen>0.5)
         {
           tree->c_fruit+=treepar->harvest_ratio*npp;
           //if(pft->par->id==COTTON) printf("PFT %s summergreen fruit are growing at month %d, phen=%g, aphen=%g, gdd=%g, temp %.2f, harvest=%g t/ha\n",pft->par->name,day/30+1,pft->phen,pft->aphen,pft->gdd,climate.temp, tree->c_fruit*1e4/0.45*1e-6);
         }
         break;
       case EVERGREEN:
         if(pft->gdd>0 && pft->gdd<1/pft->par->ramp)
         {
      
		//if(pft->par->id==CITRUS) printf("PFT %s evergreen fruit are growing at month %d, harvest=%g,gdd=%g,ramp:%g t/ha\n",pft->par->name,day/30+1,tree->c_fruit*1e4/0.45*1e-6, pft->gdd,pft->par->ramp);

           tree->c_fruit+=treepar->harvest_ratio*npp;
         }
         break;
     }
   }
   output->mnpp+=npp*stand->frac;
   output->mgpp+=gpp*stand->frac;
   output->mfpar+=(fpar(pft)*stand->frac*(1.0/(1-stand->cell->lakefrac)))*(1-beta);
   if(setting.pft_output_scaled)
     output->pft_npp[(npft-nbiomass-nagtree)+agtree(ncft)+stand->pft_id-npft+nagtree+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=npp*stand->cell->ml.landfrac[data->irrigation].ag_tree[stand->pft_id-npft+nagtree];
   else
     output->pft_npp[(npft-nbiomass-nagtree)+agtree(ncft)+stand->pft_id-npft+nagtree+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=npp;

  } /* of foreachpft */
  
  frac_g_in=(climate.prec>0.0) ? 1.0  : -1.0;
  foreachsoillayer(l)
    frac_g[l]=stand->frac_g[l];

  /* calculate water balance */
  if(setting.new_hydrology)
    runoff+=waterbalance_new(stand,aet_stand,&evap,wet_all,
                             climate.prec+melt-intercep_stand,
                             pet,cover_stand,&frac_g_evap,&return_flow_b);
  else
    /*runoff+=waterbalance(stand,aet_stand,&evap,
                         climate.prec+melt-intercep_stand,
                         pet,cover_stand,&frac_g_evap,&return_flow_b);*/
    runoff+=waterbalance(&stand->soil,aet_stand,&evap,climate.prec+melt-intercep_stand,pet,
                        cover_stand,stand->cell->coord,stand->type->landusetype,
                        stand->frac_g,frac_g_in,&return_flow_b);
#ifndef NEWHYDRO
  frac_g_evap=stand->frac_g[NSOILLAYER];
#endif

  if(data->irrigation && stand->pftlist.n>0) /*HB second element to avoid irrigation on just harvested fields */
    irrigation_stand(stand,gp_stand,wet,pet);

  output->mreturn_flow_b+=return_flow_b*stand->frac;

  output->mevap+=evap*stand->frac;
  
  foreachsoillayer(l)
    output->mswc[l]+=(stand->soil.w[l]*stand->soil.par->whcs[l]+stand->soil.w_fw[l]+stand->soil.par->wpwps[l]+
                   stand->soil.ice_depth[l]+stand->soil.ice_fw[l])/stand->soil.par->wsats[l]*stand->frac*(1.0/(1-stand->cell->lakefrac));
  forrootmoist(l)
    output->mrootmoist+=stand->soil.w[l]*soildepth[l]/rootdepth*stand->frac*(1.0/(1-stand->cell->lakefrac));

  forrootsoillayer(l)
    output->mtransp+=aet_stand[l]*stand->frac;
 
  output->minterc+=intercep_stand*stand->frac;

  /* output for green and blue water for evaporation, transpiration and interception */
  output_gbw_agriculture_tree(output,stand,frac_g_evap,frac_g,evap,aet_stand,
                             intercep_stand,npft,nagtree,ncft,setting);

  return runoff;
} /* of 'daily_agriculture_tree' */
