/***************************************************************************/
/**                                                                       **/
/**                f  p  r  i  n  t  c  o  n  f  i  g  .  c               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function prints LPJ configuration                                 **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-11-06 11:17:06 +0100 (ven., 06 nov. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

static int printsim(FILE *file,int len,int *count,const char *s)
{
  len=fputstring(file,len,(*count) ? ", " : "Simulation with ",78);
  (*count)++;
  len=fputstring(file,len,s,78);
  return len;
} /* of 'printsim' */

static void printoutname(FILE *file,const char *filename,Bool isoneyear,
                         const Config *config)
{
  char *fmt;
  char *pos;
  if(isoneyear)
  {
    fmt=malloc(strlen(filename)+6);
    if(fmt!=NULL)
    {
      pos=strstr(filename,"%d");
      strncpy(fmt,filename,pos-filename);
      fmt[pos-filename]='\0';
      strcat(fmt,"[%d-%d]");
      strcat(fmt,pos+2);
      fprintf(file,fmt,config->firstyear,config->lastyear);
      free(fmt);
    }
  }
  else
    fputs(filename,file);
} /* of printoutname' */

void fprintconfig(FILE *file,           /* File pointer to text output file */
                  const Config *config, /* LPJ configuration */
                  int npft,             /* Number of natural PFTs */
                  int ncft              /* Number of crop PFTs */
                 )
{
  char *irrig[]={"no","limited","potential","all","irrigation on rainfed"};
  char *fmt[]={"raw","clm","clm2","txt","fms","cdf"};
  char *data[]={"byte","short","int","float","double"};
  String s;
  int len;
  char *method[]={"write","MPI-2","gathered","socket"};
  int i,size,count=0;
  Bool isnetcdf;
  fputs("==============================================================================\n",file);
  fprintf(file,"Simulation \"%s\"",config->sim_name);
  if(config->ntask>1)
    fprintf(file," running on %d tasks\n",config->ntask);
  else
    putc('\n',file);
  len=0;
#ifdef IMAGE
  if(config->sim_id==LPJML_IMAGE)
    len=printsim(file,len,&count,"IMAGE coupling");
#endif
  if(config->wet_filename.name!=NULL)
    len=printsim(file,len,&count,"random precipitation");
  if(config->fire)
  {
    len=printsim(file,len,&count,(config->fire==SPITFIRE) ? "spitfire" : "fire");
    if(config->fire==SPITFIRE && config->ispopulation)
      len=fputstring(file,len," and population",78);
  }
  if(config->river_routing)
    len=printsim(file,len,&count,"river routing");
  if(config->permafrost)
    len=printsim(file,len,&count,"permafrost");

  if(config->withlanduse)
  {
    switch(config->withlanduse)
    {
      case CONST_LANDUSE:
        snprintf(s,STRING_LEN," const landuse set to year %d, ",param.landuse_year_const);
        len=printsim(file,len,&count,s);
        break;
      case ALL_CROPS:
        len=printsim(file,len,&count,"all crops land use, ");
        break;
      default:
        len=printsim(file,len,&count,"land use, ");
    } /* of switch */
    len=fputstring(file,len,irrig[config->irrig_scenario],78);
    len=fputstring(file,len," irrigation",78);
    if(config->laimax_interpolate==LAIMAX_INTERPOLATE)
    {
      len+=fprintf(file,", ");
      len=fputstring(file,len,"interpolated LAImax",78);
    }
    else if(config->laimax_interpolate==CONST_LAI_MAX)
    {
      len+=fprintf(file,", ");
      snprintf(s,STRING_LEN,"const LAImax=%.1f",param.laimax);
      len=fputstring(file,len,s,78);
    }
    if(config->sdate_option==FIXED_SDATE)
    {
      len=fputstring(file,len,", ",78);
      count++;
      snprintf(s,STRING_LEN,"fixed sowing date after %d",param.sdate_fixyear);
      len=fputstring(file,len,s,78);
    }
    else if(config->sdate_option==PRESCRIBED_SDATE)
    {
      len=fputstring(file,len,", ",78);
      count++;
      len=fputstring(file,len,"prescribed sowing date",78);
    }
  }
#ifdef NEW_GRASS
  if(config->grassfix_filename.name!=NULL)
    len=printsim(file,len,&count,"grassland fixed PFT");
  if(config->grassharvest_filename.name!=NULL)
    len=printsim(file,len,&count,"grassland harvest");
#endif
  if(config->firewood)
    len=printsim(file,len,&count,"wood fires");
  if(config->reservoir)
    len=printsim(file,len,&count,"dam reservoirs");
  if(config->wateruse_filename.name!=NULL)
    len=printsim(file,len,&count,"water use");
  if(count)
    fputs(".\n",file);
  fprintf(file,"Working directory: %s\n",getdir());
  if(isreadrestart(config))
    fprintf(file,"Starting from restart file '%s'.\n",config->restart_filename);
  fprintf(file,"Input files:\n"
               "Variable   Fmt  Varname  Filename\n"
               "---------- ---- -------- -----------------------------------------------------\n"
               "soil       %-4s %-8s %s\n",
          fmt[config->soil_filename.fmt],(config->soil_filename.var==NULL) ? "" : config->soil_filename.var,config->soil_filename.name);
  if(config->soil_filename.fmt!=CDF)

    fprintf(file,"coord      %-4s          %s\n",
            fmt[config->coord_filename.fmt],config->coord_filename.name);
  fprintf(file,"drainclass %-4s %-8s %s\n"
               "temp       %-4s %-8s %s\n"
               "prec       %-4s %-8s %s\n",
           fmt[config->drainclass_filename.fmt],
           (config->drainclass_filename.var==NULL) ? "": config->drainclass_filename.var,
           config->drainclass_filename.name,
           fmt[config->temp_filename.fmt],
           (config->temp_filename.var==NULL) ? "": config->temp_filename.var,
           (config->temp_filename.name==NULL) ? "": config->temp_filename.name,
           fmt[config->prec_filename.fmt],
           (config->prec_filename.var==NULL) ? "": config->prec_filename.var,
           (config->prec_filename.name==NULL) ? "" : config->prec_filename.name);
#ifdef IMAGE
  if(config->sim_id==LPJML_IMAGE)
    fprintf(file,"temp var   %-4s %-8s %s\n"
                 "prec var   %-4s %-8s %s\n"
                 "prod pool  %-4s %-8s %s\n",
            fmt[config->temp_var_filename.fmt],
           (config->temp_var_filename.var==NULL) ? "": config->temp_var_filename.var,
            config->temp_var_filename.name,
            fmt[config->prec_var_filename.fmt],
           (config->prec_var_filename.var==NULL) ? "": config->prec_var_filename.var,
            config->prec_var_filename.name,
            fmt[config->prodpool_init_filename.fmt],
           (config->prodpool_init_filename.var==NULL) ? "": config->prodpool_init_filename.var,
            config->prodpool_init_filename.name);
#endif
  if(config->with_radiation)
  {
    if(config->with_radiation==RADIATION || config->with_radiation==RADIATION_LWDOWN)
      fprintf(file,"%s     %-4s %-8s %s\n",
              (config->with_radiation==RADIATION) ? "lwnet " : "lwdown",
              fmt[config->lwnet_filename.fmt],
              (config->lwnet_filename.var==NULL) ? "": config->lwnet_filename.var,
              (config->lwnet_filename.name==NULL) ? "" : config->lwnet_filename.name);
    fprintf(file,"swdown     %-4s %-8s %s\n",
            fmt[config->swdown_filename.fmt],
            (config->swdown_filename.var==NULL) ? "": config->swdown_filename.var,
            (config->swdown_filename.name==NULL) ? "" : config->swdown_filename.name);
  }
  else
    fprintf(file,"cloud      %-4s %-8s %s\n",
            fmt[config->cloud_filename.fmt],
            (config->cloud_filename.var==NULL) ? "": config->cloud_filename.var,
            (config->cloud_filename.name==NULL) ? "" : config->cloud_filename.name);
  fprintf(file,"co2        %-4s          %s\n",fmt[config->co2_filename.fmt],
          (config->co2_filename.name==NULL) ? "" : config->co2_filename.name);
  if(config->fire==SPITFIRE)
  {
    if(config->tamp_filename.fmt==CDF && config->tmax_filename.name!=NULL)
    {
      fprintf(file,"temp min   %-4s %-8s %s\n"
                   "temp max   %-4s %-8s %s\n",
              fmt[config->tamp_filename.fmt],
              (config->tamp_filename.var==NULL) ? "": config->tamp_filename.var,
              (config->tamp_filename.name==NULL) ? "" : config->tamp_filename.name,
              fmt[config->tmax_filename.fmt],
              (config->tmax_filename.var==NULL) ? "": config->tmax_filename.var,
              (config->tmax_filename.name==NULL) ? "": config->tmax_filename.name);
    }
    else
      fprintf(file,"temp ampl  %-4s %-8s %s\n",
              fmt[config->tamp_filename.fmt],
              (config->tamp_filename.var==NULL) ? "": config->tamp_filename.var,
              (config->tamp_filename.name==NULL) ? "" : config->tamp_filename.name);
    fprintf(file,"windspeed  %-4s %-8s %s\n"
                 "lightning  %-4s %-8s %s\n"
                 "human ign  %-4s %-8s %s\n",
            fmt[config->wind_filename.fmt],
            (config->wind_filename.var==NULL) ? "": config->wind_filename.var,
            (config->wind_filename.name==NULL) ? "" : config->wind_filename.name,
            fmt[config->lightning_filename.fmt],
            (config->lightning_filename.var==NULL) ? "": config->lightning_filename.var,
            (config->lightning_filename.name==NULL) ? "" : config->lightning_filename.name,
            fmt[config->human_ignition_filename.fmt],
            (config->human_ignition_filename.var==NULL) ? "": config->human_ignition_filename.var,
            config->human_ignition_filename.name);
  }
  if(config->ispopulation)
    fprintf(file,"pop. dens  %-4s %-8s %s\n",
            fmt[config->popdens_filename.fmt],
            (config->popdens_filename.var==NULL) ? "": config->popdens_filename.var,
            config->popdens_filename.name);
#ifdef NEW_GRASS
  if(config->grassfix_filename.name!=NULL)
    fprintf(file,"Grassfix   %-4s %-8s %s\n",
            fmt[config->grassfix_filename.fmt],
            (config->grassfix_filename.var==NULL) ? "": config->grassfix_filename.var,
            config->grassfix_filename.name);
  if(config->grassharvest_filename.name!=NULL)
    fprintf(file,"Grassharv  %-4s %-8s %s\n",
            fmt[config->grassharvest_filename.fmt],
            (config->grassharvest_filename.var==NULL) ? "": config->grassharvest_filename.var,
            config->grassharvest_filename.name);
#endif
  if(config->withlanduse!=NO_LANDUSE)
  {
    fprintf(file,"countries  %-4s %-8s %s\n",
            fmt[config->countrycode_filename.fmt],
            (config->countrycode_filename.var==NULL) ? "": config->countrycode_filename.var,
            config->countrycode_filename.name);
    if(config->countrycode_filename.fmt==CDF)
      fprintf(file,"regions    %-4s %-8s %s\n",
              fmt[config->regioncode_filename.fmt],
              (config->regioncode_filename.var==NULL) ? "": config->regioncode_filename.var,
              config->regioncode_filename.name);
    if(config->withlanduse!=ALL_CROPS)
      fprintf(file,"landuse    %-4s %-8s %s\n",
              fmt[config->landuse_filename.fmt],
              (config->landuse_filename.var==NULL) ? "": config->landuse_filename.var,
              config->landuse_filename.name);
    if(config->sdate_option==PRESCRIBED_SDATE)
      fprintf(file,"sdates     %-4s %-8s %s\n",
              fmt[config->sdate_filename.fmt],
              (config->sdate_filename.var==NULL) ? "": config->sdate_filename.var,
              config->sdate_filename.name);
  }
  if(config->reservoir)
    fprintf(file,"elevation  %-4s %-8s %s\n"
                 "reservoir  %-4s %-8s %s\n",
            fmt[config->elevation_filename.fmt],
            (config->elevation_filename.var==NULL) ? "": config->elevation_filename.var,
            config->elevation_filename.name,
            fmt[config->reservoir_filename.fmt],
            (config->reservoir_filename.var==NULL) ? "": config->reservoir_filename.var,
            config->reservoir_filename.name);
  if(config->wet_filename.name!=NULL)
    fprintf(file,"wetdays    %-4s %-8s %s\n",
            fmt[config->wet_filename.fmt],
            (config->wet_filename.var==NULL) ? "": config->wet_filename.var,
            (config->wet_filename.name==NULL) ? "" : config->wet_filename.name);
  if(config->river_routing)
  {
    fprintf(file,"drainage   %-4s %-8s %s\n",
            fmt[config->drainage_filename.fmt],
            (config->drainage_filename.var==NULL) ? "": config->drainage_filename.var,
            config->drainage_filename.name);
    if(config->drainage_filename.fmt==CDF)
    fprintf(file,"river      %-4s %-8s %s\n",
            fmt[config->river_filename.fmt],
            (config->river_filename.var==NULL) ? "": config->drainage_filename.var,
            config->river_filename.name);
    fprintf(file,"lakes      %-4s %-8s %s\n",
            fmt[config->lakes_filename.fmt],
            (config->lakes_filename.var==NULL) ? "": config->lakes_filename.var,
            config->lakes_filename.name);
    if(config->withlanduse!=NO_LANDUSE)
      fprintf(file,"neighbour  %-4s %-8s %s\n",
              fmt[config->neighb_irrig_filename.fmt],
              (config->neighb_irrig_filename.var==NULL) ? "": config->neighb_irrig_filename.var,
              config->neighb_irrig_filename.name);
  }
  if(config->wateruse_filename.name!=NULL)
    fprintf(file,"wateruse   %-4s %-8s %s\n",
            fmt[config->wateruse_filename.fmt],
            (config->wateruse_filename.var==NULL) ? "": config->wateruse_filename.var,
            config->wateruse_filename.name);
  fputs("---------- ---- -------- -----------------------------------------------------\n",file);
  if(config->param_out)
    fprintparam(file,npft,ncft,config);
  if(iswriterestart(config))
    fprintf(file,"Writing restart file '%s' after year %d.\n",
            config->write_restart_filename,config->restartyear);
#ifdef IMAGE
  if(config->sim_id==LPJML_IMAGE)
    fprintf(file,
            "Coupled to IMAGE model running on host %s using port %d and %d.\n"
            "Time to wait for connection: %6d sec\n",
            config->image_host,config->image_inport,
            config->image_outport,config->wait_image);

#endif
  if(config->n_out)
  {
    size=strlen("Variable");
    for(i=0;i<config->n_out;i++)
      if(config->outnames[config->outputvars[i].id].name!=NULL &&
         size<strlen(config->outnames[config->outputvars[i].id].name))
        size=strlen(config->outnames[config->outputvars[i].id].name);
    fprintf(file,"Number of output files:       %d\n"
                 "Byte order in output files:   %s\n"
                 "Output method:                %s",
            config->n_out,
            bigendian() ? "big endian" : "little endian",
            method[config->outputmethod]);
    if(config->outputmethod==LPJ_SOCKET)
      fprintf(file," to %s using port %d",config->hostname,config->port);
    fputc('\n',file);
    isnetcdf=FALSE;
    for(i=0;i<config->n_out;i++)
      if(config->outputvars[i].filename.fmt==CDF)
      {
        isnetcdf=TRUE;
        break;
      }
    if(isnetcdf)
    {
      if(config->compress)
        fprintf(file,"Compression level for NetCDF: %d\n",config->compress);
      fprintf(file,"Missing value in NetCDF:      %g\n",config->missing_value);
    }
    fprintf(file,"%*s Fmt Unit         Type   Filename\n",-size,"Variable");
    frepeatch(file,'-',size);
    fputs(" --- ------------ ------ ",file);
    frepeatch(file,'-',77-size-4-13-7);
    putc('\n',file);
    for(i=0;i<config->n_out;i++)
    {
      if(config->outnames[config->outputvars[i].id].name==NULL)
        fprintf(file,"%*d",size,config->outputvars[i].id);
      else
        fprintf(file,"%*s",size,config->outnames[config->outputvars[i].id].name);
      fprintf(file," %s %12s %6s ",fmt[config->outputvars[i].filename.fmt],
              strlen(config->outnames[config->outputvars[i].id].unit)==0 ? "-" : config->outnames[config->outputvars[i].id].unit,
              data[getoutputtype(config->outputvars[i].id)]);
      printoutname(file,config->outputvars[i].filename.name,config->outputvars[i].oneyear,config);
      putc('\n',file);
    }
    frepeatch(file,'-',size);
    fputs(" --- ------------ ------ ",file);
    frepeatch(file,'-',77-size-4-13-7);
    putc('\n',file);
    if(config->crop_index>=0)
    {
      fprintf(file,"CFT for daily output:        %s\n"
                   "Irrigation for daily output: %s\n",
              config->pftpar[config->crop_index].name,
              (config->crop_irrigation) ? "irrigated" : "rain fed");
    }
    if(config->pft_output_scaled)
      fputs("PFT-specific output is grid scaled.\n",file);
  }
  else
    fputs("No output files written.\n",file);
  if(config->nspinup)
    fprintf(file,"Spinup years:                %6d\n"
            "Cycle length during spinup:  %6d\n",
             config->nspinup,config->nspinyear);
  else
    fputs("No spinup years.\n",file);
  fprintf(file,"First year:                  %6d\n"
          "Last year:                   %6d\n",
          config->firstyear,config->lastyear);
#ifdef IMAGE
  if(config->sim_id==LPJML_IMAGE)
    fprintf(file,"Start IMAGE coupling:        %6d\n",
            config->start_imagecoupling);
#endif
  if(config->firstgrid)
    fprintf(file,"Index of first cell:         %6d\n",config->firstgrid);
  fprintf(file,"Number of grid cells:        %6d\n",config->nall);
  fputs("==============================================================================\n",file);
  fflush(file);
} /* of 'fprintconfig' */
