/***************************************************************************/
/**                                                                       **/
/**                c  r  e  a  t  e  c  o  o  r  d  .  c                  **/
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

Coord_array *createcoord(Outputfile *output,
                         const Cell grid[],
                         const Config *config)
{
  Coord_array *array;
  Real *lon,*lat;
#ifdef USE_MPI
  Real *vec;
#endif
  Real lon_max,lat_max;
  int cell,count;
  Bool iserr;
  if(config->total==0)
  {
    if(isroot(*config))
      fputs("ERROR429: No cell with valid soil code found in create_coord().\n",stderr);
    return NULL;
  }
  iserr=FALSE;
  array=new(Coord_array);
  if(array==NULL)
  {
    printallocerr("index");    
    iserr=TRUE;
  }
  if(isroot(*config))
  {
    lat=newvec(Real,config->total);
    if(lat==NULL)
    {
      printallocerr("lat");
      iserr=TRUE;
    }
    lon=newvec(Real,config->total);
    if(lon==NULL)
    {
      printallocerr("lon");
      iserr=TRUE;
    }
  }
  else
    array->index=NULL;
  count=0;
#ifdef USE_MPI
  vec=newvec(Real,config->count);
  if(vec==NULL)
  {
    printallocerr("vec");
    iserr=TRUE;
  }
  if(iserror(iserr,config))
  {
    free(vec);
    free(array);
    if(isroot(*config))
    {
      free(lat);
      free(lon);
    }
    return NULL;
  }
  for(cell=0;cell<config->ngridcell;cell++)
    if(!grid[cell].skip)
      vec[count++]=grid[cell].coord.lon;
  MPI_Gatherv(vec,config->count,
              (sizeof(Real)==sizeof(double)) ? MPI_DOUBLE : MPI_FLOAT,
              lon,output->counts,output->offsets,
              (sizeof(Real)==sizeof(double)) ? MPI_DOUBLE : MPI_FLOAT,
              0,config->comm);
  count=0;
  for(cell=0;cell<config->ngridcell;cell++)
    if(!grid[cell].skip)
      vec[count++]=grid[cell].coord.lat;
  MPI_Gatherv(vec,config->count,
              (sizeof(Real)==sizeof(double)) ? MPI_DOUBLE : MPI_FLOAT,
              lat,output->counts,output->offsets,
              (sizeof(Real)==sizeof(double)) ? MPI_DOUBLE : MPI_FLOAT,
              0,config->comm);
  free(vec);
#else
  if(iserr)
  {
    free(array);
    free(lat);
    free(lon);
    return NULL;
  }
  for(cell=0;cell<config->ngridcell;cell++)
    if(!grid[cell].skip)
    {
      lon[count]=grid[cell].coord.lon;
      lat[count++]=grid[cell].coord.lat;
    }
#endif
  if(isroot(*config))
  {
    array->index=newvec(int,config->total);
    if(array->index==NULL)
    {
      printallocerr("index");    
      iserr=TRUE;
    }
    else
    {
      array->lon_min=array->lat_min=1000;
      lon_max=lat_max=-1000;
      for(cell=0;cell<config->total;cell++)
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
      for(cell=0;cell<config->total;cell++)
      {
        array->index[cell]=(int)((lon[cell]-array->lon_min)/config->resolution.lon+0.5)+
                           (int)((lat[cell]-array->lat_min)/config->resolution.lat+0.5)*array->nlon;
#ifdef SAFE
        if(array->index[cell]<0 || array->index[cell]>=array->nlon*array->nlat)
        {
          fprintf(stderr,"ERROR433: Invalid index %d in createcoord().\n",array->index[cell]);
          free(array->index);
          break;
        }
#endif
      }
      iserr=(cell<config->total);
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
} /* of 'createcoord' */
