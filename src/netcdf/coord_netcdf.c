/***************************************************************************/
/**                                                                       **/
/**        c  o  o  r  d  _  n  e  t  c  d  f  .  c                       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function opens soil data file in NetCDF format                    **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date::                                            $ **/
/**     By         : $Author::                                 $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#if defined(USE_NETCDF) || defined(USE_NETCDF4)
#include <netcdf.h>

#define error(var,rc) if(rc){ if(isout) fprintf(stderr,"ERROR403: Cannot read '%s' in '%s': %s.\n",var,filename,nc_strerror(rc)); nc_close(coord->ncid); free(coord); return NULL;}
#endif

struct coord_netcdf
{
  int ncid;
  int varid;
  Type type;
  union
  {
    Byte b;
    int i;
    short s;
  } missing_value;
  float *lon;
  float *lat;
  size_t lon_len,lat_len;
  size_t offsets[2];
};

void closecoord_netcdf(Coord_netcdf coord)
{
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  if(coord!=NULL)
  {
    free(coord->lon);
    free(coord->lat);
    nc_close(coord->ncid);
    free(coord);
  }
#endif
} /* of 'closecoord_netcdf' */

int numcoord_netcdf(const Coord_netcdf coord)
{
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  int count;
  size_t i;
  short *soil;
  int *isoil;
  Byte *bsoil;
  count=0;
  if(coord==NULL)
    return -1;
  switch(coord->type)
  {
    case LPJ_SHORT:
      soil=newvec(short,coord->lat_len*coord->lon_len);
      if(soil==NULL)
        return -1;
      if(nc_get_var_short(coord->ncid,coord->varid,soil))
      {
        free(soil);
        return -1;
      }
      for(i=0;i<coord->lat_len*coord->lon_len;i++)
        if(soil[i]!=coord->missing_value.s)
          count++;
      free(soil);
      break;
    case LPJ_INT:
      isoil=newvec(int,coord->lat_len*coord->lon_len);
      if(isoil==NULL)
        return -1;
      if(nc_get_var_int(coord->ncid,coord->varid,isoil))
      {
        free(isoil);
        return -1;
      }
      for(i=0;i<coord->lat_len*coord->lon_len;i++)
        if(isoil[i]!=coord->missing_value.i)
          count++;
      free(isoil);
      break;
    case LPJ_BYTE:
      bsoil=malloc(coord->lat_len*coord->lon_len);
      if(bsoil==NULL)
        return -1;
      if(nc_get_var_uchar(coord->ncid,coord->varid,bsoil))
      {
        free(bsoil);
        return -1;
      }
      for(i=0;i<coord->lat_len*coord->lon_len;i++)
        if(bsoil[i]!=coord->missing_value.b)
          count++;
      free(bsoil);
      break;
  }
  return count;
#else
  return -1;
#endif
} /* of 'numcoord_netcdf' */

int *getindexcoord_netcdf(const Coord_netcdf coord)
{
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  int count;
  size_t i;
  int *index;
  short *soil;
  int *isoil;
  Byte *bsoil;
  count=0;
  if(coord==NULL)
    return NULL;
  index=newvec(int,coord->lat_len*coord->lon_len);
  if(index==NULL)
    return NULL;
  switch(coord->type)
  {
    case LPJ_SHORT:
      soil=newvec(short,coord->lat_len*coord->lon_len);
      if(soil==NULL)
      {
        free(index);
        return NULL;
      }
      if(nc_get_var_short(coord->ncid,coord->varid,soil))
      {
        free(index);
        free(soil);
        return NULL;
      }
      for(i=0;i<coord->lat_len*coord->lon_len;i++)
        if(soil[i]==coord->missing_value.s)
          index[i]=-1;
        else
          index[i]=count++;
      free(soil);
      break;
    case LPJ_INT:
      isoil=newvec(int,coord->lat_len*coord->lon_len);
      if(isoil==NULL)
      {
        free(index);
        return NULL;
      }
      if(nc_get_var_int(coord->ncid,coord->varid,isoil))
      {
        free(index);
        free(isoil);
        return NULL;
      }
      for(i=0;i<coord->lat_len*coord->lon_len;i++)
        if(isoil[i]==coord->missing_value.i)
          index[i]=-1;
        else
          index[i]=count++;
      free(isoil);
      break;
    case LPJ_BYTE:
      bsoil=malloc(coord->lat_len*coord->lon_len);
      if(bsoil==NULL)
      {
        free(index);
        return NULL;
      }
      if(nc_get_var_uchar(coord->ncid,coord->varid,bsoil))
      {
        free(index);
        free(bsoil);
        return NULL;
      }
      for(i=0;i<coord->lat_len*coord->lon_len;i++)
        if(bsoil[i]==coord->missing_value.b)
          index[i]=-1;
        else
          index[i]=count++;
      free(bsoil);
      break;
  }
  return index;
#else
  return NULL;
#endif
} /* of 'getindexcoord_netcdf' */

