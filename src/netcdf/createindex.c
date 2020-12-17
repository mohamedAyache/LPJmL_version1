/***************************************************************************/
/**                                                                       **/
/**                c  r  e  a  t  e  i  n  d  e  x  .  c                  **/
/**                                                                       **/
/**     Function creates index vector for NetCDF output                   **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2013-07-10 09:36:11 +0200 (mer., 10 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Coord_array *createindex(const Coord *grid,int ngrid,Coord resolution)
{
  Coord_array *array;
  Real lon_max,lat_max;
  int cell;
  array=new(Coord_array);
  if(array==NULL)
    return NULL;
  array->index=newvec(int,ngrid);
  if(array->index==NULL)
  {
    printallocerr("index");
    free(array);
    return NULL;
  }
  array->lon_min=array->lat_min=1000;
  lon_max=lat_max=-1000;
  for(cell=0;cell<ngrid;cell++)
  {
    if(array->lon_min>grid[cell].lon)
      array->lon_min=grid[cell].lon;
    if(lon_max<grid[cell].lon)
      lon_max=grid[cell].lon;
    if(array->lat_min>grid[cell].lat)
      array->lat_min=grid[cell].lat;
    if(lat_max<grid[cell].lat)
      lat_max=grid[cell].lat;
  }
  array->nlon=(int)((lon_max-array->lon_min)/resolution.lon+0.5)+1;
  array->nlat=(int)((lat_max-array->lat_min)/resolution.lat+0.5)+1;
  for(cell=0;cell<ngrid;cell++)
  {
   array->index[cell]=(int)((grid[cell].lon-array->lon_min)/resolution.lon+0.5)+
                      (int)((grid[cell].lat-array->lat_min)/resolution.lat+0.5)*array->nlon;
#ifdef SAFE
   if(array->index[cell]<0 || array->index[cell]>=array->nlon*array->nlat)
     fprintf(stderr,"Invalid index %d.\n",array->index[cell]);
#endif
  }
  return array;
} /* of 'createindex' */
