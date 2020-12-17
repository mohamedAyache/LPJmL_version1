/***************************************************************************/
/**                                                                       **/
/**       i  n  f  i  l  _  p  e  r  c  _  r  a  i  n  .  c               **/
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
/**     Update Mohamed Ayache                                             **/
/**     Aix-Marseille University                                          **/
/***************************************************************************/

#include "lpj.h"
#include "agriculture.h"

Real infil_perc_rain(Stand *stand,        /* Stand pointer */
        Real infil,          /* rainfall + melting water - interception_stand (mm) + rw_irrig */
        Real *return_flow_b, /* blue water return flow (mm) */
        Bool rw_manage,	  /* do rain water management? (TRUE/FALSE) */
        const Config *config,
        int cellnumber
        )                     /* returns water runoff (mm) */
{
    Real runoff;
    Real perc,slug;
    Real TT; /*traveltime in [mm/h]*/
    Real HC; /*hydraulic conductivity in [mm/h]*/
    Real influx;
    Real frac_g_influx;
    Real outflux,grunoff,inactive_water[NSOILLAYER];
    Real runoff_surface,freewater,soil_infil;
    Soil *soil;
    int l;
    Real updated_soil_water=0,previous_soil_water[NSOILLAYER];
    Irrigation *data_irrig;


    /* barrier.n */
    Real srunoff;
    Real w_mobile=0; /* Water mixing with nutrient in layer mmH2O */
    Real nitleachNO3=0; /* Output of the infil_nut_irr function (used for saving) */
    Real nitleachPO4=0; /* Output of the infil_nut_irr function (used for saving) */
    Real nitleachNH4=0; /* Output of the infil_nut_irr function (used for saving) */
    Real NO3perc_ly = 0;
    Real PO4perc_ly = 0;
    Real NH4perc_ly = 0;
    Real DOCperc_ly = 0;
    Real DONperc_ly = 0;
    Real DOPperc_ly = 0;

    data_irrig=stand->data;

    soil=&stand->soil;
    influx=grunoff=perc=frac_g_influx=freewater=0.0;
    runoff_surface=runoff=outflux=0;
    soil_infil=2; /* default to draw square root for infiltration factor*/

    /*infiltration*/
    if(rw_manage)
        if(stand->type->landusetype==AGRICULTURE || stand->type->landusetype==GRASSLAND || stand->type->landusetype==BIOMASS_GRASS || stand->type->landusetype==BIOMASS_TREE)
            soil_infil=param.soil_infil; /* parameter to increase soil infiltration rate */
    if(soil_infil<2)
        soil_infil=2;

    for(l=0;l<NSOILLAYER;l++)
    {
        previous_soil_water[l]=soil->w[l]*soil->par->whcs[l]+soil->ice_depth[l]+soil->w_fw[l]+soil->ice_fw[l];
        freewater+=soil->w_fw[l];
        if (soil->w[l]+soil->ice_depth[l]/soil->par->whcs[l]>1)
            freewater+=(soil->w[l]+soil->ice_depth[l]/soil->par->whcs[l]-1)*soil->par->whcs[l];
    }
    while(infil > epsilon || freewater > epsilon)
    {
        /* barrier.n: initialisation of the percolation values for each nutrients */
        NO3perc_ly= PO4perc_ly = NH4perc_ly = DOCperc_ly = DONperc_ly = DOPperc_ly = 0.0; /* barrier.n */
        freewater=0.0;

        slug=infil;
        infil=infil-slug;
        influx=slug*pow(1-(soil->w[0]*soil->par->whcs[0]+soil->w_fw[0]+soil->ice_depth[0]+soil->ice_fw[0])/(soil->par->wsats[0]-soil->par->wpwps[0]),(1/soil_infil));
        runoff_surface += slug - influx;
        srunoff = slug - influx;
        frac_g_influx=1; /* first layer has only green influx, but lower layers with percolation have mixed frac_g_influx */

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
                fprintf(stderr,"Pixel: %.2f %.2f infil_perc_rain.c error updated smaller then previous --- updated=  %3.12f --- previous=  %3.12f --- diff=  %3.12f\n",stand->cell->coord.lat,stand->cell->coord.lon,updated_soil_water,previous_soil_water[l],updated_soil_water-previous_soil_water[l]);
                fflush(stdout);
            }
            if(updated_soil_water>previous_soil_water[l] && updated_soil_water>0)
            {
                stand->frac_g[l]=(previous_soil_water[l]*stand->frac_g[l] + (updated_soil_water - previous_soil_water[l])*frac_g_influx)/updated_soil_water; /* new green fraction equals old green amount + new green amount divided by total water */
            }

            /* lateral runoff of water above saturation */
            if ((soil->w[l]*soil->par->whcs[l])>(soildepth[l]-soil->freeze_depth[l])*(soil->par->wsat-soil->par->wpwp))
            {
                grunoff=(soil->w[l]*soil->par->whcs[l])-((soildepth[l]-soil->freeze_depth[l])*(soil->par->wsat-soil->par->wpwp));
                soil->w[l]-=grunoff/soil->par->whcs[l];
                runoff+=grunoff;
                *return_flow_b+=grunoff*(1-stand->frac_g[l]);
            }
            /*needed here??? -> Only if (soildepth[l]-soil->freeze_depth[l])*(soil->par->wsat[l]-soil->par->wpwp[l])!=(soil->ice_depth[l]+soil->ice_fw[l])*/
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

                    w_mobile = 0;
                    w_mobile = perc + srunoff + grunoff;

                    if (l<LASTLAYER -1) //avoid index errors
                        {




                      if (config->with_nutdrainanddep)
                      {

                           nitleachNO3 = nit_infilNO3(l, stand, &soil->NO3[l], &NO3perc_ly, w_mobile, srunoff, runoff, perc, soil->par);
                           stand->cell->discharge.NO3runoff += nitleachNO3*stand->frac;//we multiply by stand->frac because nitleachNO3 is a value for the stand only whereas NO3runoff is a value for the whole cell

                          nitleachPO4 = nit_infilPO4(l, stand, &soil->PO4[l], srunoff);
                          stand->cell->discharge.PO4runoff += nitleachPO4*stand->frac;
                      }

                            nitleachNH4 = nit_infilNH4(l, stand, &soil->NH4[l], &NH4perc_ly, w_mobile, srunoff, runoff, perc, soil->par);
                            stand->cell->discharge.NH4runoff += nitleachNH4*stand->frac;

/*
                            nitleach = nit_infil(l, stand, &soil->DON[l], &DONperc_ly, w_mobile, srunoff, runoff, perc, soil->par);
                            stand->cell->discharge.DONrunoff += nitleach*stand->frac;

                            nitleach = nit_infil(l, stand, &soil->DOC[l], &DOCperc_ly, w_mobile, srunoff, runoff, perc, soil->par);
                            stand->cell->discharge.DOCrunoff += nitleach*stand->frac;

                            nitleach = nit_infil(l, stand, &soil->DOP[l], &DOPperc_ly, w_mobile, srunoff, runoff, perc, soil->par);
                            stand->cell->discharge.DOPrunoff += nitleach*stand->frac;*/

                            srunoff=0.0; /* not used for lower soil layers */
                        }
                        if (l == BOTTOMLAYER) //we skip the LASTLAYER-1 to match the water runoff and the nutrient runoff

                        {



                      if (config->with_nutdrainanddep)
                      {
                           nitleachNO3 = nit_infilNO3(l-1, stand, &soil->NO3[l-1], &NO3perc_ly, w_mobile, srunoff, runoff, perc, soil->par);
                      }


                            nitleachNH4 = nit_infilNH4(l, stand, &soil->NH4[l-1], &NH4perc_ly, w_mobile, srunoff, runoff, perc, soil->par);
                            stand->cell->discharge.NH4runoff += nitleachNH4*stand->frac;
/*
                            nitleach = nit_infil(l, stand, &soil->DON[l-1], &DONperc_ly, w_mobile, srunoff, runoff, perc, soil->par);
                            stand->cell->discharge.DONrunoff += nitleach*stand->frac;

                            nitleach = nit_infil(l, stand, &soil->DOC[l-1], &DOCperc_ly, w_mobile, srunoff, runoff, perc, soil->par);
                            stand->cell->discharge.DOCrunoff += nitleach*stand->frac;

                            nitleach = nit_infil(l, stand, &soil->DOP[l-1], &DOPperc_ly, w_mobile, srunoff, runoff, perc, soil->par);
                            stand->cell->discharge.DOPrunoff += nitleach*stand->frac;*/

                           srunoff=0.0; /* not used for lower soil layers */
                        }


                } /*end percolation*/
            } /* if soil depth > freeze_depth */
        } /* soil layer loop */
    if (config->with_nutdrainanddep)
    {

        stand->cell->discharge.NO3runoff += NO3perc_ly*stand->frac;

    }

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
            fprintf(stderr,"Pixel: %.2f %.2f infil= %3.10f icedepth[%d]= %3.8f fw_ice= %.6f w_fw=%.6f w=%.6f soilwater=%.6f wsats=%.6f whcs=%f\n",
                    stand->cell->coord.lat,stand->cell->coord.lon,infil,l,soil->ice_depth[l],soil->ice_fw[l],soil->w_fw[l],soil->w[l]*soil->par->whcs[l],
                    allwater(soil,l)+allice(soil,l),soil->par->wsats[l],soil->par->whcs[l]);
            fflush(stdout);
            fail(NEGATIVE_SOIL_MOISTURE_ERR,TRUE,
                    "Pixel: %f %f Soil-moisture %d negative: %g, lutype %s soil_type %s in infil_perc_rain\n",
                    stand->cell->coord.lat,stand->cell->coord.lon,l,soil->w[l],stand->type->name,soil->par->name);
        }
