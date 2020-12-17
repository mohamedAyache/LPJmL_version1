/***************************************************************************/
/**                                                                       **/
/**                c  l  m  2  c  d  f  .  c                              **/
/**                                                                       **/
/**     Converts CLM data file into NetCDF data file                      **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
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

#define error(rc) if(rc) {free(lon);free(lat);free(year);fprintf(stderr,"ERROR427: Cannot write '%s': %s.\n",filename,nc_strerror(rc)); nc_close(cdf->ncid); free(cdf);return NULL;}

#define MISSING_VALUE -9999.99
#define USAGE "Usage: %s [-longheader] [-scale s] [-cellsize size] [-int] [-landuse]\n       [-compress level] [-units u] [-descr d] name gridfile clmfile netcdffile\n"

typedef struct
{
  const Coord_array *index;
  int ncid;
  int varid;
} Cdf;

static Cdf *create_cdf(const char *filename,
                       const char *clm_filename,
                       const char *name,
                       const char *units,
                       const char *descr,
                       Header header,
                       int compress,
                       Bool landuse,
                       const Coord_array *array)
{
  Cdf *cdf;
  float *lon,*lat,miss=MISSING_VALUE;
  int *year,i,j,rc,dim[4];
  String s;
  time_t t;
  int time_var_id,lat_var_id,lon_var_id,time_dim_id,lat_dim_id,lon_dim_id;
  int landuse_dim_id;
  cdf=new(Cdf);
  lon=newvec(float,array->nlon);
  if(lon==NULL)
  {
    printallocerr("lon");
    free(cdf);
    return NULL;
  }
  lat=newvec(float,array->nlat);
  if(lat==NULL)
  {
    printallocerr("lat");
    free(lon);
    free(cdf);
    return NULL;
  }
  lon[0]=(float)array->lon_min;
  for(i=1;i<array->nlon;i++)
    lon[i]=lon[i-1]+(float)header.cellsize;
  lat[0]=(float)array->lat_min;
  for(i=1;i<array->nlat;i++)
    lat[i]=lat[i-1]+(float)header.cellsize;
  year=newvec(int,(landuse) ?  header.nyear : header.nyear*header.nbands);
  if(year==NULL)
  {
    printallocerr("year");
    free(lon);
    free(lat);
    free(cdf);
    return NULL;
  }
  if(landuse)
    for(i=0;i<header.nyear;i++)
      year[i]=header.firstyear+i;
  else switch(header.nbands)
  {
    case 1:
      for(i=0;i<header.nyear;i++)
        year[i]=header.firstyear+i;
      break;
    case 12:
      for(i=0;i<header.nyear;i++)
        for(j=0;j<12;j++)
          if(i==0 && j==0)
            year[0]=ndaymonth[j]-1;
          else
            year[i*12+j]=year[i*12+j-1]+ndaymonth[j];
      break;
    case NDAYYEAR:
      for(i=0;i<header.nyear*NDAYYEAR;i++)
        year[i]=i;
      break;
    default:
      fputs("ERROR425: Invalid value for number of data points per year.\n",
            stderr);
      free(year);
      free(lon);
      free(lat);
      free(cdf);
      return NULL;
  }
  cdf->index=array;
#ifdef USE_NETCDF4
  rc=nc_create(filename,(compress) ? NC_CLOBBER|NC_NETCDF4 : NC_CLOBBER,&cdf->ncid);
#else
  rc=nc_create(filename,NC_CLOBBER,&cdf->ncid);
#endif
  if(rc)
  {
    fprintf(stderr,"ERROR426: Cannot create file '%s': %s.\n",
            filename,nc_strerror(rc));
    free(year);
    free(lon);
    free(lat);
    free(cdf);
    return NULL;
  }
  rc=nc_def_dim(cdf->ncid,TIME_DIM_NAME,(landuse) ? header.nyear : header.nyear*header.nbands,&time_dim_id);
  error(rc);
  rc=nc_def_var(cdf->ncid,"time",NC_INT,1,&time_dim_id,&time_var_id);
  error(rc);
  rc=nc_def_dim(cdf->ncid,LAT_DIM_NAME,array->nlat,&lat_dim_id);
  error(rc);
  rc=nc_def_dim(cdf->ncid,LON_DIM_NAME,array->nlon,&lon_dim_id);
  error(rc);
  snprintf(s,STRING_LEN,"clm2cdf %s",clm_filename);
  rc=nc_put_att_text(cdf->ncid,NC_GLOBAL,"source",strlen(s),s);
  error(rc);
  time(&t);
  snprintf(s,STRING_LEN,"Created for user %s on %s at %s",getuser(),gethost(),
          ctime(&t));
  s[strlen(s)-1]='\0';
  rc=nc_put_att_text(cdf->ncid,NC_GLOBAL,"history",strlen(s),s);
  error(rc);
  rc=nc_def_var(cdf->ncid,LAT_NAME,NC_FLOAT,1,&lat_dim_id,&lat_var_id);
  error(rc);
  rc=nc_def_var(cdf->ncid,LON_NAME,NC_FLOAT,1,&lon_dim_id,&lon_var_id);
  error(rc);
  if(landuse || header.nbands==1)
    rc=nc_put_att_text(cdf->ncid,time_var_id,"units",strlen("years"),"years");
  else if(header.nbands>1)
  {
    snprintf(s,STRING_LEN,"days since %d-1-1 0:0:0",header.firstyear);
    rc=nc_put_att_text(cdf->ncid,time_var_id,"units",strlen(s),s);
    error(rc);
    rc=nc_put_att_text(cdf->ncid,time_var_id,"calendar",strlen("noleap"),
                      "noleap");
  }
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
  rc=nc_put_att_text(cdf->ncid,lat_var_id,"units",strlen("degrees_north"),
                   "degrees_north");
  error(rc);
  rc=nc_put_att_text(cdf->ncid, lat_var_id,"long_name",strlen("latitude"),"latitude");
  error(rc);
  rc=nc_put_att_text(cdf->ncid, lat_var_id,"standard_name",strlen("latitude"),"latitude");
  error(rc);
  rc=nc_put_att_text(cdf->ncid, lat_var_id,"axis",strlen("Y"),"Y");
  error(rc);
  if(landuse)
  {
    rc=nc_def_dim(cdf->ncid,name,header.nbands,&landuse_dim_id);
    error(rc);
    dim[0]=time_dim_id;
    dim[1]=landuse_dim_id;
    dim[2]=lat_dim_id;
    dim[3]=lon_dim_id;
  }
  else
  {
    dim[0]=time_dim_id;
    dim[1]=lat_dim_id;
    dim[2]=lon_dim_id;
  }
  rc=nc_def_var(cdf->ncid,name,NC_FLOAT,(landuse) ? 4 : 3,dim,&cdf->varid);
  error(rc);
#ifdef USE_NETCDF4
  if(compress)
  {
    rc=nc_def_var_deflate(cdf->ncid, cdf->varid, 0, 1,compress);
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
  nc_put_att_float(cdf->ncid, cdf->varid,"missing_value",NC_FLOAT,1,&miss);
  rc=nc_put_att_float(cdf->ncid, cdf->varid,"_FillValue",NC_FLOAT,1,&miss);
  rc=nc_enddef(cdf->ncid);
  error(rc);
  rc=nc_put_var_int(cdf->ncid,time_var_id,year);
  error(rc);
  rc=nc_put_var_float(cdf->ncid,lat_var_id,lat);
  error(rc);
  rc=nc_put_var_float(cdf->ncid,lon_var_id,lon);
  error(rc);
  free(lat);
  free(lon);
  free(year);
  return cdf;
} /* of 'create_cdf' */