Bool seekcoord_netcdf(Coord_netcdf coord,int pos)
{
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  size_t counts[2]={1,1};
  short soil;
  int isoil;
  Byte bsoil;
  int count;
  count=0;
  if(coord==NULL)
    return TRUE;
  switch(coord->type)
  {
    case LPJ_SHORT:
      for(coord->offsets[0]=0;coord->offsets[0]<coord->lat_len;coord->offsets[0]++)
        for(coord->offsets[1]=0;coord->offsets[1]<coord->lon_len;coord->offsets[1]++)
        {
          if(nc_get_vara_short(coord->ncid,coord->varid,coord->offsets,counts,&soil))
            return TRUE;
          if(soil!=coord->missing_value.s)
          {
            if(count==pos)
              return FALSE;
            else
              count++;
          }
        }
      break;
    case LPJ_INT:
      for(coord->offsets[0]=0;coord->offsets[0]<coord->lat_len;coord->offsets[0]++)
        for(coord->offsets[1]=0;coord->offsets[1]<coord->lon_len;coord->offsets[1]++)
        {
          if(nc_get_vara_int(coord->ncid,coord->varid,coord->offsets,counts,&isoil))
            return TRUE;
          if(isoil!=coord->missing_value.i)
          {
            if(count==pos)
              return FALSE;
            else
              count++;
          }
        }
      break;
    case LPJ_BYTE:
      for(coord->offsets[0]=0;coord->offsets[0]<coord->lat_len;coord->offsets[0]++)
        for(coord->offsets[1]=0;coord->offsets[1]<coord->lon_len;coord->offsets[1]++)
        {
          if(nc_get_vara_uchar(coord->ncid,coord->varid,coord->offsets,counts,&bsoil))
            return TRUE;
          if(bsoil!=coord->missing_value.b)
          {
            if(count==pos)
              return FALSE;
            else
              count++;
          }
        }
      break;
  }
#endif
  return TRUE;
} /* of 'seekcord_netcdf' */

Bool readcoord_netcdf(Coord_netcdf coord,Coord *c,Coord resol,int *soil)
{
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  short data;
  int idata;
  Byte bdata;
  size_t counts[2]={1,1};
  if(coord==NULL || c==NULL)
    return TRUE;
  switch(coord->type)
  {
    case LPJ_SHORT:
      for(;coord->offsets[0]<coord->lat_len;coord->offsets[0]++)
      {
        for(;coord->offsets[1]<coord->lon_len;coord->offsets[1]++)
        {
          if(nc_get_vara_short(coord->ncid,coord->varid,coord->offsets,counts,&data))
            return TRUE;
          if(data!=coord->missing_value.s)
          {
             c->lat=coord->lat[coord->offsets[0]];
             c->lon=coord->lon[coord->offsets[1]];
             c->area=cellarea(*c,resol);
             if(coord->offsets[1]==coord->lon_len-1)
             {
               coord->offsets[1]=0;
               coord->offsets[0]++;
             }
             else
               coord->offsets[1]++;
             *soil=data;
             return FALSE;
          }
        }
        coord->offsets[1]=0;
      }
      break;
    case LPJ_INT:
      for(;coord->offsets[0]<coord->lat_len;coord->offsets[0]++)
      {
        for(;coord->offsets[1]<coord->lon_len;coord->offsets[1]++)
        {
          if(nc_get_vara_int(coord->ncid,coord->varid,coord->offsets,counts,&idata))
            return TRUE;
          if(idata!=coord->missing_value.i)
          {
             c->lat=coord->lat[coord->offsets[0]];
             c->lon=coord->lon[coord->offsets[1]];
             c->area=cellarea(*c,resol);
             if(coord->offsets[1]==coord->lon_len-1)
             {
               coord->offsets[1]=0;
               coord->offsets[0]++;
             }
             else
               coord->offsets[1]++;
             *soil=idata;
             return FALSE;
          }
        }
        coord->offsets[1]=0;
      }
      break;
    case LPJ_BYTE:
      for(;coord->offsets[0]<coord->lat_len;coord->offsets[0]++)
      {
        for(;coord->offsets[1]<coord->lon_len;coord->offsets[1]++)
        {
          if(nc_get_vara_uchar(coord->ncid,coord->varid,coord->offsets,counts,&bdata))
            return TRUE;
          if(bdata!=coord->missing_value.b)
          {
             c->lat=coord->lat[coord->offsets[0]];
             c->lon=coord->lon[coord->offsets[1]];
             c->area=cellarea(*c,resol);
             if(coord->offsets[1]==coord->lon_len-1)
             {
               coord->offsets[1]=0;
               coord->offsets[0]++;
             }
             else
               coord->offsets[1]++;
             *soil=bdata;
             return FALSE;
          }
        }
        coord->offsets[1]=0;
      }
      break;
  }
#endif
  return TRUE;
} /* of 'readcoord_netcdf' */