#endif
    } /* soil layer loop */
    /* Rainwater Harvesting: store part of surface runoff for supplementary irrigation */
    if(rw_manage && ((stand->type->landusetype==AGRICULTURE && !data_irrig->irrigation) || stand->type->landusetype==SETASIDE_RF))
    {
        soil->rw_buffer+=param.frac_ro_stored*runoff_surface;
        runoff_surface-=param.frac_ro_stored*runoff_surface;
        if(soil->rw_buffer > param.rw_buffer_max)
        {
            runoff_surface+=soil->rw_buffer - param.rw_buffer_max;
            soil->rw_buffer=param.rw_buffer_max;
        }
    }

#ifdef SAFE
    /* if(config->with_nitrogen) */
    forrootsoillayer(l){
        if (soil->NO3[l]<-0.01)
            fail(NEGATIVE_SOIL_NO3_ERR,TRUE,"infil_perc_rain: Pixel: %.2f %.2f NO3=%g<0 in layer %d", stand->cell->coord.lat,stand->cell->coord.lon,soil->NO3[l],l);
        if (soil->PO4[l]<-0.01)
            fail(NEGATIVE_SOIL_PO4_ERR,TRUE,"infil_perc_rain: Pixel: %.2f %.2f PO4=%g<0 in layer %d", stand->cell->coord.lat,stand->cell->coord.lon,soil->PO4[l],l);
    }
#endif

    /*writing output*/
    stand->cell->output.mseepage+=outflux*stand->frac;
    stand->cell->output.mrunoff_lat+=runoff*stand->frac;
    stand->cell->output.mrunoff_surf+=runoff_surface*stand->frac;

    return runoff+outflux+runoff_surface;

} /* of 'infil_perc_rain' */
