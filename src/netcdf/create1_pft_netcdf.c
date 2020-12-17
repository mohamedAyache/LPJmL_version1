/***************************************************************************/
/**                                                                       **/
/**      c  r  e  a  t  e  1  _  p  f  t  _  n  e  t  c  d  f  .  c       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function creates NetCDF file for PFT output                       **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#if defined(USE_NETCDF) || defined(USE_NETCDF4)
#include <netcdf.h>
#include <time.h>

static nc_type nctype[]={NC_BYTE,NC_SHORT,NC_INT,NC_FLOAT,NC_DOUBLE};
#endif

#define error(rc) if(rc) {free(lon);free(lat);free(layer);fprintf(stderr,"ERROR427: Cannot write '%s': %s.\n",filename,nc_strerror(rc)); nc_close(cdf->ncid); return TRUE;}

Bool create1_pft_netcdf(Netcdf *cdf,
                        const char *filename, /* filename of NetCDF file */
                        int index,
                        int npft,             /* number of PFTs */
                        int ncft,
                        const char *name,     /* name of output variable */
                        const char *descr,    /* description of output variable */
                        const char *units,    /* unit of output variable */
                        Type type,            /* type of output variable */
                        const Coord_array *array, /* coordinate array */
                        const Config *config  /* LPJ configuration */
                       )                      /* returns TRUE on error */
{
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  String s;
  time_t t;
  int i,rc,imiss=MISSING_VALUE_INT,size;
  short smiss=MISSING_VALUE_SHORT;
  float *lon,*lat,miss=config->missing_value,*layer;
  int dim[3];
  int lon_dim_id,lat_dim_id,lon_var_id,lat_var_id,pft_dim_id,pft_var_id;
  char **pftnames;
#ifndef USE_NETCDF4
  int dimids[2],pft_len_id;
  size_t offset[2],count[2],pft_len;
#endif
  if(array==NULL || name==NULL || filename==NULL)
  {
    fputs("ERROR424: Invalid array pointer in create1_pft_netcdf().\n",stderr);
    return TRUE;
  }
  cdf->missing_value=config->missing_value;
  lon=newvec(float,array->nlon);
  if(lon==NULL)
  {
    printallocerr("lon");
    return TRUE;
  }
  lat=newvec(float,array->nlat);
  if(lat==NULL)
  {
    free(lon);
    printallocerr("lat");
    return TRUE;
  }
  size=outputsize(index,npft,config->nbiomass,config->nagtree,ncft);
  if(index==SOILC_LAYER)
  {
    layer=newvec(float,size);
    if(layer==NULL)
    {
      free(lon);
      free(lat);
      printallocerr("layer");
      return TRUE;
    }
    layer[0]=0;
    for(i=1;i<size;i++)
     layer[i]=(float)soildepth[i-1];
  }
  else layer=NULL;

  lon[0]=(float)array->lon_min;
  for(i=1;i<array->nlon;i++)
    lon[i]=lon[i-1]+(float)config->resolution.lon;
  lat[0]=(float)array->lat_min;
  for(i=1;i<array->nlat;i++)
    lat[i]=lat[i-1]+(float)config->resolution.lat;
  cdf->index=array;
#ifdef USE_NETCDF4
  rc=nc_create(filename,NC_CLOBBER|NC_NETCDF4,&cdf->ncid);
#else
  rc=nc_create(filename,NC_CLOBBER,&cdf->ncid);
#endif
  if(rc)
  {
    fprintf(stderr,"ERROR426: Cannot create file '%s': %s.\n",
            filename,nc_strerror(rc));
    free(lon);
    free(lat);
    free(layer);
    return TRUE;
  }
  error(rc);
  rc=nc_def_dim(cdf->ncid,(index==SOILC_LAYER) ? "nsoil" : "npft",size,&pft_dim_id);
  error(rc);
  rc=nc_def_dim(cdf->ncid,LAT_DIM_NAME,array->nlat,&lat_dim_id);
  error(rc);
  rc=nc_def_dim(cdf->ncid,LON_DIM_NAME,array->nlon,&lon_dim_id);
  error(rc);
  dim[0]=pft_dim_id;
  dim[1]=lat_dim_id;
  dim[2]=lon_dim_id;
  if(index==SOILC_LAYER)
  {
    rc=nc_def_var(cdf->ncid,"layer",NC_FLOAT,1,&pft_dim_id,&pft_var_id);
    error(rc);
    rc=nc_put_att_text(cdf->ncid,pft_var_id,"units",strlen("mm"),"mm");
  }
  else
  {
    pftnames=createpftnames(index,npft,config->nbiomass,config->nagtree,ncft,config->pftpar);
    if(pftnames==NULL)
    {
      free(lon);
      free(lat);
      free(layer);
      printallocerr("pftnames");
      nc_close(cdf->ncid);
      return TRUE;
    }
#ifdef USE_NETCDF4
    rc=nc_def_var(cdf->ncid,"NamePFT",NC_STRING,1,&pft_dim_id,&pft_var_id);
#else
    pft_len=0;
    for(i=0;i<size;i++)
      if(pft_len<strlen(pftnames[i]))
        pft_len=strlen(pftnames[i]);
    pft_len++;
    rc=nc_def_dim(cdf->ncid,"len",pft_len,&pft_len_id);
    error(rc);
    dimids[0]=pft_dim_id;
    dimids[1]=pft_len_id;
    rc=nc_def_var(cdf->ncid,"NamePFT",NC_CHAR,2,dimids,&pft_var_id);
#endif
  }
  error(rc);
  rc=nc_def_var(cdf->ncid,LAT_NAME,NC_FLOAT,1,&lat_dim_id,&lat_var_id);
  error(rc);
  rc=nc_put_att_text(cdf->ncid,lat_var_id,"units",strlen("degrees_north"),
                     "degrees_north");
  error(rc);
  rc=nc_put_att_text(cdf->ncid, lat_var_id,"long_name",strlen("latitude"),"latitude");
  error(rc);
  rc=nc_put_att_text(cdf->ncid, lat_var_id,"standard_name",strlen("latitude"),"latitude");
  error(rc);
  rc=nc_put_att_text(cdf->ncid, lat_var_id,"axis",strlen("Y"),"Y");
  error(rc);
  rc=nc_def_var(cdf->ncid,LON_NAME,NC_FLOAT,1,&lon_dim_id,&lon_var_id);
  error(rc);
  rc=nc_put_att_text(cdf->ncid,lon_var_id,"units",strlen("degrees_east"),
                     "degrees_east");
  error(rc);
  rc=nc_put_att_text(cdf->ncid, lon_var_id,"long_name",strlen("longitude"),"longitude");
  error(rc);
  rc=nc_put_att_text(cdf->ncid, lon_var_id,"standard_name",strlen("longitude"),"longitude");
  error(rc);
  rc=nc_put_att_text(cdf->ncid, lon_var_id,"axis",strlen("X"),"X");
  error(rc);
  rc=nc_def_var(cdf->ncid,name,nctype[type],3,dim,&cdf->varid);
  error(rc);
#ifdef USE_NETCDF4
  if(config->compress)
  {
    rc=nc_def_var_deflate(cdf->ncid, cdf->varid, 0, 1, config->compress);
    error(rc);
  }
#endif
  if(units!=NULL)
  {
    rc=nc_put_att_text(cdf->ncid, cdf->varid,"units",strlen(units),units);
    error(rc);
  }
  if(descr!=NULL)
  {
    rc=nc_put_att_text(cdf->ncid, cdf->varid,"long_name",strlen(descr),descr);
    error(rc);
  }
  switch(type)
  { 
    case LPJ_FLOAT:
      nc_put_att_float(cdf->ncid, cdf->varid, "missing_value", NC_FLOAT,1,&miss);
      rc=nc_put_att_float(cdf->ncid, cdf->varid, "_FillValue", NC_FLOAT,1,&miss);
      break;
    case LPJ_SHORT:
      nc_put_att_short(cdf->ncid, cdf->varid, "missing_value", NC_SHORT,1,&smiss);
      rc=nc_put_att_short(cdf->ncid, cdf->varid, "_FillValue", NC_SHORT,1,&smiss);
      break;
    case LPJ_INT:
      nc_put_att_int(cdf->ncid, cdf->varid, "missing_value", NC_INT,1,&imiss);
      rc=nc_put_att_int(cdf->ncid, cdf->varid, "_FillValue", NC_INT,1,&imiss);
      break;
  }
  error(rc);
  rc=nc_put_att_text(cdf->ncid,NC_GLOBAL,"title",
                     strlen(config->sim_name),config->sim_name);
  error(rc);
  snprintf(s,STRING_LEN,"LPJmL C Version " LPJ_VERSION "-%04d",getsvnrev());
  rc=nc_put_att_text(cdf->ncid,NC_GLOBAL,"source",strlen(s),s);
  error(rc);
  time(&t);
  snprintf(s,STRING_LEN,"Created for user %s on %s at %s",getuser(),gethost(),
           ctime(&t));
  s[strlen(s)-1]='\0';
  rc=nc_put_att_text(cdf->ncid,NC_GLOBAL,"history",strlen(s),s);
  error(rc);
  rc=nc_enddef(cdf->ncid);
  error(rc);
  if(index==SOILC_LAYER)
  {
    rc=nc_put_var_float(cdf->ncid,pft_var_id,layer);
    error(rc);
  }
  else
  {
#ifdef USE_NETCDF4
    rc=nc_put_var_string(cdf->ncid,pft_var_id,(const char **)pftnames);
    error(rc);
#else
    count[0]=1;
    offset[1]=0;
    for(i=0;i<size;i++)
    {
      offset[0]=i;
      count[1]=strlen(pftnames[i])+1;
      rc=nc_put_vara_text(cdf->ncid,pft_var_id,offset,count,pftnames[i]);
      error(rc);
    }
#endif
    freepftnames(pftnames,index,npft,config->nbiomass,config->nagtree,ncft);
  }
  rc=nc_put_var_float(cdf->ncid,lat_var_id,lat);
  error(rc);
  rc=nc_put_var_float(cdf->ncid,lon_var_id,lon);
  error(rc);
  free(layer);
  free(lat);
  free(lon);
  return FALSE;
#else
  fputs("ERROR401: NetCDF output is not supported by this version of LPJmL.\n",stderr);
  return TRUE;
#endif
} /* of 'create1_pft_netcdf' */
