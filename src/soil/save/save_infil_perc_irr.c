/***************************************************************************/
/**                                                                       **/
/**         i  n  f  i  l  _  p  e  r  c  _  i  r  r  .  c                **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function calculates water balance                                 **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 13.10.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "agriculture.h"

Real infil_perc_irr(Stand *stand,        /* Stand pointer */
                    Real infil,          /* infiltration water (mm) */
                    Real *return_flow_b, /* blue water return flow (mm) */
                    Bool rw_manage       /* do rain water management? (TRUE/FALSE) */
                   )                     /* returns water runoff (mm) */
{
  Real runoff;
  Real perc,slug;
  Real TT; /*traveltime in [mm/h]*/
  Real HC; /*hydraulic conductivity in [mm/h]*/
  Real influx;
  Real frac_g_influx,soil_infil;
  Real outflux,grunoff,inactive_water[NSOILLAYER];
  Real runoff_surface,freewater,soildepth_irrig,deficit;
  Soil *soil;
  int l;
  Real updated_soil_water=0,previous_soil_water[NSOILLAYER];
  Irrigation *data_irrig;
  data_irrig=stand->data;

  soil=&stand->soil;
  influx=grunoff=perc=frac_g_influx=freewater=0.0;
  runoff_surface=runoff=outflux=0;
  soil_infil=2;

  /*infiltration*/
  if(rw_manage)
    if(stand->type->landusetype==AGRICULTURE || stand->type->landusetype==GRASSLAND || stand->type->landusetype==BIOMASS_GRASS || stand->type->landusetype==BIOMASS_TREE)
      soil_infil=param.soil_infil; /* parameter to increase soil infiltration rate */
  if(soil_infil<2)
    soil_infil=2;

  for(l=0;l<NSOILLAYER;l++)
  {
    freewater+=soil->w_fw[l];
    if (soil->w[l]+soil->ice_depth[l]/soil->par->whcs[l]>1)
      freewater+=(soil->w[l]+soil->ice_depth[l]/soil->par->whcs[l]-1)*soil->par->whcs[l];
  }

  while(infil > epsilon || freewater > epsilon)
  {
    freewater=0.0;

    slug=min(4,infil);
    infil=infil-slug;
    if(data_irrig->irrig_system==SPRINK || data_irrig->irrig_system==DRIP)
      influx=slug;        /*no surface runoff for DRIP and Sprinkler*/
    else
      influx=slug*pow(1-(soil->w[0]*soil->par->whcs[0]+soil->w_fw[0]+soil->ice_depth[0]+soil->ice_fw[0])/(soil->par->wsats[0]-soil->par->wpwps[0]),(1/soil_infil));
    runoff_surface+=slug - influx;
    *return_flow_b+=slug - influx;
    frac_g_influx=0; /* first layer has only blue influx, but lower layers with percolation have mixed frac_g_influx */

    if(data_irrig->irrig_system==DRIP)
    {
      /* in case of Drip: directly fill up field cap of first two soil layers, no surface runoff, lateral runoff or percolation */
      /* -> this allows simulating perfect irrigation: drip + irrg_threshold = 1 (keep in mind: plant can still be somewhat stressed, if roots go deeper than 2. layer) */
      for(l=0;l<LASTLAYER && influx>epsilon;l++)
      {
        previous_soil_water[l]=soil->w[l]*soil->par->whcs[l]+soil->ice_depth[l]+soil->w_fw[l]+soil->ice_fw[l];
        soil->w[l]+=influx/soil->par->whcs[l];
        influx=max((soil->w[l]-1)*soil->par->whcs[l]+soil->ice_depth[l],0);
        soil->w[l]=min(soil->w[l],1-soil->ice_depth[l]/soil->par->whcs[l]);

        /*update frac_g: new green fraction equals old green amount + new green amount divided by total water */
        updated_soil_water=soil->w[l]*soil->par->whcs[l]+soil->ice_depth[l]+soil->w_fw[l]+soil->ice_fw[l];
        if(updated_soil_water>previous_soil_water[l] && updated_soil_water>0)
          stand->frac_g[l]=(previous_soil_water[l]*stand->frac_g[l])/updated_soil_water;

      } /* soil layer loop */
      outflux+=influx;
      *return_flow_b+=influx;
      influx=0.0;
    }
    else
    {
      /* Sprinkler and Surface water infiltration */
      for(l=0;l<NSOILLAYER;l++)
      {
        previous_soil_water[l]=soil->w[l]*soil->par->whcs[l]+soil->ice_depth[l]+soil->w_fw[l]+soil->ice_fw[l];
        soil->w[l]+=(soil->w_fw[l]+influx)/soil->par->whcs[l];
        soil->w_fw[l]=0.0;
        influx=0.0;
        inactive_water[l]=soil->ice_depth[l]+soil->par->wpwps[l]+soil->ice_fw[l];

        /*update frac_g to influx */
        updated_soil_water=soil->w[l]*soil->par->whcs[l]+soil->ice_depth[l]+soil->w_fw[l]+soil->ice_fw[l];
        if(previous_soil_water[l]-updated_soil_water>epsilon)
        {
          fprintf(stderr,"Pixel: %.2f %.2f infil_perc_irr.c error updated smaller then previous --- updated=  %3.12f --- previous=  %3.12f --- diff=  %3.12f\n",stand->cell->coord.lat,stand->cell->coord.lon,updated_soil_water,previous_soil_water[l],updated_soil_water-previous_soil_water[l]);
          fflush(stdout);
        }
        if(updated_soil_water>previous_soil_water[l] && updated_soil_water>0)
          stand->frac_g[l]=(previous_soil_water[l]*stand->frac_g[l] + (updated_soil_water - previous_soil_water[l])*frac_g_influx)/updated_soil_water; /* new green fraction equals old green amount + new green amount divided by total water */

        /* lateral runoff of water above saturation */
        if ((soil->w[l]*soil->par->whcs[l])>(soildepth[l]-soil->freeze_depth[l])*(soil->par->wsat-soil->par->wpwp))
        {
          grunoff=(soil->w[l]*soil->par->whcs[l])-((soildepth[l]-soil->freeze_depth[l])*(soil->par->wsat-soil->par->wpwp));
          soil->w[l]-=grunoff/soil->par->whcs[l];
          runoff+=grunoff;
          *return_flow_b+=grunoff*(1-stand->frac_g[l]);
        }
        if((inactive_water[l]+soil->w[l]*soil->par->whcs[l])>soil->par->wsats[l])
        {
          grunoff=(inactive_water[l]+soil->w[l]*soil->par->whcs[l])-soil->par->wsats[l];
          soil->w[l]-=grunoff/soil->par->whcs[l];
          runoff+=grunoff;
          *return_flow_b+=grunoff*(1-stand->frac_g[l]);
        }

        if (soildepth[l]>soil->freeze_depth[l])
        {
          /*percolation*/
          if((soil->w[l]+soil->ice_depth[l]/soil->par->whcs[l]-1)>epsilon)
          {
            HC=soil->par->Ks*pow(((soil->w[l]*soil->par->whcs[l]+inactive_water[l])/soil->par->wsats[l]),soil->par->beta_soil);
            TT=((soil->w[l]-1)*soil->par->whcs[l]+soil->ice_depth[l])/HC;
            perc=((soil->w[l]-1)*soil->par->whcs[l]+soil->ice_depth[l])*(1-exp(-24/TT));
            /*correction of percolation for water content of the following layer*/
            if (l<BOTTOMLAYER)
              perc=perc*sqrt(1-(soil->w[l+1]*soil->par->whcs[l+1]+soil->w_fw[l+1]+soil->ice_depth[l+1]+soil->ice_fw[l+1])/(soil->par->wsats[l+1]-soil->par->wpwps[l+1]));
#ifdef SAFE
            if (perc< 0)
              printf("perc<0 ; TT %3.3f HC %3.3f perc  %3.3f w[%d]  %3.7f\n",TT,HC,perc/soil->par->whcs[l],l,soil->w[l]);
            if (perc/soil->par->whcs[l]>(soil->w[l]+epsilon))
              printf("perc>w ; Pixel: %.2f %.2f; TT %3.3f HC %3.3f perc  %3.7f w[%d]  %3.7f\n",
                     stand->cell->coord.lat,stand->cell->coord.lon,TT,HC,perc/soil->par->whcs[l],l,soil->w[l]);
#endif
            soil->w[l]-=perc/soil->par->whcs[l];

            if (fabs(soil->w[l])< epsilon)
            {
              perc+=(soil->w[l])*soil->par->whcs[l];
              soil->w[l]=0;
            }
            if(l==BOTTOMLAYER)
            {
              outflux+=perc;
              *return_flow_b+=perc*(1-stand->frac_g[l]);
            }
            else
            {
              influx=perc;
              frac_g_influx=stand->frac_g[l];
              soil->perc_energy[l+1]=((soil->temp[l]-soil->temp[l+1])*perc*1e-3)*c_water;
            }
          } /*end percolation*/
        } /* if soil depth > freeze_depth */
      } /* soil layer loop */
    } /* if not drip */
  } /* while infil > 0 */

  for(l=0;l<NSOILLAYER;l++)
  {
    /*reallocate water above field capacity to freewater */
    if (soil->w[l]+soil->ice_depth[l]/soil->par->whcs[l]>1)
    {
      soil->w_fw[l]=(soil->w[l]+soil->ice_depth[l]/soil->par->whcs[l]-1)*soil->par->whcs[l];
      soil->w[l]-=soil->w_fw[l]/soil->par->whcs[l];
    }
    if (fabs(soil->w_fw[l])<epsilon)
      soil->w_fw[l]=0;
    if (fabs(soil->w[l])<epsilon)
      soil->w[l]=0;
    if(soil->w[l]*soil->par->whcs[l]+soil->ice_depth[l]+soil->w_fw[l]+soil->ice_fw[l]<epsilon)
      stand->frac_g[l]=1.0;
#ifdef SAFE
    if(stand->frac_g[l]<(-0.01) || stand->frac_g[l]>(1.01))
    {
      fprintf(stderr,"Pixel: %.2f %.2f infil perc frac_g error frac_g=  %3.12f standtype= %s\n",stand->cell->coord.lat,stand->cell->coord.lon,stand->frac_g[l],stand->type->name);
      fflush(stdout);
    }
   if (soil->w[l]<0)
   {
    fprintf(stderr,"Pixel: %.2f %.2f icedepth[%d]= %3.8f fw_ice= %.6f w_fw=%.6f w=%.6f soilwater=%.6f wsats=%.6f whcs=%f\n",
            stand->cell->coord.lat,stand->cell->coord.lon,l,soil->ice_depth[l],soil->ice_fw[l],soil->w_fw[l],soil->w[l]*soil->par->whcs[l],
            allwater(soil,l)+allice(soil,l),soil->par->wsats[l],soil->par->whcs[l]);
    fflush(stdout);
    fail(NEGATIVE_SOIL_MOISTURE_ERR,TRUE,
         "Pixel: %f %f Soil-moisture %d negative: %g, lutype %s soil_type %s in infil_perc_irr\n",
         stand->cell->coord.lat,stand->cell->coord.lon,l,soil->w[l],stand->type->name,soil->par->name);
   }
#endif
  } /* soil layer loop */

  /* evaluate soil water deficit in upper 50cm (irrigation depth) after irrigation event */
  soildepth_irrig=SOILDEPTH_IRRIG;
  l=0;
  deficit=0.0;
  do
  {
    if (stand->soil.freeze_depth[l]< soildepth[l])
      deficit+=max(0,(1-stand->soil.w[l]-stand->soil.ice_depth[l]/stand->soil.par->whcs[l])*stand->soil.par->whcs[l]*min(1,soildepth_irrig/soildepth[l])*(1-stand->soil.freeze_depth[l]/soildepth[l]));
    l++;
  }while((soildepth_irrig-=soildepth[l-1])>0);

  stand->cell->output.munmet_demand+=deficit; /* daily irrigation deficit in mm*/

  /* write outputs */
  stand->cell->output.mseepage+=outflux*stand->frac; /* bottom layer percolation*/
  stand->cell->output.mrunoff_lat+=runoff*stand->frac; /* lateral runoff */
  stand->cell->output.mrunoff_surf+=runoff_surface*stand->frac; /* surface runoff */

  return runoff+outflux+runoff_surface;

} /* of 'infil_perc_irr' */
