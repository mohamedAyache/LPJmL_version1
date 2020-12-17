/***************************************************************************/
/**                                                                       **/
/**       o  p  e  n  c  l  i  m  a  t  e  _  n  e  t  c  d  f  .  c      **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function opens climate data file in NetCDF format                 **/
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
#define error(var,rc) if(rc) {if(isroot(*config))fprintf(stderr,"ERROR403: Cannot read '%s' in '%s': %s.\n",var,filename,nc_strerror(rc)); free_netcdf(file->ncid); return TRUE;}
#endif

Bool openclimate_netcdf(Climatefile *file,    /* climate data file */
                        const char *filename, /* filename */
                        const char *var,      /* variable name or NULL */
                        const char *units,    /* units or NULL */
                        const Config *config  /* LPJ configuration */
                       )                      /* returns TRUE on error */
{
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  char *s,*c,*unit,*name;
  int rc,var_id,time_id,ndims;
  int *time;
  double *date;
  size_t len,time_len;
  Bool isopen;
  if(filename==NULL || file==NULL)
    return TRUE;
  rc=open_netcdf(filename,&file->ncid,&isopen);
  if(rc)
  {
    fprintf(stderr,"ERROR409: Cannot open '%s': %s.\n",
            filename,nc_strerror(rc));
    return TRUE;
  }
  rc=nc_inq_varid(file->ncid,TIME_NAME,&var_id);
  error("time",rc);
  rc=nc_inq_varndims(file->ncid,var_id,&ndims);
  error("time dim",rc);
  if(ndims!=1)
  {
    fprintf(stderr,"ERROR408: Invalid number of dimensions for time in '%s'.\n",filename);
    free_netcdf(file->ncid);
    return TRUE;
  }
  nc_inq_vardimid(file->ncid,var_id,&time_id);
  nc_inq_dimlen(file->ncid,time_id,&time_len);
  if(!nc_inq_attlen(file->ncid, var_id, "units", &len))
  {
    s=malloc(len+1);
    if(s==NULL)
    {
      printallocerr("s");
      free_netcdf(file->ncid);
      return TRUE;
    }
    nc_get_att_text(file->ncid, var_id, "units",s);
    s[len]='\0';
    if(!strcmp("day as %Y%m%d.%f",s))
    {
      file->isdaily=TRUE;
      date=newvec(double,time_len);
      if(date==NULL)
      {
        printallocerr("date");
        free_netcdf(file->ncid);
        free(s);
        return TRUE;
      }
      rc=nc_get_var_double(file->ncid,var_id,date);
      if(rc)
      {
        fprintf(stderr,"ERROR417: Cannot read time in '%s': %s.\n",
                filename,nc_strerror(rc));
        free(date);
        free_netcdf(file->ncid);
        free(s);
        return TRUE;
      }
      file->firstyear=(int)(date[0]/10000);
      free(date); 
    }
    else
    {
      name=malloc(len+1);
      if(name==NULL)
      {
        printallocerr("name");
        free(s);
        free_netcdf(file->ncid);
        return TRUE;
      }
      if(sscanf(s,"%s since %d",name,&file->firstyear)!=2)
      {
        fputs("ERROR416: No start year in units.\n",stderr);
        free(s);
        free(name);
        free_netcdf(file->ncid);
        return TRUE;
      }
      time=newvec(int,time_len);
      if(time==NULL)
      {
        printallocerr("time");
        free_netcdf(file->ncid);
        free(name);
        free(s);
        return TRUE;
      }
      rc=nc_get_var_int(file->ncid,var_id,time);
      if(rc)
      {
        fprintf(stderr,"ERROR417: Cannot read time in '%s': %s.\n",
                filename,nc_strerror(rc));
        free(time);
        free(name);
        free_netcdf(file->ncid);
        free(s);
        return TRUE;
      }
      if(!strcmp(name,"days"))
      {
        file->isdaily=(time[1]-time[0]==1);
        file->firstyear+=time[0]/NDAYYEAR;
      }
      else if(!strcmp(name,"months"))
      {
        file->isdaily=FALSE;
        file->firstyear+=time[0]/12;
      }
      else if(!strcmp(name,"hours"))
      {
        file->isdaily=(time[1]-time[0]==24);
        file->firstyear+=time[0]/NDAYYEAR/24;
      }
      else
      {
        fprintf(stderr,"ERROR432: Invalid time unit '%s' in '%s'.\n",
                name,filename);
        free(name);
        free(time);
        free_netcdf(file->ncid);
        free(s);
        return TRUE;
      }
      free(name);
      free(time);
      if(!nc_inq_attlen(file->ncid, var_id,"calendar",&len))
      {
        c=malloc(len+1);
        if(c==NULL)
        {
          printallocerr("c");
          free_netcdf(file->ncid);
          free(s);
          return TRUE;
        }
        nc_get_att_text(file->ncid, var_id,"calendar",c);
        c[len]='\0';
        file->isleap=strcmp(c,"noleap") && strcmp(c,"365_day");
        free(c);
      }
      else
        file->isleap=TRUE;
    }
    free(s);
  }
  else
  {
    rc=nc_inq_attlen(file->ncid, var_id, "time_format", &len);
    error("time_format",rc);
    s=malloc(len+1);
    if(s==NULL)
    {
      printallocerr("s");
      free_netcdf(file->ncid);
      return TRUE;
    }
    rc=nc_get_att_text(file->ncid, var_id, "time_format",s);
    if(rc)
    {
      free(s);
      fprintf(stderr,"ERROR418: Cannot read time format in '%s': %s.\n",
              filename,nc_strerror(rc));
      free_netcdf(file->ncid);
      return TRUE;
    }
    s[len]='\0';
    unit=malloc(len+1);
    if(unit==NULL)
    {
      printallocerr("unit");
      free_netcdf(file->ncid);
      free(s);
      return TRUE;
    }
    if(sscanf(s,"%s from %*s %d",unit,&file->firstyear)!=2)
    {
      free(s);
      free(unit);
      fprintf(stderr,"ERROR419: Cannot detect first year in '%s'.\n",filename);
      free_netcdf(file->ncid);
      return TRUE;
    }
    free(s);
    if(!strcmp(unit,"months"))
    {
      file->n=config->ngridcell*NMONTH;
      file->isdaily=FALSE;
    }
    else if(!strcmp(unit,"days"))
    {
      file->n=config->ngridcell*NDAYYEAR;
      file->isdaily=TRUE;
    }
    else
    {
      free(unit);
      fprintf(stderr,"ERROR420: Cannot detect unit in '%s'.\n",filename);
      free_netcdf(file->ncid);
      return TRUE;
    }
    free(unit);
  }
  if(file->isdaily)
  { 
    file->nyear=time_len/NDAYYEAR;
    file->n=config->ngridcell*NDAYYEAR;
  }
  else
  {
    file->nyear=time_len/NMONTH;
    file->n=config->ngridcell*NMONTH;
  }
  if(getvar_netcdf(file,filename,var,units,config))
  {
    free_netcdf(file->ncid);
    return TRUE;
  }
  if(getlatlon_netcdf(file,filename,config))
  {
    free_netcdf(file->ncid);
    return TRUE;
  }
  nc_inq_varndims(file->ncid,file->varid,&ndims);
  if(ndims!=3)
  {
    fprintf(stderr,"ERROR408: Invalid number of dimensions %d in '%s'.\n",
            ndims,filename);
    free_netcdf(file->ncid);
    return TRUE;
  }
  return FALSE;
#else
  fputs("ERROR401: NetCDF input is not supported by this version of LPJmL.\n",stderr);
  return TRUE;
#endif
} /* of 'openclimate_netcdf' */
