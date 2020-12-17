/***************************************************************************/
/**                                                                       **/
/**                       c  e  l  l  d  a  t  a  .  c                    **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Functions opens and reads soil data, coordinates and drainclass   **/
/**     for each cell. NetCDF input is supported if compiled with         **/
/**     -DUSE_NETCDF flag.                                                **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 03.03.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

struct celldata
{
  int soil_fmt;
  int drainclass_fmt;
  union
  {
    struct
    {
      Bool swap_soil;
      int version_soil;
      FILE *file_soil;
      Coordfile file_coord;
    } bin;
    Coord_netcdf cdf;
  } soil;
  union
  {
    FILE *file;
    Input_netcdf cdf;
  } drainclass;
};

Celldata opencelldata(Config *config)
{
  Celldata celldata;
  Header header;
  String headername;
  int version;
  Bool swap;
  celldata=new(struct celldata);
  if(celldata==NULL)
    return NULL;
  celldata->soil_fmt=config->soil_filename.fmt;
  if(config->soil_filename.fmt==CDF)
  {
    celldata->soil.cdf=opencoord_netcdf(config->soil_filename.name,
                                        config->soil_filename.var,
                                        isroot(*config));
    if(celldata->soil.cdf==NULL)
    {
      free(celldata);
      return NULL;
    }
    getresolution_netcdf(celldata->soil.cdf,&config->resolution);
  }
  else
  {
    celldata->soil.bin.file_coord=opencoord(&config->coord_filename);
    if(celldata->soil.bin.file_coord==NULL)
    {
      printfopenerr(config->coord_filename.name);
      free(celldata);
      return NULL;
    }
    if(config->resolution.lat!=getcellsizecoord(celldata->soil.bin.file_coord))
    {
      if(config->rank==0)
        fprintf(stderr,"ERROR154: Cell size %g different from %g in '%s'.\n",
                getcellsizecoord(celldata->soil.bin.file_coord),
                config->resolution.lat,config->coord_filename.name);
      closecoord(celldata->soil.bin.file_coord);
      free(celldata);
      return NULL;
    }
    if(isroot(*config) && config->nall>numcoord(celldata->soil.bin.file_coord))
      fprintf(stderr,
              "WARNING003: Number of gridcells in '%s' distinct from %d.\n",
            config->coord_filename.name,numcoord(celldata->soil.bin.file_coord));

    /* Open soiltype file */
    celldata->soil.bin.file_soil=fopensoilcode(&config->soil_filename,
                                               &celldata->soil.bin.swap_soil,&celldata->soil.bin.version_soil,
                                               isroot(*config));
    if(celldata->soil.bin.file_soil==NULL)
    {
      closecoord(celldata->soil.bin.file_coord);
      free(celldata);
      return NULL;
    }
  }
  celldata->drainclass_fmt=config->drainclass_filename.fmt;
  if(config->drainclass_filename.fmt==CDF)
  {
    celldata->drainclass.cdf=openinput_netcdf(config->drainclass_filename.name,
                                              config->drainclass_filename.var,
                                              NULL,0,config);
    if(celldata->drainclass.cdf==NULL)
    {
      if(isroot(*config))
        printfopenerr(config->drainclass_filename.name);
      if(config->soil_filename.fmt==CDF)
        closecoord_netcdf(celldata->soil.cdf);
      else
      {
        closecoord(celldata->soil.bin.file_coord);
        fclose(celldata->soil.bin.file_soil);
      }
      free(celldata);
      return NULL;
    }
  }
  else
  {
    celldata->drainclass.file=openinputfile(&header,&swap,
                                            &config->drainclass_filename,
                                            headername,
                                            &version,config);
    if(celldata->drainclass.file==NULL)
    {
      if(isroot(*config))
        printfopenerr(config->drainclass_filename.name);
      if(config->soil_filename.fmt==CDF)
        closecoord_netcdf(celldata->soil.cdf);
      else
      {
        closecoord(celldata->soil.bin.file_coord);
        fclose(celldata->soil.bin.file_soil);
      }
      free(celldata);
      return NULL;
    }
  }
  return celldata;
} /* of 'opencelldata' */

