/***************************************************************************/
/**                                                                       **/
/**                   i  t  e  r  a  t  e  y  e  a  r  .  c               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function performs calculation for one year for all grid cells     **/
/**     without river routing                                             **/
/**                                                                       **/
/**     Principal structure:                                              **/
/**                                                                       **/
/**           for(cell=0;cell<config.ngridcell;cell++)                    **/
/**           {                                                           **/
/**             init_annual();                                            **/
/**             foreachmonth(month)                                       **/
/**             {                                                         **/
/**               foreachdayofmonth(dayofmonth,month)                     **/
/**                 update_daily()                                        **/
/**               update_monthly();                                       **/
/**             }                                                         **/
/**             update_annual();                                          **/
/**           }                                                           **/
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

void iterateyear(Outputfile *output,   /* Output file array */
                 Cell grid[],          /* array of grid cells */
                 Input input,          /* input data */
                 Real co2,             /* CO2 concentration (ppmv) */
                 int npft,             /* number of natural PFTs */
                 int ncft,             /* number of crop PFTs */
                 int year,             /* Simulation year (AD)*/
                 const Config *config  /* LPJ configuration */
                )
{
  Dailyclimate daily;
  int month,dayofmonth,day;
  int cell;
  Bool intercrop,istimber;
  Outputmonth *outputmonth;
  Outputday *outputday;
  Real popdens=0; /* population density (capita/km2) */
#ifdef IMAGE
  istimber=(config->start_imagecoupling!=INT_MAX);
  if(year>=config->firstyear-istimber*10)
#else
  istimber=FALSE;
  if(year>=config->firstyear)
#endif
  {
    outputmonth=newoutputmonthly(output,config->count);
    if(output->withdaily)
      outputday=newoutputdaily(output,config->count);
  }
  intercrop=getintercrop(input.landuse);
  for(cell=0;cell<config->ngridcell;cell++)
    if(!grid[cell].skip)
    {
      if(config->ispopulation)
        popdens=getpopdens(input.popdens,cell);
      day=1;
      init_annual(grid+cell,npft,config->nbiomass,config->nagtree,ncft);
      if(input.landuse!=NULL)
      {
        if(!config->isconstlai)
          laimax_manage(&grid[cell].ml.manage,config->pftpar+npft,npft,ncft,year);
        if(year>=config->firstyear-config->nspinup)
          landusechange(grid+cell,config->pftpar,npft,ncft,config->nagtree,config->ntypes,
                        intercrop,istimber,year,config->pft_output_scaled);
      }
      initgdd(grid[cell].gdd,npft);

#ifdef IMAGE
      setoutput_image(grid+cell,ncft);
#endif

      foreachmonth(month)
      {

        /* Initialize random seed */
        if(israndomprec(input.climate))
          srand48(config->seed+(config->startgrid+cell)*year*month);
        initclimate_monthly(input.climate,&grid[cell].climbuf,cell,month);

#ifdef DEBUG
        /*  printf("year %d month %d temp = %.2f prec = %.2f wet = %.2f",year,month,
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
            printf("sun = %.2f\n",(getcellsun(input.climate,cell))[month]);
          fflush(stdout);*/
#endif
        initoutput_monthly(&(grid[cell].output));
#ifdef IMAGE
        monthlyoutput_image(&grid[cell].output,input.climate,cell,month);
#endif
        foreachdayofmonth(dayofmonth,month)
        {
          grid[cell].output.dcflux=0;
          initoutput_daily(&(grid[cell].output.daily));
          dailyclimate(&daily,input.climate,&grid[cell].climbuf,cell,day,
                       month,dayofmonth);
          /* get daily values for temperature, precipitation and sunshine */
          grid[cell].output.daily.temp=daily.temp;
          grid[cell].output.daily.prec=daily.prec;
          grid[cell].output.daily.sun=daily.sun;

#ifdef DEBUG
     /*     printf("day=%d cell=%d\n",day,cell);
      fprintcell(stdout,grid+cell,1,ncft,input.landuse!=NULL,FALSE);
          fflush(stdout);*/
#endif
          update_daily(grid+cell,co2,popdens,daily,day,npft,
                       ncft,year,month,output->withdaily,intercrop,config,cell);
#ifdef IMAGE
          if(year>=config->firstyear-istimber*10 && output->withdaily)
#else
          if(year>=config->firstyear && output->withdaily)
#endif
            update_outputdaily(outputday,(Real *)(&grid[cell].output.daily),
                               config->count,day-1);
          day++;
        } /* of 'foreachdayofmonth */
        update_monthly(grid+cell,getmtemp(input.climate,&grid[cell].climbuf,
                       cell,month),getmprec(input.climate,&grid[cell].climbuf,
                       cell,month),month);
#ifdef DEBUG
printf("year = %i, config->firstyear = %i \n", year,config->firstyear );
#endif
#ifdef IMAGE
        if(year>=config->firstyear-istimber*10)
#else
        if(year>=config->firstyear)
#endif
          update_outputmonthly(outputmonth,&grid[cell].output,config->count,
                               month);
      }  /* of 'foreachmonth' */

      update_annual(grid+cell,npft,ncft,popdens,year,intercrop,config);
#ifdef DEBUG
      //fprintcell(stdout,grid+cell,1,ncft,input.landuse!=NULL,FALSE);
#endif

      grid[cell].balance.awater_flux+=grid[cell].discharge.dmass_lake/grid[cell].coord.area;
      grid[cell].discharge.dmass_lake=0.0; /* get rid of excess irrigation water */

#ifdef SAFE
      check_fluxes(grid+cell,year,cell,config);
#endif
      if(config->nspinup>veg_equil_year &&
         year==config->firstyear-config->nspinup+veg_equil_year && !config->from_restart)
        equilveg(grid+cell);
      if(config->nspinup>soil_equil_year &&
         year==config->firstyear-config->nspinup+soil_equil_year && !config->from_restart)
        equilsom(grid+cell,npft+ncft,config->pftpar);
#ifdef IMAGE
      if(year>=config->firstyear-istimber*10)
#else
      if(year>=config->firstyear)
#endif
      {
        outputmonth->count++;
        if(output->withdaily)
          outputday->count++;
      }
    } /* of '!grid-skipped' */
#ifdef IMAGE
  if(year>=config->firstyear-istimber*10)
#else
  if(year>=config->firstyear)
#endif
  {
    /* write out monthly and annual output */
    fwriteoutput_monthly2(output,outputmonth,year,config);
    freeoutputmonthly(outputmonth);
    if(output->withdaily)
    {
      fwriteoutput_daily2(output,outputday,year,config);
      freeoutputdaily(outputday);
    }
    fwriteoutput_annual(output,grid,year,config);
    fwriteoutput_pft(output,grid,npft,ncft,year,config);
  }
} /* of 'iterateyear' */
