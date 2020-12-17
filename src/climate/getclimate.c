/***************************************************************************/
/**                                                                       **/
/**                      g  e  t  c  l  i  m  a  t  e  .  c               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function getclimate reads monthly climate data (temperature,      **/
/**     precipitation, cloudiness and wet days) for a specified year.     **/
/**     The pointer to the climate data has to be initialized by the      **/
/**     function initclimate.                                             **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  19.02.2013                                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool readclimate(Climatefile *file,   /* climate data file */
                 Real data[],         /* climate data read */
                 Real offset,         /* offset for data */
                 Real scale,          /* scale factor for data*/
                 const Cell grid[],   /* LPJ grid */
                 int year,            /* year (AD) */
                 const Config *config /* LPJ configuration */
                )                     /* return TRUE on error */
{
  short *vec;
  int i,rc;
  long long index;
  if(file->fmt==FMS)
    return FALSE;
  index=year-file->firstyear;
  if(index<0 || index>=file->nyear)
  {
    if(isroot(*config))
      fprintf(stderr,"ERROR130: Invalid year %d in readclimate().\n",year);
    return TRUE;
  }
  if(file->fmt==CDF)
    rc=readclimate_netcdf(file,data,grid,index,config);
  else
  {
    if(fseek(file->file,index*file->size+file->offset,SEEK_SET))
      rc=TRUE;
    else
    {
      vec=newvec(short,file->n);
      if(vec==NULL)
      {
        printallocerr("vec");
        rc=TRUE;
      }
      else
      {
        if(fread(vec,sizeof(short),file->n,file->file)!=file->n)
          rc=TRUE;
        else
        {
          if(file->swap)
            for(i=0;i<file->n;i++)
              data[i]=offset+swapshort(vec[i])*scale;
          else
            for(i=0;i<file->n;i++)
              data[i]=offset+vec[i]*scale;
          rc=FALSE;
        }
        free(vec);
      }
    }
  }
  return iserror(rc,config);
} /* of'readclimate' */

Bool getclimate(Climate *climate,    /* pointer to climate data */
                const Cell grid[],   /* LPJ grid */
                int year,            /* year of climate data to be read */
                const Config *config /* LPJ configuration */
               )                     /* returns TRUE on error */
{
  Real *wet;
  int i,index;
  Bool rc;
  if(readclimate(&climate->file_temp,climate->data.temp,0,climate->file_temp.scalar,grid,year,config))
  {
    if(isroot(*config))
      fprintf(stderr,"ERROR131: Cannot read temperature of year %d in getclimate().\n",
              year);
    return TRUE;
  }
  if(readclimate(&climate->file_prec,climate->data.prec,0,climate->file_prec.scalar,grid,year,config))
  {
    if(isroot(*config))
      fprintf(stderr,"ERROR131: Cannot read precipitation of year %d in getclimate().\n",
              year);
    return TRUE;
  }
  if(climate->data.tmax!=NULL)
  {
    if(readclimate(&climate->file_tmax,climate->data.tmax,0,climate->file_tmax.scalar,grid,year,config))
    {
      if(isroot(*config))
        fprintf(stderr,"ERROR131: Cannot read tmax of year %d in getclimate().\n",
                year);
      return TRUE;
    }
  }
  if(climate->data.sun!=NULL)
  {
    if(readclimate(&climate->file_cloud,climate->data.sun,100,-climate->file_cloud.scalar,grid,year,config))
    {
      if(isroot(*config))
        fprintf(stderr,"ERROR131: Cannot read cloudiness of year %d in getclimate().\n",
                year);
      return TRUE;
    }
    if(config->cloud_filename.fmt==CDF)
      for(i=0;i<climate->file_cloud.n;i++)
        climate->data.sun[i]=100-climate->data.sun[i];
  }
  if(climate->data.lwnet!=NULL)
  {
    if(readclimate(&climate->file_lwnet,climate->data.lwnet,0,climate->file_lwnet.scalar,grid,year,config))
    {
      if(isroot(*config))
        fprintf(stderr,"ERROR131: Cannot read lwnet of year %d in getclimate().\n",
                year);
      return TRUE;
    }
  }
  if(climate->data.swdown!=NULL)
  {
    if(readclimate(&climate->file_swdown,climate->data.swdown,0,climate->file_swdown.scalar,grid,year,config))
    {
      if(isroot(*config))
        fprintf(stderr,"ERROR131: Cannot read swdown of year %d in getclimate().\n",
                year);
      return TRUE;
    }
  }
  if(climate->data.wind!=NULL)
  {
    if(readclimate(&climate->file_wind,climate->data.wind,0,climate->file_wind.scalar,grid,year,config))
    {
      if(isroot(*config))
        fprintf(stderr,"ERROR131: Cannot read wind of year %d in getclimate().\n",
                year);
      return TRUE;
    }
  }
  if(climate->data.tamp!=NULL)
  {
    if(readclimate(&climate->file_tamp,climate->data.tamp,0,climate->file_tamp.scalar,grid,year,config))
    {
      if(isroot(*config))
        fprintf(stderr,"ERROR131: Cannot read %s of year %d in getclimate().\n",
                (config->tamp_filename.fmt==CDF) ? "tmin" : "tamp",year);
      return TRUE;
    }
  }
  if(climate->data.wet!=NULL)
  {
    index=year-climate->file_wet.firstyear;
    if(index<0)
    {
      if(isroot(*config))
        fprintf(stderr,"ERROR130: Invalid year %d in getclimate().\n",year);
      return TRUE;
    }
    if(index<climate->file_wet.nyear)
    {
      if(readclimate(&climate->file_wet,climate->data.wet,0,climate->file_wet.scalar,grid,year,config))
      {
        if(isroot(*config))
          fprintf(stderr,"ERROR131: Cannot read wet days of year %d in getclimate().\n",
                  year);
        return TRUE;
      }
      climate->file_wet.ready=FALSE;
    }
    else if(!climate->file_wet.ready)
    {
      climate->file_wet.ready=TRUE;
      wet=newvec(Real,climate->file_wet.n);
      if(wet==NULL)
      {
        printallocerr("wet");
        rc=TRUE;
      }
      else
        rc=FALSE;
      if(iserror(rc,config))
        return TRUE;
      for(i=0;i<climate->file_wet.n;i++)
        climate->data.wet[i]=0;
      /* Average number of wet days from 1960 to 1989 */
      for(index=1960;index<1990;index++)
      {
        readclimate(&climate->file_wet,wet,0,climate->file_wet.scalar,grid,index,config);
        for(i=0;i<climate->file_wet.n;i++)
          climate->data.wet[i]+=wet[i];
      }
      for(i=0;i<climate->file_wet.n;i++)
        climate->data.wet[i]/=30;
      free(wet);
    }
  }
  return FALSE;
} /* of 'getclimate' */
