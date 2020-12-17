/***************************************************************************/
/**                                                                       **/
/**           c  r  e  a  t  e  c  o  o  r  d  _  a  l  l  .  c           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function creates index vector for NetCDF output                   **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-09-25 11:06:09 +0200 (jeu., 25 sept. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Coord_array *createcoord_all(const Cell grid[],const Config *config)
{
  Coord_array *array;
  Real *lon,*lat;
#ifdef USE_MPI
  Real *vec;
  int *counts,*offsets;
#endif
  Real lon_max,lat_max;
  int cell;
  Bool iserr;
  array=new(Coord_array);
  iserr=FALSE;
  if(array==NULL)
  {
    printallocerr("index");    
    iserr=TRUE;
  }
  if(isroot(*config))
  {
    lat=newvec(Real,config->nall);
    if(lat==NULL)
    {
      printallocerr("lat");
      iserr=TRUE;
    }
    lon=newvec(Real,config->nall);
    if(lon==NULL)
    {
      printallocerr("lon");
      iserr=TRUE;
    }
  }
  else
    array->index=NULL;
#ifdef USE_MPI
  vec=newvec(Real,config->ngridcell);
  if(vec==NULL)
  {
    printallocerr("vec");
    iserr=TRUE;
  }
  counts=newvec(int,config->ntask);
  if(counts==NULL)
  {
    printallocerr("counts");
    iserr=TRUE;
  }
  offsets=newvec(int,config->ntask);
  if(offsets==NULL)
  {
    printallocerr("offsets");
    iserr=TRUE;
  }
  if(iserror(iserr,config))
  {
    free(vec);
    free(counts);
    free(offsets);
    free(array);
    if(isroot(*config))
    {
      free(lat);
      free(lon);
    }
    return NULL;
  }
  getcounts(counts,offsets,config->nall,1,config->ntask);
  for(cell=0;cell<config->ngridcell;cell++)
    vec[cell]=grid[cell].coord.lon;
  MPI_Gatherv(vec,config->ngridcell,
              (sizeof(Real)==sizeof(double)) ? MPI_DOUBLE : MPI_FLOAT,
              lon,counts,offsets,
              (sizeof(Real)==sizeof(double)) ? MPI_DOUBLE : MPI_FLOAT,
              0,config->comm);
  for(cell=0;cell<config->ngridcell;cell++)
    vec[cell]=grid[cell].coord.lat;
  MPI_Gatherv(vec,config->ngridcell,
              (sizeof(Real)==sizeof(double)) ? MPI_DOUBLE : MPI_FLOAT,
              lat,counts,offsets,
              (sizeof(Real)==sizeof(double)) ? MPI_DOUBLE : MPI_FLOAT,
              0,config->comm);
  free(vec);
  free(counts);
  free(offsets);
#else
  if(iserr)
  {
    free(lat);
    free(lon);
    free(array);
    return NULL;
  } 
  for(cell=0;cell<config->ngridcell;cell++)
  {
    lon[cell]=grid[cell].coord.lon;
    lat[cell]=grid[cell].coord.lat;
  }
#endif
  if(isroot(*config))
  {
    array->index=newvec(int,config->nall);
    if(array->index==NULL)
    {
      printallocerr("index");    
      iserr=TRUE;
    }
    else
    {
      array->lon_min=array->lat_min=1000;
      lon_max=lat_max=-1000;
      for(cell=0;cell<config->nall;cell++)
      {
        if(array->lon_min>lon[cell])
          array->lon_min=lon[cell]; 
        if(lon_max<lon[cell])
          lon_max=lon[cell]; 
        if(array->lat_min>lat[cell])
          array->lat_min=lat[cell]; 
        if(lat_max<lat[cell])
          lat_max=lat[cell];
      }
      array->nlon=(int)((lon_max-array->lon_min)/config->resolution.lon)+1;
      array->nlat=(int)((lat_max-array->lat_min)/config->resolution.lat)+1;
      for(cell=0;cell<config->nall;cell++)
      {
        array->index[cell]=(int)((lon[cell]-array->lon_min)/config->resolution.lon+0.5)+
                         (int)((lat[cell]-array->lat_min)/config->resolution.lat+0.5)*array->nlon;
#ifdef SAFE
        if(array->index[cell]<0 || array->index[cell]>=array->nlon*array->nlat)
        {
          fprintf(stderr,"ERROR433: Invalid index %d in createcoord_all().\n",array->index[cell]);
          free(array->index);
          break;
        }
#endif
      }
      iserr=(cell<config->nall);
    }
    free(lon);
    free(lat);
  }
#ifdef USE_MPI
  MPI_Bcast(&iserr,1,MPI_INT,0,config->comm);
#endif
  if(iserr)
  {
    free(array);
    array=NULL;
  } 
  return array;
} /* of 'createcoord_all' */
