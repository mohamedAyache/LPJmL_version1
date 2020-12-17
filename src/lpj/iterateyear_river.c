/***************************************************************************/
/**                                                                       **/
/**        i  t  e  r  a  t  e  y  e  a  r  _  r  i  v  e  r  .  c        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function performs iteration over the cell grid for one year with  **/
/**     river routing enabled.                                            **/
/**                                                                       **/
/**     Principal structure:                                              **/
/**                                                                       **/
/**           for(cell=0;cell<config.ngridcell;cell++)                    **/
/**             init_annual();                                            **/
/**           foreachmonth(month)                                         **/
/**           {                                                           **/
/**             foreachdayofmonth(dayofmonth,month)                       **/
/**             {                                                         **/
/**               irrig_amount_river();                                   **/
/**               for(cell=0;cell<config.ngridcell;cell++)                **/
/**                 update_daily();                                       **/
/**               drain();                                                **/
/**             }                                                         **/
/**             for(cell=0;cell<config.ngridcell;cell++)                  **/
/**               update_monthly();                                       **/
/**           for(cell=0;cell<config.ngridcell;cell++)                    **/
/**             update_annual();                                          **/
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

void iterateyear_river(Outputfile *output,  /* Output file data */
        Cell grid[],         /* cell array */
        Input input,         /* input data */
        Real co2,            /* atmospheric CO2 (ppmv) */
        int npft,            /* number of natural PFTs */
        int ncft,            /* number of crop PFTs */
        int year,            /* simulation year (AD) */
        const Config *config, /* LPJ configuration */
        Bool isFirstYear
        )
{
    Dailyclimate daily;
    Bool intercrop,istimber;
    int month,dayofmonth,day;
    int cell,i,s,l;
    Stand *stand;
    Real popdens=0; /* population density (capita/km2) */
#ifdef IMAGE
    istimber=(config->start_imagecoupling!=INT_MAX);
#else
    istimber=FALSE;
#endif
    intercrop=getintercrop(input.landuse);
    for(cell=0;cell<config->ngridcell;cell++)
    {
        //init the uptake save variables need to be put here when starting from restart
        foreachstand(stand,s,grid[cell].standlist)
        {
            for (l=0; l <LASTLAYER; l++) {
            stand->annualUptakeN[l] =0;
            stand->annualUptakeP[l] =0;
            }
            if (isFirstYear)
            {
                nit_init_uptake_annual(stand);
            }

        }

        // update of the NO3was variable, from the file input , useful?
        /*if(config->with_wastewater)
            nit_update_daily_wastewater(grid, cell, config);*/
        if(config->with_manure || config->with_ferti){
            // init the application variables: set the 1st element to 1 and the
            // others to 0.
            nit_init_application(grid + cell, config->npft[CROP], config->nagtree);

        }

        // update of the crop, forest and grass fraction for each cell
        nit_udpdate_frac_for_ads(config, grid + cell);

        grid[cell].output.adischarge=0;
        grid[cell].output.surface_storage=0;
        if(!grid[cell].skip)
        {
            init_annual(grid+cell,npft,config->nbiomass,config->nagtree,ncft);
            if(input.landuse!=NULL)
            {
                if(grid[cell].lakefrac<1)
                {
                    /* calculate landuse change */
                    if(!config->isconstlai)
                        laimax_manage(&grid[cell].ml.manage,config->pftpar+npft,npft,ncft,year);
                    if(year>config->firstyear-config->nspinup)
                        landusechange(grid+cell,config->pftpar,npft,ncft,config->nagtree,config->ntypes,
                                intercrop,istimber,year,config->pft_output_scaled);
                    else if(grid[cell].ml.dam)
                        landusechange_for_reservoir(grid+cell,config->pftpar,npft,istimber,
                                intercrop,ncft);
                }
#ifdef IMAGE
                setoutput_image(grid+cell,ncft);
#endif
            }
            initgdd(grid[cell].gdd,npft);
        } /*gridcell skipped*/
    } /* of for(cell=...) */

    day=1;
    foreachmonth(month)
    {
        for(cell=0;cell<config->ngridcell;cell++)
        {

            grid[cell].discharge.mfin=grid[cell].discharge.mfout=grid[cell].output.mdischarge=grid[cell].output.mwateramount=grid[cell].ml.mdemand=0.0;

            if(!grid[cell].skip)
            {
                initoutput_monthly(&((grid+cell)->output));
                /* Initialize random seed */
                if(israndomprec(input.climate))
                    srand48(config->seed+(config->startgrid+cell)*year*month);
                initclimate_monthly(input.climate,&grid[cell].climbuf,cell,month);

#ifdef IMAGE
                monthlyoutput_image(&grid[cell].output,input.climate,cell,month);
#endif

#ifdef DEBUG
           /*     printf("temp = %.2f prec = %.2f wet = %.2f",
                        (getcelltemp(input.climate,cell))[month],
                        (getcellprec(input.climate,cell))[month],
                        (israndomprec(input.climate)) ? (getcellwet(input.climate,cell))[month] : 0);
                if(config->with_radiation)
                {
                    if(config->with_radiation==RADIATION)
                        printf("lwnet = %.2f ",(getcelllwnet(input.climate,cell))[month]);
                    else if(config->with_radiation==RADIATION_LWDOWN)
                        printf("lwdown = %.2f ",(getcelllwnet(input.climate,cell))[month]);
                    printf("swdown = %.2f\n",(getcellswdown(input.climate,cell))[month]);
                }
                else
                    printf("sun = %.2f\n",(getcellsun(input.climate,cell))[month]);*/
#endif
            }
        } /* of 'for(cell=...)' */

        // daily loop
        foreachdayofmonth(dayofmonth,month)
        {

            Real totalDailyNferti=0;
            for(cell=0;cell<config->ngridcell;cell++)
            {

                // update of the NO3was variable, from the file input
                if(config->with_wastewater)
                nit_update_daily_wastewater(grid, cell, config);

                if(!grid[cell].skip)
                {
                    if(config->ispopulation)
                        popdens=getpopdens(input.popdens,cell);
                    grid[cell].output.dcflux=0;
                    initoutput_daily(&(grid[cell].output.daily));
                    /* get daily values for temperature, precipitation and sunshine */
                    dailyclimate(&daily,input.climate,&grid[cell].climbuf,cell,day,
                            month,dayofmonth);
                    /* get daily values for temperature, precipitation and sunshine */
                    grid[cell].output.daily.temp=daily.temp;
                    grid[cell].output.daily.prec=daily.prec;
                    grid[cell].output.daily.sun=daily.sun;

                    // barrier.n: update of the preccheck array in order to determine
                    // if the last three days have been rainy
                    nit_update_preccheck(grid + cell, daily.prec);


                    if(config->with_ferti)
                        // apply fertilizers on the current cell
                        {
                            nit_fert(config, grid+cell, day, fertdata->fert_n[cell], fertdata->fert_p[cell], year, cell);
                        }

                    if(config->with_manure)
                        // apply manure on the current cell
                       {
                           nit_manure(config, grid+cell, day, manuredata->man_ann_past_n[cell], manuredata->man_ann_crop_n[cell], manuredata->man_sedac[cell], year, cell);
                        }

                    update_daily(grid+cell,co2,popdens,daily,day,npft,
                            ncft,year,month,output->withdaily,intercrop,config,cell);

                }

            }
            if(config->river_routing)
            {
                if(input.landuse!=NULL || input.wateruse!=NULL)
                    withdrawal_demand(grid,config);

                drain(grid,config,month);

                if(input.landuse!=NULL || input.wateruse!=NULL)
                    wateruse(grid,npft,ncft,config);
            }


            if(output->withdaily && year>=config->firstyear)
                fwriteoutput_daily(output,grid,day-1,year,config);


            day++;
        } /* of 'foreachdayofmonth */

        Real sumPrec =0;

        /* Calculate resdata->mdemand as sum of ddemand to reservoir, instead of the sum of evaporation deficits per cell*/
        for(cell=0;cell<config->ngridcell;cell++)
        {
            if(grid[cell].discharge.next<0)
                grid[cell].output.adischarge+=grid[cell].output.mdischarge; /* only endcell outflow */
            grid[cell].output.mdischarge*=1e-9/ndaymonth[month]; /* daily mean discharge per month in 1.000.000 m3 per cell */
            grid[cell].output.mres_storage*=1e-9/ndaymonth[month]; /* hb 5-11-09 mean monthly reservoir storage in 1.000.000 m3 per cell */
            grid[cell].output.mwateramount*=1e-9/ndaymonth[month]; /* mean wateramount per month in 1.000.000 m3 per cell */

            if(!grid[cell].skip)
                {
                update_monthly(grid+cell,getmtemp(input.climate,&grid[cell].climbuf,
                            cell,month),getmprec(input.climate,&grid[cell].climbuf,
                                cell,month),month);
                nit_update_outputs(&grid[cell],cell);//save the monthly nitrogen outputs

                //sum if the monthly precipitation
                sumPrec+= grid[cell].output.mprec*0.001*grid[cell].coord.area/ndaymonth[month]/24/60/60 ;//m3/s

                }


        } /* of 'for(cell=0;...)' */

        Real totalNdis=0;
        Real totalFerti=0;


#ifdef IMAGE
        if(year>=config->firstyear-istimber*10)
#else
            //if(year>=config->firstyear) barrier.n
            if(1)
#endif
                /* write out monthly output */
                fwriteoutput_monthly(output,grid,month,year,config);

    } /* of 'foreachmonth */

    for(cell=0;cell<config->ngridcell;cell++)
    {
        if(!grid[cell].skip)
        {

            update_annual(grid+cell,npft,ncft,popdens,year,intercrop,config);

#ifdef SAFE
            check_fluxes(grid+cell,year,cell,config);
#endif

#ifdef DEBUG
         /*   if(year>config->firstyear)
            {
                printf("year=%d\n",year);
                printf("cell=%d\n",cell);
                printcell(grid+cell,1,ncft,input.landuse!=NULL,TRUE);
            }*/
#endif
            if(config->nspinup>veg_equil_year &&
                    year==config->firstyear-config->nspinup+veg_equil_year && !config->from_restart)
                equilveg(grid+cell);

            if(config->nspinup>soil_equil_year &&
                    year==config->firstyear-config->nspinup+soil_equil_year && !config->from_restart)
                equilsom(grid+cell,npft+ncft,config->pftpar);

        }

        grid[cell].output.surface_storage=grid[cell].discharge.dmass_lake+grid[cell].discharge.dmass_river;
        if(grid[cell].ml.dam)
        {
            grid[cell].output.surface_storage+=grid[cell].ml.resdata->dmass;
            for(i=0;i<NIRRIGDAYS;i++)
                grid[cell].output.surface_storage+=grid[cell].ml.resdata->dfout_irrigation_daily[i];
        }
    } /* of for(cell=0,...) */

#ifdef IMAGE
    if(year>=config->firstyear-istimber*10)
#else
        //if(year>=config->firstyear) barrier.n
        if(1)
#endif
        {
            /* write out annual output */
            fwriteoutput_annual(output,grid,year,config);
            fwriteoutput_pft(output,grid,npft,ncft,year,config);
        }
} /* of 'iterateyear_river' */
