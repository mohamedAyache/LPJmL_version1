/***************************************************************************/
/**                                                                       **/
/**           r  e  a  d  d  a  t  a  _  n  e  t  c  d  f  .  c           **/
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

Bool readdata_netcdf(const Climatefile *file,Real data[],const Cell grid[],
                     int year,const Config *config)
{
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  int cell,rc,start,i;
  float *f;
  short *s;
  size_t offsets[4];
  size_t counts[4];
  offsets[0]=year;
  offsets[1]=offsets[2]=offsets[3]=0;
  counts[0]=1;
  if(file->var_len>1)
  {
    counts[1]=file->var_len;
    start=2;
  }
  else
    start=1;
  counts[start]=file->nlat;
  counts[start+1]=file->nlon;
  switch(file->type)
  {
    case LPJ_FLOAT:
      f=newvec(float,file->nlon*file->nlat*file->var_len);
      if(f==NULL)
      {
        printallocerr("data");
        rc=TRUE;
      }
      else if(isroot(*config))
      { 
        if((rc=nc_get_vara_float(file->ncid,file->varid,offsets,counts,f)))
          fprintf(stderr,"ERROR421: Cannot read float data: %s.\n",
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
      MPI_Bcast(f,file->nlon*file->nlat*file->var_len,MPI_FLOAT,0,config->comm);
#endif
      for(cell=0;cell<config->ngridcell;cell++)
      {
        if(file->offset)
          offsets[start]=file->offset-(int)((grid[cell].coord.lat-file->lat_min)/config->resolution.lat+0.5);
        else
          offsets[start]=(int)((grid[cell].coord.lat-file->lat_min)/config->resolution.lat+0.5);
        offsets[start+1]=(int)((grid[cell].coord.lon-file->lon_min)/config->resolution.lon+0.5);
        if(offsets[start]>=file->nlat || offsets[start+1]>=file->nlon)
        {
          fprintf(stderr,"ERROR422: Invalid coordinate for cell %d (",
                  cell+config->startgrid);
          fprintcoord(stderr,grid[cell].coord);
          fputs(") in data file.\n",stderr);
          free(f);
          return TRUE;
        }

        for(i=0;i<file->var_len;i++)
        {
          if(!grid[cell].skip && f[file->nlon*file->nlat*i+file->nlon*offsets[start]+offsets[start+1]]==file->missing_value.f)
          {
            fprintf(stderr,"ERROR423: Missing value for cell=%d (",
                    cell+config->startgrid);
            fprintcoord(stderr,grid[cell].coord);
            fputs(").\n",stderr);
            free(f);
            return TRUE;
          }
          data[cell*file->var_len+i]=file->slope*f[file->nlon*file->nlat*i+file->nlon*offsets[start]+offsets[start+1]]+file->intercept;
        }
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
          offsets[start]=file->offset-(int)((grid[cell].coord.lat-file->lat_min)/config->resolution.lat+0.5);
        else
          offsets[start]=(int)((grid[cell].coord.lat-file->lat_min)/config->resolution.lat+0.5);
        offsets[start+1]=(int)((grid[cell].coord.lon-file->lon_min)/config->resolution.lon+0.5);
        if(offsets[start]>=file->nlat || offsets[start+1]>=file->nlon)
        {
          fprintf(stderr,"ERROR422: Invalid coordinate for cell %d (",
                  cell+config->startgrid);
          fprintcoord(stderr,grid[cell].coord);
          fputs(") in data file.\n",stderr);
          free(s);
          return TRUE;
        }

        for(i=0;i<file->var_len;i++)
        {
          if(!grid[cell].skip && s[file->nlon*file->nlat*i+file->nlon*offsets[start]+offsets[start+1]]==file->missing_value.s)
          {
            fprintf(stderr,"ERROR423: Missing value for cell=%d (",
                    cell+config->startgrid);
            fprintcoord(stderr,grid[cell].coord);
            fputs(").\n",stderr);
            free(s);
            return TRUE;
          }
          data[cell*file->var_len+i]=file->slope*s[file->nlon*file->nlat*i+file->nlon*offsets[1]+offsets[2]]+file->intercept;
        }
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
} /* of 'readdata_netcdf' */
