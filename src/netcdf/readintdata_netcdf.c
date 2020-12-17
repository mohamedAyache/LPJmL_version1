/***************************************************************************/
/**                                                                       **/
/**      r  e  a  d  i  n  t  d  a  t  a  _  n  e  t  c  d  f  .  c       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads data in NetCDF format                              **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-09-29 16:02:37 +0200 (lun., 29 sept. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#if defined(USE_NETCDF) || defined(USE_NETCDF4)
#include <netcdf.h>
#endif

Bool readintdata_netcdf(const Climatefile *file,int data[],const Cell grid[],
                        int year,const Config *config)
{
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  int cell,rc;
  int *f;
  short *s;
  size_t offsets[4];
  size_t counts[4];
  offsets[0]=year-file->firstyear;
  offsets[1]=offsets[2]=offsets[3]=0;
  counts[0]=1;
  counts[1]=file->nlat;
  counts[2]=file->nlon;
  counts[3]=file->var_len;
  switch(file->type)
  {
    case LPJ_INT:
      f=newvec(int,file->nlon*file->nlat*file->var_len);
      if(f==NULL)
      {
        printallocerr("data");
        rc=TRUE;
      }
      else if(isroot(*config))
      {
        if((rc=nc_get_vara_int(file->ncid,file->varid,offsets,counts,f)))
          fprintf(stderr,"ERROR421: Cannot read int data: %s.\n",
                  nc_strerror(rc)); 
      }
      else
        rc=FALSE;
      if(iserror(rc,config))
      {
        free(f);
        return TRUE;
      }
#ifdef USE_MPI
      MPI_Bcast(f,file->nlon*file->nlat*file->var_len,MPI_INT,0,config->comm);
#endif
      for(cell=0;cell<config->ngridcell;cell++)
      {
        if(file->offset)
          offsets[1]=file->offset-(int)((grid[cell].coord.lat-file->lat_min)/config->resolution.lat+0.5);
        else
          offsets[1]=(int)((grid[cell].coord.lat-file->lat_min)/config->resolution.lat+0.5);
        offsets[2]=(int)((grid[cell].coord.lon-file->lon_min)/config->resolution.lon+0.5);
        if(offsets[1]>=file->nlat || offsets[2]>=file->nlon)
        {
          fprintf(stderr,"ERROR422: Invalid coordinate for cell %d (%.2f, %.2f) in data file.\n",
                  cell+config->startgrid,grid[cell].coord.lat,grid[cell].coord.lon);
          free(f);
          return TRUE;
        }

        if(!grid[cell].skip && f[file->nlon*offsets[1]+offsets[2]]==file->missing_value.i)
        {
          fprintf(stderr,"ERROR423: Missing value for cell=%d (",
                  cell+config->startgrid);
          fprintcoord(stderr,grid[cell].coord);
          fputs(").\n",stderr);
          free(f);
          return TRUE;
        }
        data[cell]=f[file->nlon*offsets[1]+offsets[2]];
      }
      free(f);
      break;
    case LPJ_SHORT:
      s=newvec(short,file->nlon*file->nlat*file->var_len);
      if(s==NULL)
      {
        printallocerr("data");
        rc=TRUE;
      }
      else if(isroot(*config))
      {
        if((rc=nc_get_vara_short(file->ncid,file->varid,offsets,counts,s)))
          fprintf(stderr,"ERROR421: Cannot read short data: %s.\n",
                  nc_strerror(rc));
      }
      else
        rc=FALSE;
      if(iserror(rc,config))
      {
        free(s);
        return TRUE;
      }
#ifdef USE_MPI
      MPI_Bcast(s,file->nlon*file->nlat*file->var_len,MPI_SHORT,0,config->comm);
#endif
      for(cell=0;cell<config->ngridcell;cell++)
      {
        if(file->offset)
          offsets[1]=file->offset-(int)((grid[cell].coord.lat-file->lat_min)/config->resolution.lat+0.5);
        else
          offsets[1]=(int)((grid[cell].coord.lat-file->lat_min)/config->resolution.lat+0.5);
        offsets[2]=(int)((grid[cell].coord.lon-file->lon_min)/config->resolution.lon+0.5);
        if(offsets[1]>=file->nlat || offsets[2]>=file->nlon)
        {
          fprintf(stderr,"ERROR422: Invalid coordinate for cell %d (%.2f, %.2f) in data file.\n",
                  cell+config->startgrid,grid[cell].coord.lat,grid[cell].coord.lon);
          free(s);
          return TRUE;
        }

        if(!grid[cell].skip && s[file->nlon*offsets[1]+offsets[2]]==file->missing_value.s)
        {
          fprintf(stderr,"ERROR423: Missing value for cell=%d (",
                  cell+config->startgrid);
          fprintcoord(stderr,grid[cell].coord);
          fputs(").\n",stderr);
          free(s);
          return TRUE;
        }
        data[cell]=s[file->nlon*offsets[1]+offsets[2]];
      }
      free(s);
      break;
    default:
      if(isroot(*config))
        fputs("ERROR428: Invalid data type in NetCDF file.\n",stderr);
      return TRUE;
  } /* of switch(file->type) */
  return FALSE;
#else
  return TRUE;
#endif
} /* of 'readintdata_netcdf' */
