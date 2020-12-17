/***************************************************************************/
/**                                                                       **/
/**                      i  t  e  r  a  t  e  .  c                        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function iterate provides main time loop for LPJ                  **/
/**     In each year iterateyear/iterateyear_river is called              **/
/**                                                                       **/
/**     Prinicpal structure                                               **/
/**                                                                       **/
/**     for(year=...)                                                     **/
/**     {                                                                 **/
/**       co2=getco2();                                                   **/
/**       getclimate();                                                   **/
/**       if(landuse) getlanduse();                                       **/
/**       if(wateruse) getwateruse();                                     **/
/**       if(output->method==LPJ_SOCKET || river_routing)                 **/
/**         iterateyear_river();                                          **/
/**       else                                                            **/
/**         iterateyear();                                                **/
/**       flux_sum();                                                     **/
/**       if(year==config->restartyear)                                   **/
/**         fwriterestart();                                              **/
/**     }                                                                 **/
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
#include "crop.h"

int iterate(Outputfile *output,  /* Output file data */
        Cell grid[],         /* cell grid array */
        Input input,         /* input data: climate, land use, water use */
        int npft,            /* Number of natural PFT's */
        int ncft,            /* Number of crop PFT's */
        const Config *config /* LPJ configuration data */
        )                     /* returns last year+1 on success */
{
    Real co2,cflux_total;
    Flux flux;
    int year,landuse_year,wateruse_year;
    Bool rc;
#ifdef STORECLIMATE
    Climatedata store,data_save;
    if(config->nspinup)
    {
        /* climate for the first nspinyear years is stored in memory
           to avoid reading repeatedly from disk */
        rc=storeclimate(&store,input.climate, grid,input.climate->firstyear,config->nspinyear,config);
        failonerror(config,rc,STORE_CLIMATE_ERR,"Storage of climate failed");

        data_save=input.climate->data;
    }
#endif
    if(config->initsoiltemp)
    {
        rc=initsoiltemp(input.climate,grid,config);
        failonerror(config,rc,INITSOILTEMP_ERR,"Initialization of soil temperature failed");
    }

    /* main loop over spinup + simulation years  */
    for(year=config->firstyear-config->nspinup;year<=config->lastyear;year++)
    {
#ifdef IMAGE
        if(year>=config->start_imagecoupling)
            co2=receive_image_co2(config);
        else
#endif
            co2=getco2(input.climate,year); /* get atmospheric CO2 concentration */
        if(year<input.climate->firstyear) /* are we in spinup phase? */
            /* yes, let climate data point to stored data */
#ifdef STORECLIMATE
            moveclimate(input.climate,store,
                    (year-config->firstyear+config->nspinup) % config->nspinyear);
#else
        getclimate(input.climate,grid,input.climate->firstyear+(year-config->firstyear+config->nspinup) % config->nspinyear,config);
#endif
        else
        {
#ifdef STORECLIMATE
            if(year==input.climate->firstyear && config->nspinup)
            {
                /* restore climate data pointers to initial data */
                input.climate->data=data_save;
                freeclimatedata(&store); /* free data not used anymore */
            }
#endif
            /* read climate from files */
#ifdef IMAGE
            if(year>=config->start_imagecoupling)
            {
                if(receive_image_climate(input.climate,year,config))
                {
                    fprintf(stderr,"ERROR104: Simulation stopped in receive_image_climate().\n");
                    fflush(stderr);
                    break; /* leave time loop */
                }
            }
            else
#endif
                if(getclimate(input.climate,grid,year,config))
                {
                    fprintf(stderr,"ERROR104: Simulation stopped in getclimate().\n");
                    fflush(stderr);
                    break; /* leave time loop */
                }
        }

        // barrier.n: updating the waste water grid variable
        if (config->with_wastewater){
            nit_update_waste_file(config, grid, year);
        }

        // barrier.n: updating the fertilizer grid variable
        if (config->with_ferti){
            nit_update_fert_file(config, grid, year);
        }

        // barrier.n: updating the manure grid variable
        if(config->with_manure){
            nit_update_manure_file(config, grid, year);
        }

        if(input.landuse!=NULL)
        {
            calc_seasonality(grid,npft,ncft,config);
            if(config->withlanduse==CONST_LANDUSE || config->withlanduse==ALL_CROPS) /* constant landuse? */
                landuse_year=param.landuse_year_const;
            else
                landuse_year=year;
            /* under constand landuse also keep wateruse at landuse_year_const */
            if(config->withlanduse==CONST_LANDUSE)
                wateruse_year=param.landuse_year_const;
            else
                wateruse_year=year;
#ifdef IMAGE
            if(year>=config->start_imagecoupling)
            {
                if(receive_image_data(grid,npft,ncft,config))
                {
                    fprintf(stderr,"ERROR104: Simulation stopped in receive_image_data().\n");
                    fflush(stderr);
                    break; /* leave time loop */
                }
            }
            else
#endif
                /* read landuse pattern from file */
                if(getlanduse(input.landuse,grid,landuse_year,ncft,config))
                {
                    fprintf(stderr,"ERROR104: Simulation stopped in getlanduse().\n");
                    fflush(stderr);
                    break; /* leave time loop */
                }
            if(config->reservoir)
                allocate_reservoir(grid,year,config);
        }
        //printf("vegetables=%g\n",grid[0].ml.landfrac[0].ag_tree[VEGETABLES-npft+config->nagtree]);
        if(input.wateruse!=NULL && input.landuse!=NULL)
        {
            /* read wateruse data from file */
            if(getwateruse(input.wateruse,grid,wateruse_year,config))
            {
                fprintf(stderr,"ERROR104: Simulation stopped in getwateruse().\n");
                fflush(stderr);
                break; /* leave time loop */
            }
        }
        if(config->ispopulation)
        {
            if(readpopdens(input.popdens,year,grid,config))
            {
                fprintf(stderr,"ERROR104: Simulation stopped in getpopdens().\n");
                fflush(stderr);
                break; /* leave time loop */
            }
        }

        /* perform iteration for one year */
        if(year>=config->firstyear)
            {openoutput_yearly(output,year,config);}
        if(output->method==LPJ_SOCKET || config->river_routing)
            {iterateyear_river(output,grid,input,co2,npft,ncft,year,config,year== config->firstyear);}
        else
            /* iteration without river routing */
            {iterateyear(output,grid,input,co2,npft,ncft,year,config);}
        if(year>=config->firstyear)
            closeoutput_yearly(output,config);
        /* calculating total carbon and water fluxes collected from all tasks */
        cflux_total=flux_sum(&flux,grid,config);
        if(config->rank==0)
        {
            /* output of total carbon flux and water on stdout on root task */
            printflux(flux,cflux_total,year,config);
            if(output->method==LPJ_SOCKET && output->socket!=NULL &&
                    year>=config->firstyear)
                output_flux(output,flux);
            fflush(stdout); /* force output to console */
#ifdef SAFE
            check_balance(flux,year,config);
#endif
        }
#ifdef IMAGE
        if(year>=config->start_imagecoupling)
        {
            /* send data to IMAGE */
#ifdef DEBUG_IMAGE
            if(config->rank==0)
            {
                printf("sending data to image? year %d startimagecoupling %d\n",
                        year,config->start_imagecoupling);
                fflush(stdout);
            }
#endif
            if(send_image_data(config,grid,input.climate,npft))
                fail(SEND_IMAGE_ERR,FALSE,
                        "Problem with writing maps for transfer to IMAGE");
        }
#endif
        if(iswriterestart(config) && year==config->restartyear)
            fwriterestart(grid,npft,ncft,year,config); /* write restart file */
    } /* of 'for(year=...)' */
#ifdef STORECLIMATE
    if(config->nspinup && (config->lastyear<input.climate->firstyear || year<input.climate->firstyear))
    {
        /* restore climate data pointers to initial data */
        input.climate->data=data_save;
        freeclimatedata(&store); /* free data not used anymore */
    }
#endif
    return year;
} /* of 'iterate' */
