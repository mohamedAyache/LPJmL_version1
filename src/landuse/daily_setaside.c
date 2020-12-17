/***************************************************************************/
/**                                                                       **/
/**                d  a  i  l  y  _  s  e  t  a  s  i  d  e  .  c         **/
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
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifdef DAILY_ESTABLISHMENT

#include "lpj.h"
#include "grass.h"
#include "natural.h"
#include "agriculture.h"

Real daily_setaside(Stand *stand, /* stand pointer */
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
                   int UNUSED(nagtree),   /* number of agriculture tree PFTs */
                   int UNUSED(ncft),   /* number of crop PFTs   */
                   int UNUSED(year),
                   Bool withdailyoutput,
                   Bool intercrop,
                   const Config *config /* LPJ config */
                  ) /* returns runoff (mm) */
{
  int p,l;
  Pft *pft;
  Output *output;
  Real aet_stand[LASTLAYER];
  Real green_transp[LASTLAYER];
  Real evap,evap_blue; /* evaporation (mm) */
  Real rd,gpp,frac_g_evap,runoff,wet_all;
  Real *wet; /* wet from pftlist */
  Real return_flow_b; /* irrigation return flows from surface runoff, lateral runoff and percolation (mm)*/
  Real cover_stand,intercep_stand,rainmelt;
  Real npp; /* net primary productivity (gC/m2) */
  Real wdf; /* water deficit fraction */
  Real gc_pft;
  Real acflux_estab = 0;
  int n_est;
  Bool *present;
  output=&stand->cell->output;
  Real rootdepth=0;

  forrootmoist(l) rootdepth+=soildepth[l];
  evap=evap_blue=cover_stand=intercep_stand=wet_all=rainmelt=0.0;
  present=newvec(Bool,npft);
  check(present);
  for(p=0;p<npft;p++)
    present[p]=FALSE;
  runoff=return_flow_b=0.0;
  stand->growing_days++;
  if(getnpft(&stand->pftlist)>0)
  {
    wet=newvec(Real,getnpft(&stand->pftlist)); /* wet from pftlist */
    check(wet);
    for(p=0;p<getnpft(&stand->pftlist);p++)
      wet[p]=0;
  }
  else
    wet=NULL;

  for(l=0;l<LASTLAYER;l++)
    aet_stand[l]=green_transp[l]=0;

  foreachpft(pft,p,&stand->pftlist)
  {
    present[pft->par->id]=TRUE;
    leaf_phenology(pft,climate.temp,day);
    cover_stand+=pft->fpc*pft->phen;
    intercep_stand+=interception(&wet[p],pft,pet,climate.prec);
    wet_all+=wet[p]*pft->fpc;
  }

  /* soil inflow: infiltration and percolation */
  rainmelt=climate.prec+melt;
  if(rainmelt<0)
    rainmelt=0.0;
  runoff+=infil_perc_rain(stand,rainmelt-intercep_stand,&return_flow_b,config->rw_manage,config,cellnumber);

  foreachpft(pft,p,&stand->pftlist)
  {
    /*
     *  Calculate net assimilation, i.e. gross primary production minus leaf
     *  respiration, including conversion from FPC to grid cell basis.
     *
     */

    gpp=water_stressed(pft,aet_stand,gp_stand,gp_stand_leafon,
                       gp_pft[getpftpar(pft,id)],&gc_pft,&rd,
                       &wet[p],pet,co2,climate.temp,par,daylength,&wdf,config->permafrost);
    if(gp_pft[getpftpar(pft,id)]>0.0)
    {
      output->gcgp_count[pft->par->id]++;
      output->pft_gcgp[pft->par->id]+=gc_pft/gp_pft[getpftpar(pft,id)];
    }

    npp=npp(pft,gtemp_air,gtemp_soil,gpp-rd);
    output->mnpp+=npp*stand->frac;
    output->dcflux+=npp*stand->frac;
    output->mgpp+=gpp*stand->frac;
    output->mfpar+=(fpar(pft)*stand->frac*(1.0/(1-stand->cell->lakefrac)))*(1-beta);
    if(config->pft_output_scaled)
      output->pft_npp[pft->par->id]+=npp*stand->frac;
    else
      output->pft_npp[pft->par->id]+=npp;

  } /* of foreachpft */

  /* soil outflow: evap and transpiration */
  waterbalance(stand,aet_stand,green_transp,&evap,&evap_blue,wet_all,pet,cover_stand,
               &frac_g_evap,config->rw_manage);

  if(withdailyoutput)
  {
    foreachpft(pft,p,&stand->pftlist)
      if(pft->par->id==output->daily.cft)
      {
        output->daily.evap=evap;
        forrootsoillayer(l)
          output->daily.trans+=aet_stand[l];
        output->daily.irrig=0;
        output->daily.w0=stand->soil.w[1];
        output->daily.w1=stand->soil.w[2];
        output->daily.wevap=stand->soil.w[0];
        output->daily.par=par;
        output->daily.daylength=daylength;
        output->daily.pet=pet;
      }
  }

  foreachsoillayer(l)
    output->mswc[l]+=(stand->soil.w[l]*stand->soil.par->whcs[l]+stand->soil.w_fw[l]+stand->soil.par->wpwps[l]+
                   stand->soil.ice_depth[l]+stand->soil.ice_fw[l])/stand->soil.par->wsats[l]*stand->frac*(1.0/(1-stand->cell->lakefrac));
  forrootmoist(l)
    output->mrootmoist+=stand->soil.w[l]*soildepth[l]/rootdepth*stand->frac*(1.0/(1-stand->cell->lakefrac));
  forrootsoillayer(l)
  {
    output->mtransp+=aet_stand[l]*stand->frac;
    output->mtransp_b+=(aet_stand[l]-green_transp[l])*stand->frac;
  }

  output->minterc+=intercep_stand*stand->frac;
  output->mevap+=evap*stand->frac;
  output->mevap_b+=evap_blue*stand->frac; /*TODOJJ why are monthly outputs in setaside written but cft-outputs not? */
  output->mreturn_flow_b+=return_flow_b*stand->frac;

  /* new block for daily establishment */
  n_est=0;
  if(intercrop)
  {
    acflux_estab=0;
    for(p=0;p<npft;p++)
    {
      if(config->pftpar[p].type==GRASS && config->pftpar[p].cultivation_type==NONE /* still correct?? */ && (!present[p]) &&
         establish(stand->cell->gdd[p],config->pftpar+p,&stand->cell->climbuf))
        addpft(stand,config->pftpar+p);
        n_est++;
    }
    foreachpft(pft,p,&stand->pftlist)
      if(!pft->established)
      {
        acflux_estab+=establishment_grass(pft,0,0,n_est);
        pft->established=TRUE;
      }


    output->flux_estab+=acflux_estab*stand->frac;
  }
  free(present);
  /* end new block for daily establishment */

  /* output for green and blue water for evaporation, transpiration and interception */
  /*nooutput_gbw(output,stand,frac_g_evap,frac_g,evap,aet_stand,intercep_stand,npft,ncft,setting);*/
  free(wet);
  return runoff;
} /* of 'daily_setaside' */

#endif /* DAILY_ESTABLISHMENT */
