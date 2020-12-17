/***************************************************************************/
/**                                                                       **/
/**               i  n  i  t  c  l  i  m  a  t  e  .  c                   **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function initclimate allocates and initializes the climate data   **/
/**     pointer. The corresponding data files are opened and storage      **/
/**     allocated.                                                        **/
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

Climate *initclimate(const Cell grid[],   /* LPJ grid */
                     const Config *config /* pointer to LPJ configuration */
                    )               /* returns allocated climate data struct
                                       or NULL on error */
{
  int i;
  int *vec;
  Climate *climate;
  climate=new(Climate);
  if(climate==NULL)
    return NULL;
  if(openclimate(&climate->file_temp,&config->temp_filename,"celsius",sizeof(short),config))
  {
    free(climate);
    return NULL;
  }
  if(config->temp_filename.fmt!=CDF && climate->file_temp.version==1)
    climate->file_temp.scalar=0.1;
  climate->firstyear=climate->file_temp.firstyear;
  if(openclimate(&climate->file_prec,&config->prec_filename,"kg/m2/day" /* "mm" */,sizeof(short),config))
  {
    closeclimatefile(&climate->file_temp,isroot(*config));
    free(climate);
    return NULL;
  }
  if(climate->firstyear<climate->file_prec.firstyear)
    climate->firstyear=climate->file_prec.firstyear;
  if(config->with_radiation)
  {
    if(config->with_radiation==RADIATION || config->with_radiation==RADIATION_LWDOWN)
    {
      if(openclimate(&climate->file_lwnet,&config->lwnet_filename,"W/m2",sizeof(short),config))
      {
        closeclimatefile(&climate->file_temp,isroot(*config));
        closeclimatefile(&climate->file_prec,isroot(*config));
        free(climate);
        return NULL;
      }
      if(config->lwnet_filename.fmt!=CDF && climate->file_lwnet.version==1)
        climate->file_lwnet.scalar=0.1;
      if(climate->firstyear<climate->file_lwnet.firstyear)
        climate->firstyear=climate->file_lwnet.firstyear;
    }
    if(openclimate(&climate->file_swdown,&config->swdown_filename,"W/m2",sizeof(short),config))
    {
      closeclimatefile(&climate->file_temp,isroot(*config));
      closeclimatefile(&climate->file_prec,isroot(*config));
      if(config->with_radiation==RADIATION || config->with_radiation==RADIATION_LWDOWN)
        closeclimatefile(&climate->file_lwnet,isroot(*config));
      free(climate);
      return NULL;
    }
    if(config->swdown_filename.fmt!=CDF && climate->file_swdown.version==1)
      climate->file_swdown.scalar=0.1;
    if(climate->firstyear<climate->file_swdown.firstyear)
      climate->firstyear=climate->file_swdown.firstyear;
    climate->data.sun=NULL;
  }
  else
  {
    if(openclimate(&climate->file_cloud,&config->cloud_filename,"%",sizeof(short),config))
    {
      closeclimatefile(&climate->file_temp,isroot(*config));
      closeclimatefile(&climate->file_prec,isroot(*config));
      free(climate);
      return NULL;
    }
    if(climate->firstyear<climate->file_cloud.firstyear)
      climate->firstyear=climate->file_cloud.firstyear;
    climate->data.lwnet=climate->data.swdown=NULL;
  }
  if(config->wet_filename.name!=NULL)
  {
    if(openclimate(&climate->file_wet,&config->wet_filename,NULL,sizeof(short),config))
    {
      closeclimatefile(&climate->file_temp,isroot(*config));
      closeclimatefile(&climate->file_prec,isroot(*config));
      if(config->with_radiation)
      {
        if(config->with_radiation==RADIATION || config->with_radiation==RADIATION_LWDOWN)
          closeclimatefile(&climate->file_lwnet,isroot(*config));
        closeclimatefile(&climate->file_swdown,isroot(*config));
      }
      else
        closeclimatefile(&climate->file_cloud,isroot(*config));
      free(climate);
      return NULL;
    }
    if(climate->firstyear<climate->file_wet.firstyear)
      climate->firstyear=climate->file_wet.firstyear;
    climate->file_wet.ready=FALSE;
  }
  else
    climate->data.wet=NULL;

  if(config->fire==SPITFIRE)
  {
    if(openclimate(&climate->file_wind,&config->wind_filename,"m/s",sizeof(short),config))
    {
      closeclimatefile(&climate->file_temp,isroot(*config));
      closeclimatefile(&climate->file_prec,isroot(*config));
      if(config->with_radiation)
      {
        if(config->with_radiation==RADIATION || config->with_radiation==RADIATION_LWDOWN)
          closeclimatefile(&climate->file_lwnet,isroot(*config));
        closeclimatefile(&climate->file_swdown,isroot(*config));
      }
      else
        closeclimatefile(&climate->file_cloud,isroot(*config));
      if(config->wet_filename.name!=NULL)
        closeclimatefile(&climate->file_wet,isroot(*config));
      free(climate);
      return NULL;
    }
    if(config->wind_filename.fmt!=CDF && climate->file_wind.version==1)
      climate->file_wind.scalar=0.001;
    if(openclimate(&climate->file_tamp,&config->tamp_filename,(config->tamp_filename.fmt==CDF) ? NULL : NULL,sizeof(short),config))
    {
      closeclimatefile(&climate->file_temp,isroot(*config));
      closeclimatefile(&climate->file_prec,isroot(*config));
      if(config->with_radiation)
      {
        if(config->with_radiation==RADIATION || config->with_radiation==RADIATION_LWDOWN)
          closeclimatefile(&climate->file_lwnet,isroot(*config));
        closeclimatefile(&climate->file_swdown,isroot(*config));
      }
      else
        closeclimatefile(&climate->file_cloud,isroot(*config));
      if(config->wet_filename.name!=NULL)
        closeclimatefile(&climate->file_wet,isroot(*config));
      free(climate);
      return NULL;
    }
    if(config->tamp_filename.fmt!=CDF && climate->file_tamp.version==1)
      climate->file_tamp.scalar=0.1;
    if(config->tamp_filename.fmt==CDF && config->tmax_filename.name!=NULL)
    {
      if(openclimate(&climate->file_tmax,&config->tmax_filename,"celsius",sizeof(short),config))
    {
      closeclimatefile(&climate->file_tamp,isroot(*config));
      closeclimatefile(&climate->file_temp,isroot(*config));
      closeclimatefile(&climate->file_prec,isroot(*config));
      if(config->with_radiation)
      {
        if(config->with_radiation==RADIATION || config->with_radiation==RADIATION_LWDOWN)
          closeclimatefile(&climate->file_lwnet,isroot(*config));
        closeclimatefile(&climate->file_swdown,isroot(*config));
      }
      else
        closeclimatefile(&climate->file_cloud,isroot(*config));
      if(config->wet_filename.name!=NULL)
        closeclimatefile(&climate->file_wet,isroot(*config));
      free(climate);
      return NULL;
    }
      if(config->tmax_filename.fmt!=CDF && climate->file_tmax.version==1)
        climate->file_tmax.scalar=0.1;
    }
    if(openclimate(&climate->file_lightning,&config->lightning_filename,NULL,sizeof(int),config))
    {
      closeclimatefile(&climate->file_temp,isroot(*config));
      closeclimatefile(&climate->file_prec,isroot(*config));
      if(config->with_radiation)
      {
        if(config->with_radiation==RADIATION || config->with_radiation==RADIATION_LWDOWN)
          closeclimatefile(&climate->file_lwnet,isroot(*config));
        closeclimatefile(&climate->file_swdown,isroot(*config));
      }
      else
        closeclimatefile(&climate->file_cloud,isroot(*config));
      if(config->wet_filename.name!=NULL)
        closeclimatefile(&climate->file_wet,isroot(*config));
      free(climate);
      return NULL;
    }
  }
  else
    climate->data.wind=climate->data.tamp=climate->data.lightning=NULL;

#ifdef IMAGE
  if(config->sim_id==LPJML_IMAGE)
  {
    if(openclimate(&climate->file_temp_var,&config->temp_var_filename,NULL,sizeof(short),config))
    {
      closeclimatefile(&climate->file_temp,isroot(*config));
      closeclimatefile(&climate->file_prec,isroot(*config));
      if(config->with_radiation)
      {
        if(config->with_radiation==RADIATION || config->with_radiation==RADIATION_LWDOWN)
          closeclimatefile(&climate->file_lwnet,isroot(*config));
        closeclimatefile(&climate->file_swdown,isroot(*config));
      }
      else
        closeclimatefile(&climate->file_cloud,isroot(*config));
      if(config->wet_filename.name!=NULL)
        closeclimatefile(&climate->file_wet,isroot(*config));
      free(climate);
      return NULL;
    }
    if(openclimate(&climate->file_prec_var,&config->prec_var_filename,NULL,sizeof(short),config))
    {
      closeclimatefile(&climate->file_temp,isroot(*config));
      closeclimatefile(&climate->file_prec,isroot(*config));
      if(config->with_radiation)
      {
        if(config->with_radiation==RADIATION || config->with_radiation==RADIATION_LWDOWN)
          closeclimatefile(&climate->file_lwnet,isroot(*config));
        closeclimatefile(&climate->file_swdown,isroot(*config));
      }
      else
        closeclimatefile(&climate->file_cloud,isroot(*config));
      if(config->wet_filename.name!=NULL)
        closeclimatefile(&climate->file_wet,isroot(*config));
      closeclimatefile(&climate->file_temp_var,isroot(*config));
      free(climate);
      return NULL;
    }
  }
  else
    climate->file_temp_var.file=climate->file_prec_var.file=NULL;
#endif
  if(climate->firstyear>config->firstyear)
  {
    if(isroot(*config))
      fprintf(stderr,"ERROR200: Climate data starts at %d, later than first simulation year.\n",
              climate->firstyear);
    closeclimatefile(&climate->file_temp,isroot(*config));
    closeclimatefile(&climate->file_prec,isroot(*config));
    if(config->with_radiation)
    {
      if(config->with_radiation==RADIATION || config->with_radiation==RADIATION_LWDOWN)
        closeclimatefile(&climate->file_lwnet,isroot(*config));
      closeclimatefile(&climate->file_swdown,isroot(*config));
    }
    else
      closeclimatefile(&climate->file_cloud,isroot(*config));
    if(config->wet_filename.name!=NULL)
      closeclimatefile(&climate->file_wet,isroot(*config));
#ifdef IMAGE
    if(config->sim_id==LPJML_IMAGE)
    {
      closeclimatefile(&climate->file_temp_var,isroot(*config));
      closeclimatefile(&climate->file_prec_var,isroot(*config));
    }
#endif
    free(climate);
    return NULL;
  }
  if(readco2(&climate->co2,&config->co2_filename,isroot(*config)))
  {
    free(climate);
    return NULL;
  }
  if(isroot(*config) && climate->co2.firstyear>climate->file_temp.firstyear)
      fprintf(stderr,"WARNING001: First year in '%s'=%d greater than climate->file_temp.firstyear=%d.\n"
              "            Preindustrial values are used.\n",
              config->co2_filename.name,climate->co2.firstyear,climate->file_temp.firstyear);
#ifdef DEBUG7
  printf("climate->file_temp.firstyear: %d  co2-year: %d  value: %f\n",
         climate->file_temp.firstyear, climate->co2.firstyear,climate->co2.data[0]);
#endif
  if(config->prec_filename.fmt==FMS)
    climate->data.prec=NULL;
  else
  {
    if((climate->data.prec=newvec(Real,climate->file_prec.n))==NULL)
    {
      printallocerr("prec");
      free(climate->co2.data);
      free(climate);
      return NULL;
    }
  }
  if(config->temp_filename.fmt==FMS)
    climate->data.temp=NULL;
  else
  {
    if((climate->data.temp=newvec(Real,climate->file_temp.n))==NULL)
    {
      printallocerr("temp");
      free(climate->co2.data);
      free(climate->data.prec);
      free(climate);
      return NULL;
    }
  }
  if(config->fire==SPITFIRE)
  {
    if(config->wind_filename.fmt==FMS)
      climate->data.wind=NULL;
    else
    {
      if((climate->data.wind=newvec(Real,climate->file_wind.n))==NULL)
      {
        if(isroot(*config))
          printallocerr("wind");
        free(climate->co2.data);
        free(climate->data.prec);
        free(climate->data.temp);
        free(climate);
        return NULL;
      }
    }
    if(config->tamp_filename.fmt==FMS)
      climate->data.tamp=NULL;
    else
    {
      if((climate->data.tamp=newvec(Real,climate->file_tamp.n))==NULL)
      {
        if(isroot(*config))
          printallocerr("tamp");
        free(climate->co2.data);
        free(climate->data.wind);
        free(climate->data.prec);
        free(climate->data.temp);
        free(climate);
        return NULL;
      }
    }
    if(config->tamp_filename.fmt==CDF && config->tmax_filename.name!=NULL)
    {
      if((climate->data.tmax=newvec(Real,climate->file_tmax.n))==NULL)
      {
        if(isroot(*config))
          printallocerr("tmax");
        free(climate->co2.data);
        free(climate->data.wind);
        free(climate->data.tamp);
        free(climate->data.prec);
        free(climate->data.temp);
        free(climate);
        return NULL;
      }
    }
    else
      climate->data.tmax=NULL;
    if((climate->data.lightning=newvec(Real,climate->file_lightning.n))==NULL)
    {
      if(isroot(*config))
        printallocerr("lightning");
      free(climate->co2.data);
      free(climate->data.wind);
      free(climate->data.tamp);
      free(climate->data.prec);
      free(climate->data.temp);
      free(climate);
      return NULL;
    }
    if(climate->file_lightning.fmt!=CDF && climate->file_lightning.version==1)
      climate->file_lightning.scalar=1e-7;
    if(climate->file_lightning.fmt==CDF)
    {
     if(readclimate_netcdf(&climate->file_lightning,climate->data.lightning,grid,0,config))
       return NULL;
    }
    else
    {
      if(fseek(climate->file_lightning.file,climate->file_lightning.offset,SEEK_SET))
      {
        fputs("ERROR191: Cannot seek lightning in initclimate().\n",stderr);
        closeclimatefile(&climate->file_lightning,isroot(*config));
        return NULL;
      }
      vec=newvec(int,climate->file_lightning.n);
      if(vec==NULL)
      {
        if(isroot(*config))
          printallocerr("vec");
        free(climate->co2.data);
        free(climate->data.wind);
        free(climate->data.tamp);
        free(climate->data.prec);
        free(climate->data.temp);
        free(climate->data.lightning);
        return NULL;
      }
      if(fread(vec,sizeof(int),climate->file_lightning.n,
               climate->file_lightning.file)!=climate->file_lightning.n)
      {
        fputs("ERROR192: Cannot read lightning in initclimate().\n",stderr);
        free(vec);
        closeclimatefile(&climate->file_lightning,isroot(*config));
        return NULL;
      }
      if(climate->file_lightning.swap)
        for(i=0;i<climate->file_lightning.n;i++)
          climate->data.lightning[i]=swapint(vec[i])*climate->file_lightning.scalar;
      else
        for(i=0;i<climate->file_lightning.n;i++)
          climate->data.lightning[i]=vec[i]*climate->file_lightning.scalar;

      closeclimatefile(&climate->file_lightning,isroot(*config));
      free(vec);
    }
  }
  else
    climate->data.wind=climate->data.tamp=climate->data.lightning=climate->data.tmax=NULL;
  if(config->with_radiation)
  {
    if(config->with_radiation==RADIATION || config->with_radiation==RADIATION_LWDOWN)
    {
      if(config->lwnet_filename.fmt==FMS)
        climate->data.lwnet=NULL;
      else
      {
        if((climate->data.lwnet=newvec(Real,climate->file_lwnet.n))==NULL)
        {
          printallocerr("lwnet");
          free(climate->co2.data);
          free(climate->data.prec);
          free(climate->data.temp);
          if(config->fire==SPITFIRE)
          {
            free(climate->data.wind);
            free(climate->data.tamp);
            free(climate->data.lightning);
          }
          free(climate);
          return NULL;
        }
      }
    }
    else
      climate->data.lwnet=NULL;
    if(config->swdown_filename.fmt==FMS)
      climate->data.swdown=NULL;
    else
    {
      if((climate->data.swdown=newvec(Real,climate->file_swdown.n))==NULL)
      {
        if(isroot(*config))
          printallocerr("swdown");
        free(climate->co2.data);
        free(climate->data.prec);
        free(climate->data.temp);
        free(climate->data.lwnet);
        if(config->fire==SPITFIRE)
        {
          free(climate->data.wind);
          free(climate->data.tamp);
          free(climate->data.lightning);
        }
        free(climate);
        return NULL;
      }
    }
  }
  else
  {
    if(config->cloud_filename.fmt==FMS)
      climate->data.sun=NULL;
    else
    {
      if((climate->data.sun=newvec(Real,climate->file_cloud.n))==NULL)
      {
        printallocerr("cloud");
        free(climate->co2.data);
        free(climate->data.prec);
        free(climate->data.temp);
        if(config->fire==SPITFIRE)
        {
          free(climate->data.wind);
          free(climate->data.tamp);
          free(climate->data.lightning);
        }
        free(climate);
        return NULL;
      }
    }
  }
  if(config->wet_filename.name!=NULL)
  {
    if(config->wet_filename.fmt==FMS)
      climate->data.wet=NULL;
    else
    {
      if((climate->data.wet=newvec(Real,climate->file_wet.n))==NULL)
      {
        printallocerr("wet");
        free(climate->co2.data);
        free(climate->data.prec);
        free(climate->data.temp);
        if(config->fire==SPITFIRE)
        {
          free(climate->data.wind);
          free(climate->data.tamp);
          free(climate->data.lightning);
        }
        if(config->with_radiation)
        {
          free(climate->data.lwnet);
          free(climate->data.swdown);
        }
        else
          free(climate->data.sun);
        free(climate);
        return NULL;
      }
    }
  }
  return climate;
} /* of 'initclimate' */
