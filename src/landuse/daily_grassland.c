/***************************************************************************/
/**                                                                       **/
/**             d  a  i  l  y  _  g  r  a  s  s  l  a  n  d  .  c         **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function of daily update of grassland stand                       **/
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

#include "lpj.h"
#include "grass.h"
#include "agriculture.h"
#include "grassland.h"

#ifdef NEW_GRASS
static const int mowingDays[] = {152, 335}; // mowing on fixed dates 1-june or 1-dec

Bool isMowingDay(int aDay)
{
  int i;
  int len = sizeof(mowingDays)/sizeof(int);
  for (i=0; i < len; i++)
  {
    if (aDay == mowingDays[i])
      return TRUE;
  }
  return FALSE;
}
#endif

Real daily_grassland(Stand *stand, /* stand pointer */
                     Real co2,   /* atmospheric CO2 (ppmv) */
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
                     int ncft,   /* number of crop PFTs   */
                     int year,
                     Bool withdailyoutput,
                     Bool intercrop,
                     const Config *config, /* LPJ config */
                     int cellnumber
                    )            /* returns runoff (mm) */
{
  int p,l;
  Pft *pft;
  Output *output;
  Harvest harvest={0,0,0,0};
  Real aet_stand[LASTLAYER];
  Real green_transp[LASTLAYER];
  Real evap,evap_blue,rd,gpp,frac_g_evap,runoff,wet_all,intercept,sprink_interc;
  Real cover_stand,intercep_pft;
  Real *wet; /* wet from pftlist */
  Real return_flow_b; /* irrigation return flows from surface runoff, lateral runoff and percolation (mm)*/
  Real rainmelt,irrig_apply;
  Real intercep_stand; /* total stand interception (rain + irrigation) (mm)*/
  Real intercep_stand_blue; /* irrigation interception (mm)*/
  Real rw_apply; /*applied irrigation water from rainwater harvesting storage, counted as green water */
  Real npp; /* net primary productivity (gC/m2) */
  Real gc_pft,gcgp;
  Real wdf; /* water deficit fraction */
  Bool isphen;
  Irrigation *data;
  Real rootdepth=0;
  Real dfpar;
  Pftgrass *grass;
  Real hfrac=0.5;
  irrig_apply=0.0;

  forrootmoist(l) rootdepth+=soildepth[l];
  data=stand->data;
  output=&stand->cell->output;
  evap=evap_blue=cover_stand=intercep_stand=intercep_stand_blue=wet_all=rw_apply=intercept=sprink_interc=rainmelt=intercep_pft=0.0;
  runoff=return_flow_b=0.0;
  if(getnpft(&stand->pftlist)>0)
  {
    wet=newvec(Real,getnpft(&stand->pftlist)); /* wet from pftlist */
    check(wet);
    for(p=0;p<getnpft(&stand->pftlist);p++)
      wet[p]=0;
  }
  else
    wet=NULL;
  if(!config->river_routing)
    irrig_amount(stand,config->pft_output_scaled,npft,config->nagtree,ncft);

  for(l=0;l<LASTLAYER;l++)
    aet_stand[l]=green_transp[l]=0;

  /* green water inflow */
  rainmelt=climate.prec+melt;
  if(rainmelt<0)
    rainmelt=0.0;

  if(data->irrigation && data->irrig_amount>epsilon)
  {
    irrig_apply=max(data->irrig_amount-rainmelt,0);  /*irrigate only missing deficit after rain, remainder goes to stor */
    data->irrig_stor+=data->irrig_amount-irrig_apply;
    data->irrig_amount=0.0;
    if(irrig_apply<1 && data->irrig_system!=DRIP) /* min. irrigation requirement of 1mm */
    {
      data->irrig_stor+=irrig_apply;
      irrig_apply=0.0;
    }
    else
    {
      /* write irrig_apply to output */
      stand->cell->output.mirrig+=irrig_apply*stand->frac;
      if(config->pft_output_scaled)
      {
        stand->cell->output.cft_airrig[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]+=irrig_apply*stand->cell->ml.landfrac[data->irrigation].grass[0];
        stand->cell->output.cft_airrig[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]+=irrig_apply*stand->cell->ml.landfrac[data->irrigation].grass[1];
      }
      else
      {
        stand->cell->output.cft_airrig[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]+=irrig_apply;
        stand->cell->output.cft_airrig[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]+=irrig_apply;
      }
    }
  }

  /* INTERCEPTION */
  foreachpft(pft,p,&stand->pftlist)
  {
    sprink_interc=(data->irrig_system==SPRINK) ? 1 : 0;

    intercept=interception(&wet[p],pft,pet,climate.prec+irrig_apply*sprink_interc);
    wet_all+=wet[p]*pft->fpc;
    intercep_stand_blue+=(climate.prec+irrig_apply*sprink_interc>epsilon) ? intercept*(irrig_apply*sprink_interc)/(climate.prec+irrig_apply*sprink_interc) : 0; /* blue intercept fraction */
    intercep_stand+=intercept;
  }
  irrig_apply-=intercep_stand_blue;
  rainmelt-=(intercep_stand-intercep_stand_blue);

  /* rain-water harvesting*/
  if(!data->irrigation && config->rw_manage && rainmelt<5)
    rw_apply=rw_irrigation(stand,gp_stand,wet,pet); /* Note: RWH supplementary irrigation is here considered green water */

  /* soil inflow: infiltration and percolation */
  if(irrig_apply>epsilon)
  {
    runoff+=infil_perc_irr(stand,irrig_apply,&return_flow_b,config->rw_manage,config,cellnumber);
    /* count irrigation events*/
    output->cft_irrig_events[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]++; /* id is consecutively counted over natural pfts, biomass, and the cfts; ids for cfts are from 12-23, that is why npft (=12) is distracted from id */
    output->cft_irrig_events[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]++; /* id is consecutively counted over natural pfts, biomass, and the cfts; ids for cfts are from 12-23, that is why npft (=12) is distracted from id */
  }

  runoff+=infil_perc_rain(stand,rainmelt+rw_apply,&return_flow_b,config->rw_manage,config,cellnumber);

  /* Version with daily allocation and grass management */
#ifdef NEW_GRASS

  isphen = FALSE;
  foreachpft(pft,p,&stand->pftlist)
  {
    grass=pft->data;
    pft->phen = 1.0; /* phenology is calculated from biomass */
    cover_stand+=pft->fpc*pft->phen;

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
      gcgp=gc_pft/gp_pft[getpftpar(pft,id)];
      if(stand->cell->ml.landfrac[data->irrigation].grass[0]>0.0)
      {
        output->gcgp_count[(npft-config->nbiomass)+rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]++;
        output->pft_gcgp[(npft-config->nbiomass)+rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]+=gcgp;
      }
      if(stand->cell->ml.landfrac[data->irrigation].grass[1]>0.0)
      {
        output->gcgp_count[(npft-config->nbiomass)+rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]++;
        output->pft_gcgp[(npft-config->nbiomass)+rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]+=gcgp;
      }
    }
    npp=npp_grass(pft,gtemp_air,gtemp_soil,gpp-rd);
    output->mnpp+=npp*stand->frac;
    output->dcflux+=npp*stand->frac;
    output->mgpp+=gpp*stand->frac;
    dfpar=(fpar(pft)*stand->frac*(1.0/(1-stand->cell->lakefrac)))*(1-beta);
    output->mfpar+=dfpar;
    //fprintf(stdout,"daily grassland id %i npp %g gpp %g phen %g bminc %g fpar %g fpc %g leaf %g root %g\n",
    //pft->par->id,npp,gpp,pft->phen,pft->bm_inc,dfpar,pft->fpc,grass->ind.leaf,grass->ind.root);
    if(config->pft_output_scaled)
    {
      output->pft_npp[(npft-config->nbiomass)+rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]+=npp*stand->cell->ml.landfrac[data->irrigation].grass[0];
      output->pft_npp[(npft-config->nbiomass)+rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]+=npp*stand->cell->ml.landfrac[data->irrigation].grass[1];
    }
    else
    {
      output->pft_npp[(npft-config->nbiomass)+rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]+=npp;
      output->pft_npp[(npft-config->nbiomass)+rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]+=npp;
    }
    if(withdailyoutput)
      if(output->daily.cft == C3_PERENNIAL_GRASS && data->irrigation == output->daily.irrigation)
      {
        output->daily.interc += intercep_pft;
        output->daily.npp += npp;
        output->daily.gpp += gpp;

        output->daily.croot += grass->ind.root;
        output->daily.cleaf += grass->ind.leaf;

        output->daily.rd += rd;
        output->daily.assim += gpp-rd;

        output->daily.rot_mode = stand->cell->ml.rotation.rotation_mode;
      }
  }

  /* calculate water balance */
  waterbalance(stand,aet_stand,green_transp,&evap,&evap_blue,wet_all,pet,cover_stand,
               &frac_g_evap,config->rw_manage);
  /* allocation, turnover and harvest AFTER photosynthesis */
  stand->growing_days = 1;
  /* turnover must happen before allocation */
  foreachpft(pft,p,&stand->pftlist)
    turnover_grass(&stand->soil.litter,pft,(Real)stand->growing_days/NDAYYEAR);
  allocation_today(stand,config->ntypes);

  /* daily turnover and harvest check*/
  isphen=FALSE;
  foreachpft(pft,p,&stand->pftlist)
  {
    grass=pft->data;
    switch(stand->cell->ml.grass_scenario)
    {
      case GS_DEFAULT: // default
        grass->max_leaf=max(grass->max_leaf,grass->ind.leaf);
        if(grass->ind.leaf>=100)
        {
          isphen=TRUE;
          hfrac=0.75;
        }
        else if(grass->ind.leaf>1 && grass->ind.leaf<(0.75*grass->max_leaf))
        {
          isphen=TRUE;
          hfrac=0.75;
          if(grass->ind.leaf*(1-hfrac)<1)
            hfrac=1-1/grass->ind.leaf;
        }
        break;
      case GS_MOWING: // mowing
        if (isMowingDay(day))
        {
          if (grass->ind.leaf > STUBBLE_HEIGHT_MOWING) /* 5 cm or 25 g.C.m-2 threshold */
            isphen=TRUE;
        }
        break;
      case GS_GRAZING_EXT: /* ext. grazing  */
        stand->cell->ml.rotation.rotation_mode = RM_UNDEFINED;
        if (grass->ind.leaf > STUBBLE_HEIGHT_GRAZING_EXT) /* minimum threshold */
        {
          isphen=TRUE;
          stand->cell->ml.rotation.rotation_mode = RM_GRAZING;
          stand->cell->ml.nr_of_lsus_ext = STOCKING_DENSITY_EXT;
        }
        break;
      case GS_GRAZING_INT: /* int. grazing */
        if ((grass->ind.leaf > STUBBLE_HEIGHT_GRAZING_INT) || (stand->cell->ml.rotation.rotation_mode > RM_UNDEFINED)) // 7-8 cm or 40 g.C.m-2 threshold
          isphen=TRUE;
        break;
    } /* of switch */
  } /* of foreachpft() */

  if(isphen)
  {
    /* grass=pft->data;
    output->cft_aboveground_biomass[rothers(ncft)+data->irrigation*(ncft+NGRASS)]=(grass->ind.leaf)*pft->nind;
    output->cft_aboveground_biomass[rmgrass(ncft)+data->irrigation*(ncft+NGRASS)]=(grass->ind.leaf)*pft->nind; */
    harvest=harvest_stand(output,stand,config->ntypes,hfrac);

    /* return irrig_stor and irrig_amount in case of harvest */
    if(data->irrigation)
    {
      stand->cell->discharge.dmass_lake+=(data->irrig_stor+data->irrig_amount)*stand->cell->coord.area*stand->frac;
      stand->cell->balance.awater_flux-=(data->irrig_stor+data->irrig_amount)*stand->frac;
      /* pay back conveyance losses that have already been consumed by transport into irrig_stor */
      stand->cell->discharge.dmass_lake+=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*data->conv_evap*stand->cell->coord.area*stand->frac;
      stand->cell->balance.awater_flux-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*data->conv_evap*stand->frac;
      stand->cell->output.mstor_return+=(data->irrig_stor+data->irrig_amount)*stand->frac;
      stand->cell->output.aconv_loss_evap-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*data->conv_evap*stand->frac;
      stand->cell->output.aconv_loss_drain-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*(1-data->conv_evap)*stand->frac;

      if(config->pft_output_scaled)
      {
        stand->cell->output.cft_conv_loss_evap[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*data->conv_evap*stand->cell->ml.landfrac[data->irrigation].grass[0];
        stand->cell->output.cft_conv_loss_evap[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*data->conv_evap*stand->cell->ml.landfrac[data->irrigation].grass[1];
        stand->cell->output.cft_conv_loss_drain[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*(1-data->conv_evap)*stand->cell->ml.landfrac[data->irrigation].grass[0];
        stand->cell->output.cft_conv_loss_drain[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*(1-data->conv_evap)*stand->cell->ml.landfrac[data->irrigation].grass[1];
      }
      else
      {
        stand->cell->output.cft_conv_loss_evap[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*data->conv_evap;
        stand->cell->output.cft_conv_loss_evap[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*data->conv_evap;
        stand->cell->output.cft_conv_loss_drain[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*(1-data->conv_evap);
        stand->cell->output.cft_conv_loss_drain[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*(1-data->conv_evap);
      }
      data->irrig_stor=0;
      data->irrig_amount=0;
    }
  }

#else
/* Old Version without daily allocation and grass management */
  isphen=FALSE;
  foreachpft(pft,p,&stand->pftlist)
  {
    phenology_grass(pft,climate.temp,day);
    if(pft->phen==1 && pft->bm_inc>=100)
      isphen=TRUE;
  }
  if(isphen)
  {
    /*grass=pft->data;
    output->cft_aboveground_biomass[rothers(ncft)+data->irrigation*(ncft+NGRASS)]=(grass->ind.leaf)*pft->nind;
    output->cft_aboveground_biomass[rmgrass(ncft)+data->irrigation*(ncft+NGRASS)]=(grass->ind.leaf)*pft->nind;*/
    harvest=harvest_stand(output,stand,config->ntypes,0.5);
    /* return irrig_stor and irrig_amount in case of harvest */
    if(data->irrigation)
    {
      stand->cell->discharge.dmass_lake+=(data->irrig_stor+data->irrig_amount)*stand->cell->coord.area*stand->frac;
      stand->cell->balance.awater_flux-=(data->irrig_stor+data->irrig_amount)*stand->frac;
      /* pay back conveyance losses that have already been consumed by transport into irrig_stor */
      stand->cell->discharge.dmass_lake+=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*data->conv_evap*stand->cell->coord.area*stand->frac;
      stand->cell->balance.awater_flux-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*data->conv_evap*stand->frac;
      stand->cell->output.mstor_return+=(data->irrig_stor+data->irrig_amount)*stand->frac;
      stand->cell->output.aconv_loss_evap-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*data->conv_evap*stand->frac;
      stand->cell->output.aconv_loss_drain-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*(1-data->conv_evap)*stand->frac;

      if(config->pft_output_scaled)
      {
        stand->cell->output.cft_conv_loss_evap[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*data->conv_evap*stand->cell->ml.landfrac[data->irrigation].grass[0];
        stand->cell->output.cft_conv_loss_evap[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*data->conv_evap*stand->cell->ml.landfrac[data->irrigation].grass[1];
        stand->cell->output.cft_conv_loss_drain[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*(1-data->conv_evap)*stand->cell->ml.landfrac[data->irrigation].grass[0];
        stand->cell->output.cft_conv_loss_drain[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*(1-data->conv_evap)*stand->cell->ml.landfrac[data->irrigation].grass[1];
      }
      else
      {
        stand->cell->output.cft_conv_loss_evap[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*data->conv_evap;
        stand->cell->output.cft_conv_loss_evap[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*data->conv_evap;
        stand->cell->output.cft_conv_loss_drain[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*(1-data->conv_evap);
        stand->cell->output.cft_conv_loss_drain[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]-=(data->irrig_stor+data->irrig_amount)*(1/data->ec-1)*(1-data->conv_evap);
      }
      data->irrig_stor=0;
      data->irrig_amount=0;
    }
  }
  else
    stand->growing_days++;

  foreachpft(pft,p,&stand->pftlist)
  {
    cover_stand+=pft->fpc*pft->phen;

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
      gcgp=gc_pft/gp_pft[getpftpar(pft,id)];
      if(stand->cell->ml.landfrac[data->irrigation].grass[0]>0.0)
      {
        output->gcgp_count[(npft-config->nbiomass-config->nagtree)+rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]++;
        output->pft_gcgp[(npft-config->nbiomass-config->nagtree)+rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]+=gcgp;
      }
      if(stand->cell->ml.landfrac[data->irrigation].grass[1]>0.0)
      {
        output->gcgp_count[(npft-config->nbiomass-config->nagtree)+rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]++;
        output->pft_gcgp[(npft-config->nbiomass-config->nagtree)+rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]+=gcgp;
      }
    }
    npp=npp_grass(pft,gtemp_air,gtemp_soil,gpp-rd);
    output->mnpp+=npp*stand->frac;
    output->mgpp+=gpp*stand->frac;
    output->mfpar+=(fpar(pft)*stand->frac*(1.0/(1-stand->cell->lakefrac)))*(1-beta);
    output->cft_fpar[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]+=(fpar(pft)*stand->cell->ml.landfrac[data->irrigation].grass[0]*(1.0/(1-stand->cell->lakefrac)))*(1-beta);
    output->cft_fpar[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]+=(fpar(pft)*stand->cell->ml.landfrac[data->irrigation].grass[1]*(1.0/(1-stand->cell->lakefrac)))*(1-beta);

    if(config->pft_output_scaled)
    {
      output->pft_npp[(npft-config->nbiomass-config->nagtree)+rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]+=npp*stand->cell->ml.landfrac[data->irrigation].grass[0];
      output->pft_npp[(npft-config->nbiomass-config->nagtree)+rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]+=npp*stand->cell->ml.landfrac[data->irrigation].grass[1];
    }
    else
    {
      output->pft_npp[(npft-config->nbiomass-config->nagtree)+rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]+=npp;
      output->pft_npp[(npft-config->nbiomass-config->nagtree)+rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]+=npp;
    }
    grass = pft->data;
    if(withdailyoutput)
      if(output->daily.cft == C3_PERENNIAL_GRASS && data->irrigation == output->daily.irrigation)
      {
        output->daily.interc += intercep_pft;
        output->daily.npp += npp;
        output->daily.gpp += gpp;

        output->daily.croot += grass->ind.root;
        output->daily.cleaf += grass->ind.leaf;

        output->daily.rd += rd;
        output->daily.assim += gpp-rd;
      }
  } /* of foreachpft */

  /* soil outflow: evap and transpiration */
    waterbalance(stand,aet_stand,green_transp,&evap,&evap_blue,wet_all,pet,cover_stand,
                 &frac_g_evap,config->rw_manage);
#endif

  if(withdailyoutput)
  {
    foreachpft(pft,p,&stand->pftlist)
      if(output->daily.cft == C3_PERENNIAL_GRASS && data->irrigation == output->daily.irrigation)
      {
        output->daily.evap+=evap;
        forrootsoillayer(l)
          output->daily.trans+=aet_stand[l];
        output->daily.irrig=irrig_apply;
        output->daily.w0+=stand->soil.w[1];
        output->daily.w1+=stand->soil.w[2];
        output->daily.wevap+=stand->soil.w[0];
        output->daily.par=par;
        output->daily.daylength=daylength;
        output->daily.pet+=pet;
      }
  }

  if(data->irrigation && stand->pftlist.n>0) /*second element to avoid irrigation on just harvested fields */
    irrigation_stand(stand,gp_stand,wet,pet);

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

  output->minterc+=intercep_stand*stand->frac; /* Note: including blue fraction*/
  output->minterc_b+=intercep_stand_blue*stand->frac;   /* blue interception and evap */

  output->mevap+=evap*stand->frac;
  output->mevap_b+=evap_blue*stand->frac;   /* blue soil evap */

  output->mreturn_flow_b+=return_flow_b*stand->frac; /* now only changed in waterbalance_new.c*/

  if(config->pft_output_scaled)
  {
    output->pft_harvest[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)].harvest+=harvest.harvest*stand->cell->ml.landfrac[data->irrigation].grass[0];
    output->pft_harvest[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)].harvest+=harvest.harvest*stand->cell->ml.landfrac[data->irrigation].grass[1];

    output->pft_harvest[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)].residual+=harvest.residual*stand->cell->ml.landfrac[data->irrigation].grass[0];
    output->pft_harvest[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)].residual+=harvest.residual*stand->cell->ml.landfrac[data->irrigation].grass[1];
  }
  else
  {
    output->pft_harvest[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)].harvest+=harvest.harvest;
    output->pft_harvest[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)].harvest+=harvest.harvest;

    output->pft_harvest[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)].residual+=harvest.residual;
    output->pft_harvest[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)].residual+=harvest.residual;
  }

    /* harvested area */

  if (isphen)
  {
    output->cftfrac[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]=stand->cell->ml.landfrac[data->irrigation].grass[0];
    output->cftfrac[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)]=stand->cell->ml.landfrac[data->irrigation].grass[1];
  }
  output->cft_pet[rothers(ncft)+data->irrigation*(ncft+NGRASS)]+=pet;
  output->cft_temp[rothers(ncft)+data->irrigation*(ncft+NGRASS)]+= climate.temp >= 5 ? climate.temp : 0;
  output->cft_prec[rothers(ncft)+data->irrigation*(ncft+NGRASS)]+=climate.prec;
  output->cft_srad[rothers(ncft)+data->irrigation*(ncft+NGRASS)]+=climate.swdown;
  output->cft_pet[rmgrass(ncft)+data->irrigation*(ncft+NGRASS)]+=pet;
  output->cft_temp[rmgrass(ncft)+data->irrigation*(ncft+NGRASS)]+=climate.temp;
  output->cft_prec[rmgrass(ncft)+data->irrigation*(ncft+NGRASS)]+=climate.prec;
  output->cft_srad[rmgrass(ncft)+data->irrigation*(ncft+NGRASS)]+=climate.swdown;

  /* output for green and blue water for evaporation, transpiration and interception */
  output_gbw_grassland(output,stand,frac_g_evap,evap,evap_blue,return_flow_b,aet_stand,green_transp,
      intercep_stand,intercep_stand_blue,npft,config->nagtree,ncft,config->pft_output_scaled);
  free(wet);
  return runoff;
} /* of 'daily_grassland' */
