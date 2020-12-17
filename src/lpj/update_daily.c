/***************************************************************************/
/**                                                                       **/
/**                u  p  d  a  t  e  _  d  a  i  l  y  .  c               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function of daily update of individual grid cell                  **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     See file AUTHORS for list of authors contributing to this code    **/
/**                                                                       **/
/**     Last change: $Date:: 2015-11-06 11:17:06 +0100 (ven., 06 nov. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "natural.h"
#include "agriculture.h"

void update_daily(Cell *cell,            /* cell pointer           */
        Real co2,              /* atmospheric CO2 (ppmv) */
        Real popdensity,       /* population density (capita/km2) */
        Dailyclimate climate,  /* Daily climate values */
        int day,               /* day (1..365)           */
        int npft,              /* number of natural PFTs */
        int ncft,              /* number of crop PFTs   */
        int year,              /* simulation year */
        int month,             /* month (0..11) */
        Bool withdailyoutput,
        Bool intercrop,
        const Config *config,   /* LPJ config */
        int cellnumber
        )
{
    int p,s;
    Real melt=0,pet,par,daylength;
    Real gp_stand,gp_stand_leafon,runoff,snowrunoff;
    Real fpc_total_stand;
    Real gtemp_air;  /* value of air temperature response function */
    Real gtemp_soil[NSOILLAYER]; /* value of soil temperature response function */
    Real temp_bs;    /* temperature beneath snow */
    Real prec_energy; /* energy from temperature difference between rain and soil [J/m2]*/
    Real flux_estab=0;
    Real evap=0,irrig_water=0;
    Real hetres=0;
    Real *gp_pft;
    Stand *stand;
    Pft *pft;
    //Irrigation *data;
    int l;
    Livefuel livefuel={0,0,0,0,0};
    const Real prec_save=climate.prec;

    gp_pft=newvec(Real,npft+ncft);
    check(gp_pft);
    switch(config->with_radiation)
    {
        case CLOUDINESS:
            petpar(&daylength,&par,&pet,cell->coord.lat,day,climate.temp,climate.sun);
            break;
        case RADIATION:
            petpar2(&daylength,&par,&pet,cell->coord.lat,day,climate.temp,
                    climate.lwnet,climate.swdown,FALSE);
            break;
        case RADIATION_LWDOWN:
            petpar2(&daylength,&par,&pet,cell->coord.lat,day,climate.temp,
                    climate.lwnet,climate.swdown,TRUE);
            break;
        case RADIATION_SWONLY:
            petpar3(&daylength,&par,&pet,cell->coord.lat,day,climate.temp,
                    climate.swdown);
            break;
    }
    cell->output.mpet+=pet*param.ALPHAM;

#ifdef DEBUG3
    printf("par= %.2f  pet= %.5f daylength= %.5f\n",par,pet,daylength);
    printf("temp= %.2f sun= %.2f\n",climate.temp,climate.sun);
    fflush(stdout);
#endif
    updategdd(cell->gdd,config->pftpar,npft,climate.temp);
    cell->balance.aprec+=climate.prec;
    gtemp_air=temp_response(climate.temp);
    daily_climbuf(&cell->climbuf,climate.temp);

    cell->output.mprec+=climate.prec;
    cell->output.msnowf+=climate.temp<tsnow ? climate.prec : 0;
    cell->output.mrain+=climate.temp<tsnow ? 0 : climate.prec;

    if(cell->ml.landfrac!=NULL) /* landuse enabled? */
    {
        if(config->sdate_option==NO_FIXED_SDATE ||
                (config->sdate_option==FIXED_SDATE && year<=param.sdate_fixyear))
        {
            update_fallowdays(cell->ml.cropdates,cell->coord.lat,day,ncft);
            /* calling reduced calc_cropdates for computing vern_date20 as needed for vernalization */
            calc_cropdates(config->pftpar+npft,&cell->climbuf,cell->ml.cropdates,cell->coord.lat,
                    day,ncft);

            flux_estab=sowing_season(cell,day,npft,ncft,climate.prec,config);
        }
        else
            flux_estab=sowing_prescribe(cell,day,npft,ncft,config);
    }

    cell->discharge.drunoff=0.0;

    /* barrier.n: initialisation of runoff to 0 */
    cell->discharge.NO3runoff=0.0;
    cell->discharge.PO4runoff=0.0;
    cell->discharge.PO4adsrunoff=0.0;
    cell->discharge.NH4runoff=0.0;
    cell->discharge.DOCrunoff=0.0;
    cell->discharge.DONrunoff=0.0;
    cell->discharge.DOPrunoff=0.0;

    if(config->fire==SPITFIRE)
    {
        if (climate.prec >= 3.0 || (climate.tmin - 4.0) <= 0) /* benp, kirsten */
            cell->ignition.nesterov_accum=0;
        else
            cell->ignition.nesterov_accum += nesterovindex(climate.tmin,climate.tmax);
    }

    foreachstand(stand,s,cell->standlist)
    {
        // barrier.n: initialisation of uptake and residue for the current day
        // to 0 for each pft

        nit_init_residue_daily(stand);
        nit_init_uptake_daily(stand);


        if(config->fire==SPITFIRE && cell->afire_frac<1)
            dailyfire_stand(stand,&livefuel,popdensity,climate,config->ntypes);
        if(config->permafrost)
        {
            snowrunoff=snow(&stand->soil,&climate.prec,&melt,
                    climate.temp,&temp_bs,&evap)*stand->frac;
            cell->discharge.drunoff+=snowrunoff;
            cell->output.mevap+=evap*stand->frac; /* evap from snow runoff*/
            prec_energy = ((climate.temp-stand->soil.temp[TOPLAYER])*climate.prec*1e-3
                    +melt*1e-3*(T_zero-stand->soil.temp[TOPLAYER])+irrig_water)*c_water;
            stand->soil.perc_energy[TOPLAYER]=prec_energy;
            /*THIS IS DEDICATED TO MICROBIOLOGICAL HEATING*/
            /*     foreachsoillayer(l) stand->soil.micro_heating[l]=m_heat*stand->soil.decomC[l];
                   stand->soil.micro_heating[0]+=m_heat*stand->soil.litter.decomC;*/

            soiltemp(&stand->soil,temp_bs);
            foreachsoillayer(l)
                gtemp_soil[l]=temp_response(stand->soil.temp[l]);
        }
        else
        {
            gtemp_soil[0]=temp_response(soiltemp_lag(&stand->soil,&cell->climbuf));
            for(l=1;l<NSOILLAYER;l++)
                gtemp_soil[l]=gtemp_soil[0];
            snowrunoff=snow_old(&stand->soil.snowpack,&climate.prec,&melt,climate.temp)*stand->frac;
            cell->discharge.drunoff+=snowrunoff;
        }
        cell->output.msoiltemp1+=stand->soil.temp[0]*ndaymonth1[month]*stand->frac*(1.0/(1-stand->cell->lakefrac));
        cell->output.msoiltemp2+=stand->soil.temp[1]*ndaymonth1[month]*stand->frac*(1.0/(1-stand->cell->lakefrac));
        cell->output.msoiltemp3+=stand->soil.temp[2]*ndaymonth1[month]*stand->frac*(1.0/(1-stand->cell->lakefrac));
        cell->output.msoiltemp4+=stand->soil.temp[3]*ndaymonth1[month]*stand->frac*(1.0/(1-stand->cell->lakefrac));
        cell->output.msoiltemp5+=stand->soil.temp[4]*ndaymonth1[month]*stand->frac*(1.0/(1-stand->cell->lakefrac));
        cell->output.msoiltemp6+=stand->soil.temp[5]*ndaymonth1[month]*stand->frac*(1.0/(1-stand->cell->lakefrac));

        hetres=littersom(&stand->soil,gtemp_soil);
        cell->output.mrh+=hetres*stand->frac;
        cell->output.dcflux+=hetres*stand->frac;
        if (withdailyoutput)
        {
            switch(stand->type->landusetype)
            {
                case GRASSLAND:
                    if (cell->output.daily.cft == C3_PERENNIAL_GRASS)
                    {
                        cell->output.daily.rh  += hetres;
                        cell->output.daily.swe += stand->soil.snowpack;
                    }
                    break;
                case AGRICULTURE:
                    foreachpft(pft,p,&stand->pftlist)
                        if (pft->par->id == cell->output.daily.cft)
                        {
                            cell->output.daily.rh  = hetres;
                            cell->output.daily.swe = stand->soil.snowpack;
                        }
                    break;
                case NATURAL:
                    if (cell->output.daily.cft == ALLNATURAL)
                    {
                        cell->output.daily.rh  += hetres;
                        cell->output.daily.swe += stand->soil.snowpack;
                    }
                    break;
            } /* of switch() */
        }

        cell->output.msnowrunoff+=snowrunoff;
        cell->output.mmelt+=melt*stand->frac;

        if(config->fire==FIRE && climate.temp>0)
            stand->fire_sum+=fire_sum(&stand->soil.litter,stand->soil.w[0]);

        if (config->with_nutdrainanddep)
        {
            //atmosphere N deposition
            //adding daily N deposition to upper soil layer 1 g/m2/year
            //stand->soil.NO3[0]+= (1./365.);
        }

        gp_stand=gp_sum(&stand->pftlist,co2,climate.temp,par,daylength,
                &gp_stand_leafon,gp_pft,&fpc_total_stand);


        runoff=daily_stand(stand,co2,climate,day,daylength,gp_pft,
                gtemp_air,gtemp_soil[0],gp_stand,gp_stand_leafon,pet,par,
                melt,npft,ncft,year,withdailyoutput,intercrop,config,cellnumber);

        /* barrier.n: uptake of N/P and adding to litter */
        nit_uptake_allpft(stand,day);

        nit_remin_allpft(stand);

        //barrier.n: transfer of resc/resp residues into
        // NO3, NH4, PO4
        nit_immo_soil(&stand->soil,stand->type->landusetype, stand->type->name);

        // adsorption in soil
        nit_soil_adsorption(stand);

        /* barrier.n: adding denitrification */
        //nit_denitrification(stand);//removed because we don't have N deposition which compensate for this loss ?

        /* barrier.n: adding nitrification */
        nit_nitrification(stand);

        cell->discharge.drunoff+=runoff*stand->frac;

        climate.prec=prec_save;

     } /* of foreachstand */

    cell->output.mrunoff+=cell->discharge.drunoff;
    cell->output.daily.runoff+=cell->discharge.drunoff;
    /*cell->mdemand-=cell->discharge.act_irrig_amount;*/
    if(config->river_routing)
    {
        /* reservoir waterbalance */
        if(cell->ml.dam)
        {
            cell->ml.resdata->dmass+=climate.prec*cell->coord.area*cell->ml.reservoirfrac;
            cell->output.mprec_res+=climate.prec*cell->ml.reservoirfrac;
            if(cell->ml.resdata->dmass>cell->ml.resdata->reservoir.capacity)
            {
                cell->discharge.dmass_lake+=cell->ml.resdata->dmass-cell->ml.resdata->reservoir.capacity;
                cell->ml.resdata->dmass=cell->ml.resdata->reservoir.capacity;
            }
            cell->output.mevap_res+=min(cell->ml.resdata->dmass/cell->coord.area,pet*param.ALPHAM*cell->ml.reservoirfrac);
            cell->ml.resdata->dmass=max(cell->ml.resdata->dmass-pet*param.ALPHAM*cell->coord.area*cell->ml.reservoirfrac,0.0);
            cell->output.mres_storage+=cell->ml.resdata->dmass;
        }

        /* lake waterbalance */
        cell->discharge.dmass_lake+=climate.prec*cell->coord.area*cell->lakefrac;
        cell->output.input_lake+=climate.prec*cell->coord.area*cell->lakefrac;

        cell->output.mevap_lake+=min(cell->discharge.dmass_lake/cell->coord.area,pet*param.ALPHAM*cell->lakefrac);
        cell->discharge.dmass_lake=max(cell->discharge.dmass_lake-pet*param.ALPHAM*cell->coord.area*cell->lakefrac,0.0);

    } /* of 'if(river_routing)' */

    cell->output.mswe+=cell->output.daily.swe;

    killstand(cell,config->pftpar,npft,intercrop);
#ifdef SAFE
    check_stand_fracs(cell->standlist,cell->lakefrac+cell->ml.reservoirfrac);
#endif
    /* Establishment fluxes are area weighted in subroutines */
    cell->output.flux_estab+=flux_estab;
    cell->output.dcflux-=flux_estab;
    free(gp_pft);


} /* of 'update_daily' */