static Bool write_float_cdf(const Cdf *cdf,const float vec[],int year,
                            int size,Bool landuse,int nband)
{
  int i,rc;
  size_t offsets[4],counts[4];
  float *grid;
  grid=newvec(float,cdf->index->nlon*cdf->index->nlat);
  if(grid==NULL)
  {
    printallocerr("grid");
    return TRUE;
  }
  for(i=0;i<cdf->index->nlon*cdf->index->nlat;i++)
    grid[i]=MISSING_VALUE;
  for(i=0;i<size;i++)
    grid[cdf->index->index[i]]=vec[i];
  if(year==NO_TIME)
    rc=nc_put_var_float(cdf->ncid,cdf->varid,grid);
  else
  {
    offsets[0]=year;
    counts[0]=1;
    if(landuse)
    {
      counts[1]=1;
      counts[2]=cdf->index->nlat;
      counts[3]=cdf->index->nlon;
      offsets[1]=nband;
      offsets[2]=offsets[3]=0;
    }
    else
    {
      counts[1]=cdf->index->nlat;
      counts[2]=cdf->index->nlon;
      offsets[1]=offsets[2]=0;
    }
    rc=nc_put_vara_float(cdf->ncid,cdf->varid,offsets,counts,grid);
  }
  free(grid);
  if(rc!=NC_NOERR)
  {
    fprintf(stderr,"ERROR431: Cannot write output data: %s.\n",
            nc_strerror(rc));
    return TRUE;
  }
  return FALSE;
} /* of 'write_float_cdf' */