Coord_netcdf opencoord_netcdf(const char *filename,const char *var,Bool isout)
{
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  Coord_netcdf coord;
  int i,rc,var_id,nvars,*dimids,ndims;
  char name[NC_MAX_NAME+1];
  nc_type type;
  if(filename==NULL)
    return NULL;
  coord=new(struct coord_netcdf);
  if(coord==NULL)
  {
    printallocerr("coord");
    return NULL;
  }
  rc=nc_open(filename,NC_NOWRITE,&coord->ncid);
  if(rc)
  {
    if(isout)
      fprintf(stderr,"ERROR409: Cannot open '%s': %s.\n",
              filename,nc_strerror(rc));
    free(coord);
    return NULL;
  }
  if(var==NULL)
  {
    nc_inq_nvars(coord->ncid,&nvars);
    for(i=0;i<nvars;i++)
    {
      nc_inq_varname(coord->ncid,i,name);
      if(strcmp(name,LON_NAME) && strcmp(name,LAT_NAME) && strcmp(name,TIME_NAME))
      {
        coord->varid=i;
        break;
      }
    }
    if(i==nvars)
    {
      if(isout)
        fprintf(stderr,"ERROR405: No variable found in '%s'.\n",filename);
      closecoord_netcdf(coord);
      free(coord);
      return NULL;
    }
  }
  else if(nc_inq_varid(coord->ncid,var,&coord->varid))
  {
    if(isout)
      fprintf(stderr,"ERROR406: Cannot find variable '%s' in '%s'.\n",
              var,filename);
    closecoord_netcdf(coord);
    free(coord);
    return NULL;
  }
  nc_inq_varndims(coord->ncid,coord->varid,&ndims);
  if(ndims!=2)
  {
    if(isout)
      fprintf(stderr,"ERROR408: Invalid number of dimensions %d in '%s'.\n",
              ndims,filename);
    closecoord_netcdf(coord);
    free(coord);
    return NULL;
  }
  dimids=newvec(int,ndims);
  if(dimids==NULL)
  {
    printallocerr("dimids");
    closecoord_netcdf(coord);
    free(coord);
    return NULL;
  }
  nc_inq_vardimid(coord->ncid,coord->varid,dimids);
  nc_inq_dimname(coord->ncid,dimids[ndims-1],name);
  rc=nc_inq_varid(coord->ncid,name,&var_id);
  if(rc)
  {
    if(isout)
      fprintf(stderr,"ERROR410: Cannot read %s in '%s': %s.\n",
              name,filename,nc_strerror(rc));
    free(dimids);
    closecoord_netcdf(coord);
    return NULL;
  }
  nc_inq_dimlen(coord->ncid,dimids[ndims-1],&coord->lon_len);
  coord->lon=newvec(float,coord->lon_len);
  if(coord->lon==NULL)
  {
    printallocerr("lon");
    free(dimids);
    nc_close(coord->ncid);
    free(coord);
    return NULL;
  }
  rc=nc_get_var_float(coord->ncid,var_id,coord->lon);
  if(rc)
  {
    if(isout)
      fprintf(stderr,"ERROR411: Cannot read longitude in '%s': %s.\n",
              filename,nc_strerror(rc));
    free(coord->lon);
    free(dimids);
    nc_close(coord->ncid);
    free(coord);
    return NULL;
  }
  nc_inq_dimname(coord->ncid,dimids[ndims-2],name);
  rc=nc_inq_varid(coord->ncid,name,&var_id);
  if(rc)
  {
    if(isout)
      fprintf(stderr,"ERROR410: Cannot read %s in '%s': %s.\n",
              name,filename,nc_strerror(rc));
    free(dimids);
    free(coord->lon);
    nc_close(coord->ncid);
    free(coord);
    return NULL;
  }
  nc_inq_dimlen(coord->ncid,dimids[ndims-2],&coord->lat_len);
  free(dimids);
  coord->lat=newvec(float,coord->lat_len);
  if(coord->lat==NULL)
  {
    printallocerr("lat");
    free(coord->lon);
    nc_close(coord->ncid);
    free(coord);
    return NULL;
  }
  rc=nc_get_var_float(coord->ncid,var_id,coord->lat);
  if(rc)
  {
    if(isout)
      fprintf(stderr,"ERROR404: Cannot read latitude in '%s': %s.\n",
              filename,nc_strerror(rc));
    free(coord->lon);
    free(coord->lat);
    closecoord_netcdf(coord);
    free(coord);
    return NULL;
  }
  nc_inq_vartype(coord->ncid,coord->varid,&type);
  switch(type)
  {
    case NC_INT:
      rc=nc_get_att_int(coord->ncid,coord->varid,"missing_value",&coord->missing_value.i);
      if(rc)
      {
        rc=nc_get_att_int(coord->ncid,coord->varid,"_FillValue",&coord->missing_value.i);
        if(rc)
          coord->missing_value.i=MISSING_VALUE_INT;
      }
      coord->type=LPJ_INT;
      break;
    case NC_SHORT:
      rc=nc_get_att_short(coord->ncid,coord->varid,"missing_value",&coord->missing_value.s);
      if(rc)
      {
        rc=nc_get_att_short(coord->ncid,coord->varid,"_FillValue",&coord->missing_value.s);
        if(rc)
          coord->missing_value.s=MISSING_VALUE_SHORT;
      }
      coord->type=LPJ_SHORT;
      break;
    case NC_BYTE:
      rc=nc_get_att_uchar(coord->ncid,coord->varid,"missing_value",&coord->missing_value.b);
      if(rc)
      {
        rc=nc_get_att_uchar(coord->ncid,coord->varid,"_FillValue",&coord->missing_value.b);
        if(rc)
          coord->missing_value.s=MISSING_VALUE_BYTE;
      }
      coord->type=LPJ_BYTE;
      break;
    default:
      if(isout)
        fprintf(stderr,"ERROR428: Invalid data type of %s in '%s'.\n",
                (var==NULL) ? name :var,filename);
      free(coord->lon);
      free(coord->lat);
      closecoord_netcdf(coord);
      free(coord);
      return NULL;
  }
  if(rc)
  {
    if(isout)
      fprintf(stderr,"WARNING402: Cannot read missing value of %s in '%s': %s.\n",
              (var==NULL) ? name : var,filename,nc_strerror(rc));
  }
  coord->offsets[0]=coord->offsets[1]=0;
  return coord;
#else
  if(isout)
    fputs("ERROR401: NetCDF input is not supported by this version of LPJmL.\n",stderr);
  return NULL;
#endif
} /* of 'opencoord_netcdf' */

void getresolution_netcdf(const Coord_netcdf coord,Coord *resolution)
{
  resolution->lon=coord->lon[1]-coord->lon[0];
  resolution->lat=fabs(coord->lat[1]-coord->lat[0]);
} /* of 'getresolution_netcdf' */

void getextension_netcdf(Extension *ext,const Coord_netcdf coord)
{
  ext->lon_min=coord->lon[0];
  ext->lon_max=coord->lon[coord->lon_len-1];
  if(coord->lat[1]>coord->lat[0])
  {
    ext->lat_min=coord->lat[0];
    ext->lat_max=coord->lat[coord->lat_len-1];
  }
  else
  {
    ext->lat_min=coord->lat[coord->lat_len-1];
    ext->lat_max=coord->lat[0];
  }
} /* of 'getextension_netcdf' */