Bool seekcelldata(Celldata celldata,int startgrid,int nsoil)
{
  if(celldata->soil_fmt==CDF)
  {
    if(seekcoord_netcdf(celldata->soil.cdf,startgrid))
    {
      fprintf(stderr,
              "ERROR109: Cannot seek in coordinate file to position %d.\n",
              startgrid);
      return TRUE;
    }
  }
  else
  {
    if(seekcoord(celldata->soil.bin.file_coord,startgrid))
    {
      /* seeking to position of first grid cell failed */
      fprintf(stderr,
              "ERROR109: Cannot seek in coordinate file to position %d.\n",
              startgrid);
      return TRUE;
    }
    if(seeksoilcode(celldata->soil.bin.file_soil,startgrid,celldata->soil.bin.version_soil,nsoil))
    {
      /* seeking to position of first grid cell failed */
      fprintf(stderr,"ERROR107: Cannot seek in soilcode file to position '%d'.\n",
              startgrid);
      return TRUE;
    }
  }
  if(celldata->drainclass_fmt!=CDF &&
     fseek(celldata->drainclass.file,startgrid,SEEK_CUR))
  {
    /* seeking to position of first grid cell failed */
    fprintf(stderr,
            "ERROR107: Cannot seek in drainage_class file to position '%d'.\n",
            startgrid);
    return TRUE;
  }
  return FALSE;
} /* of 'seekcelldata' */

Bool readcelldata(Celldata celldata,Coord *coord,int *soilcode,Real *k_perc,
                  int cell,Config *config)
{
  Byte drainageclass;
  if(celldata->soil_fmt==CDF)
  {
    if(readcoord_netcdf(celldata->soil.cdf,coord,config->resolution,soilcode))
    {
      fprintf(stderr,"ERROR190: Unexpected end of file in '%s' for cell %d.\n",
              config->soil_filename.name,cell+config->startgrid);
      return TRUE;
    }
  }
  else
  {
    if(readcoord(celldata->soil.bin.file_coord,coord,config->resolution))
    {
      fprintf(stderr,"ERROR190: Unexpected end of file in '%s' for cell %d.\n",
              config->coord_filename.name,cell+config->startgrid);
      return TRUE;
    }
    /* read soilcode from file */

    if(freadsoilcode(celldata->soil.bin.file_soil,soilcode,
                     celldata->soil.bin.swap_soil,config->nsoil))
    {
      fprintf(stderr,"ERROR190: Unexpected end of file in '%s' for cell %d.\n",
              config->soil_filename.name,cell+config->startgrid);
      config->ngridcell=cell;
      return TRUE;
    }
  }
  /* read drainage class from file */
  if(celldata->drainclass_fmt==CDF)
  {
    if(readinput_netcdf(celldata->drainclass.cdf,k_perc,coord,
                        &config->resolution))
    {
      fprintf(stderr,"ERROR190: Unexpected end of file in '%s' for cell %d.\n",
              config->drainclass_filename.name,cell+config->startgrid);
      return TRUE;
    }
  }
  else
  {
    if(fread(&drainageclass,sizeof(drainageclass),1,celldata->drainclass.file)!=1)
    {
      fprintf(stderr,"ERROR190: Unexpected end of file in '%s' for cell %d.\n",
              config->drainclass_filename.name,cell+config->startgrid);
      return TRUE;
    }
    *k_perc=(Real)drainageclass;
  }
  return FALSE;
} /* of 'readcelldata' */

void closecelldata(Celldata celldata)
{
  if(celldata->soil_fmt==CDF)
    closecoord_netcdf(celldata->soil.cdf);
  else
  {
    closecoord(celldata->soil.bin.file_coord);
    fclose(celldata->soil.bin.file_soil);
  }
  if(celldata->drainclass_fmt==CDF)
    closeinput_netcdf(celldata->drainclass.cdf);
  else
    fclose(celldata->drainclass.file);
  free(celldata);
} /* of 'closecelldata' */