static void close_cdf(Cdf *cdf)
{
  nc_close(cdf->ncid);
  free(cdf);
} /* of 'close_cdf' */

#endif

int main(int argc,char **argv)
{
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  FILE *file;
  Coordfile coordfile;
  Coord_array *index;
  Coord *grid,res;
  Cdf *cdf;
  Header header;
  String headername;
  short *data;
  int *idata;
  int i,j,k,ngrid,version,iarg,compress;
  Bool swap,isint,landuse,longheader;
  float *f,scale,cellsize;
  char *units,*descr,*endptr;
  Filename filename;
  units=descr=NULL;
  scale=1.0;
  compress=0;
  cellsize=0;
  isint=longheader=FALSE;
  landuse=FALSE;
  for(iarg=1;iarg<argc;iarg++)
    if(argv[iarg][0]=='-')
    {
      if(!strcmp(argv[iarg],"-units"))
      {
        if(argc==iarg-1)
        {
          fprintf(stderr,"Missing argument after option '-units'.\n"
                 USAGE,argv[0]);
          return EXIT_FAILURE;
        }
        units=argv[++iarg];
      }
      else if(!strcmp(argv[iarg],"-longheader"))
        longheader=TRUE;
      else if(!strcmp(argv[iarg],"-int"))
        isint=TRUE;
      else if(!strcmp(argv[iarg],"-landuse"))
        landuse=TRUE;
      else if(!strcmp(argv[iarg],"-descr"))
      {
        if(argc==iarg-1)
        {
          fprintf(stderr,"Missing argument after option '-descr'.\n"
                 USAGE,argv[0]);
          return EXIT_FAILURE;
        }
        descr=argv[++iarg];
      }
      else if(!strcmp(argv[iarg],"-scale"))
      {
        if(argc==iarg-1)
        {
          fprintf(stderr,"Missing argument after option '-scale'.\n"
                  USAGE,argv[0]);
          return EXIT_FAILURE;
        }
        scale=(float)strtod(argv[++iarg],&endptr);
        if(*endptr!='\0')
        {
          fprintf(stderr,"Invalid number '%s' for option '-scale'.\n",argv[iarg]);
          return EXIT_FAILURE;
        }
      }
      else if(!strcmp(argv[iarg],"-cellsize"))
      {
        if(argc==iarg-1)
        {
          fprintf(stderr,"Missing argument after option '-cellsize'.\n"
                  USAGE,argv[0]);
          return EXIT_FAILURE;
        }
        cellsize=(float)strtod(argv[++iarg],&endptr);
        if(*endptr!='\0')
        {
          fprintf(stderr,"Invalid number '%s' for option '-cellsize'.\n",argv[iarg]);
          return EXIT_FAILURE;
        }
      }
 
      else if(!strcmp(argv[iarg],"-compress"))
      {
        if(argc==iarg-1)
        {
          fprintf(stderr,"Missing argument after option '-compress'.\n"
                  USAGE,argv[0]);
          return EXIT_FAILURE;
        }
        compress=strtol(argv[++iarg],&endptr,10);
        if(*endptr!='\0')
        {
          fprintf(stderr,"Invalid number '%s' for option '-compress'.\n",argv[iarg]);
          return EXIT_FAILURE;
        }
      }
      else
      {
        fprintf(stderr,"invalid option '%s'.\n"
                USAGE,argv[iarg],argv[0]);
        return EXIT_FAILURE;
      }
    }
    else
      break;
  if(argc<iarg+4)
  {
    fprintf(stderr,"Missing arguments.\n"
            USAGE,argv[0]);
    return EXIT_FAILURE;
  }
  filename.fmt=CLM;
  filename.name=argv[iarg+1];
  coordfile=opencoord(&filename);
  if(coordfile==NULL)
  {
    fprintf(stderr,"Error opening grid file '%s'.\n",filename.name);
    return EXIT_FAILURE;
  }
  ngrid=numcoord(coordfile);
  if(cellsize>0)
    res.lon=res.lat=cellsize;
  else
    res.lon=res.lat=getcellsizecoord(coordfile);
  grid=newvec(Coord,numcoord(coordfile));
  if(grid==NULL)
  {
    printallocerr("grid");
    return EXIT_FAILURE;
  }
  for(i=0;i<numcoord(coordfile);i++)
    readcoord(coordfile,grid+i,res);
  closecoord(coordfile);
  file=fopen(argv[iarg+2],"rb");
  if(file==NULL)
  {
    fprintf(stderr,"Error opening '%s': %s.\n",argv[iarg+2],strerror(errno));
    return EXIT_FAILURE;
  }
  version=(longheader) ? 2 : READ_VERSION;
  if(freadanyheader(file,&header,&swap,headername,&version))
  {
    fprintf(stderr,"Error reading header of '%s'.\n",argv[iarg+2]);
    fclose(file);
    return EXIT_FAILURE;
  }
  if(version==1)
    header.scalar=scale;
  if(header.ncell!=ngrid)
  {
    fprintf(stderr,"Number of cells in '%s' is different from %d in '%s'.\n",
            argv[iarg+2],ngrid,argv[iarg+1]);
    fclose(file);
    return EXIT_FAILURE;
  }
  if(version==1 && cellsize>0)
    header.cellsize=cellsize;
  else if(version>1 && header.cellsize!=res.lon)
  {
    fprintf(stderr,"Cell size in '%s' differs from '%s'.\n",
            argv[iarg+2],argv[iarg+1]);
    fclose(file);
    return EXIT_FAILURE;
  }
  index=createindex(grid,ngrid,res);
  if(index==NULL)
    return EXIT_FAILURE;
  free(grid);
  cdf=create_cdf(argv[iarg+3],argv[iarg+2],argv[iarg],units,descr,header,compress,landuse,index);
  if(cdf==NULL)
    return EXIT_FAILURE;
  if(isint)
  {
    idata=newvec(int,ngrid*header.nbands);
    if(idata==NULL)
    {
      printallocerr("data");
      return EXIT_FAILURE;
    }
  }
  else
  {
    data=newvec(short,ngrid*header.nbands);
    if(data==NULL)
    {
      printallocerr("data");
      return EXIT_FAILURE;
    }
  }
  f=newvec(float,ngrid);
  if(f==NULL)
  {
    printallocerr("f");
    return EXIT_FAILURE;
  }
  for(i=0;i<header.nyear;i++)
  {
    if(isint)
    {
      if(freadint(idata,ngrid*header.nbands,swap,file)!=ngrid*header.nbands)
      {
        fprintf(stderr,"Error reading data in year %d.\n",i+header.firstyear);
        return EXIT_FAILURE;
      }
      for(j=0;j<header.nbands;j++)
      {
        for(k=0;k<ngrid;k++)
          f[k]=idata[k*header.nbands+j]*header.scalar;
        if(write_float_cdf(cdf,f,(landuse) ? i : i*header.nbands+j,ngrid,landuse,j))
          return EXIT_FAILURE;
      }
    }
    else
    {
      if(freadshort(data,ngrid*header.nbands,swap,file)!=ngrid*header.nbands)
      {
        fprintf(stderr,"Error reading data in year %d.\n",i+header.firstyear);
        return EXIT_FAILURE;
      }
      for(j=0;j<header.nbands;j++)
      {
        for(k=0;k<ngrid;k++)
          f[k]=data[k*header.nbands+j]*header.scalar;
        if(write_float_cdf(cdf,f,(landuse) ? i : i*header.nbands+j,ngrid,landuse,j))
          return EXIT_FAILURE;
      }
    }
  }
  close_cdf(cdf);
  fclose(file);
  if(isint)
    free(idata);
  else
    free(data);

  return EXIT_SUCCESS;
#else
  fprintf(stderr,"ERROR401: NetCDF is not supported in this version of %s.\n",argv[0]);
  return EXIT_FAILURE;
#endif
} /* of 'main' */
