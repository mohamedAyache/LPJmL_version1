/***************************************************************************/
/**                                                                       **/
/**                     c  d  f  2  c  l  m  .  c                         **/
/**                                                                       **/
/**     Program converts NetCDF input data into CLM format                **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-09-29 16:02:37 +0200 (lun., 29 sept. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#ifdef USE_UDUNITS
#define USAGE "Usage: %s [-units unit] [-var name] [-o filename] [-scale factor] gridfile netcdffile\n"
#else
#define USAGE "Usage: %s [-var name] [-o filename] [-scale factor] gridfile netcdffile\n"
#endif

#if defined(USE_NETCDF) || defined(USE_NETCDF4)
#include <netcdf.h>

static Bool readclimate2(Climatefile *file,    /* climate data file */
                         float data[],         /* pointer to data read in */
                         int year,             /* year */
                         const Coord coords[], /* coordinates */
                         const Config *config  /* LPJ configuration */
                        )                      /* returns TRUE on error */
{
  int i,cell,rc;
  float *f;
  int size;
  size_t offsets[3];
  size_t counts[3];
  size=(file->isdaily) ? NDAYYEAR : NMONTH;
  offsets[1]=offsets[2]=0;
  offsets[0]=year*size;
  if(file->isdaily && file->isleap)
    offsets[0]+=nleapyears(file->firstyear,year+file->firstyear);
  counts[0]=size;
  counts[1]=file->nlat;
  counts[2]=file->nlon;
  f=newvec(float,size*file->nlon*file->nlat);
  if(f==NULL)
  {
    printallocerr("data");
    nc_close(file->ncid);
    return TRUE;
  }
  if((rc=nc_get_vara_float(file->ncid,file->varid,offsets,counts,f)))
  {
    free(f);
    fprintf(stderr,"ERROR421: Cannot read float data: %s.\n",
           nc_strerror(rc)); 
    nc_close(file->ncid);
    return TRUE;
  }
  for(cell=0;cell<config->ngridcell;cell++)
  {
    if(file->offset)
      offsets[1]=file->offset-(int)((coords[cell].lat-file->lat_min)/config->resolution.lat+0.5);
    else
      offsets[1]=(int)((coords[cell].lat-file->lat_min)/config->resolution.lat+0.5);
    offsets[2]=(int)((coords[cell].lon-file->lon_min)/config->resolution.lon+0.5);
    if(offsets[1]>=file->nlat || offsets[2]>=file->nlon)
    {
      fprintf(stderr,"ERROR422: Invalid coordinate for cell %d (",
              cell);
      fprintcoord(stderr,coords[cell]);
      fputs(") in data file.\n",stderr);
      free(f);
      nc_close(file->ncid);
      return TRUE;
    }
    for(i=0;i<size;i++)
    {
      if(f[file->nlon*(i*file->nlat+offsets[1])+offsets[2]]==file->missing_value.f)
      {
        fprintf(stderr,"ERROR423: Missing value for cell=%d (",cell);
        fprintcoord(stderr,coords[cell]);
        fprintf(stderr,") at %s %d.\n",(file->isdaily) ? "day" : "month",i+1);
        free(f);
        nc_close(file->ncid);
        return TRUE;
      }
      else if(isnan(f[file->nlon*(i*file->nlat+offsets[1])+offsets[2]]))
      {
        fprintf(stderr,"ERROR423: Invalid value for cell=%d (",cell);
        fprintcoord(stderr,coords[cell]);
        fprintf(stderr,") at %s %d.\n",(file->isdaily) ? "day" : "month",i+1);
        free(f);
        nc_close(file->ncid);
        return TRUE;
      }
      data[cell*size+i]=file->slope*f[file->nlon*(i*file->nlat+offsets[1])+offsets[2]]+file->intercept;
    }
  }
  free(f);
  return FALSE;
} /* of 'readclimate2' */

#endif

int main(int argc,char **argv)
{
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  Coordfile coordfile;
  Climatefile climate;
  Config config;
  char *units,*var,*outname,*endptr;
  float scale,*data;
  Filename coord_filename;
  Coord *coords;
  Header header;
  FILE *file;
  int i,j,k,year;
  short *s;
  units=NULL;
  var=NULL;
  scale=1;
  outname="out.clm"; /* default file name for output */
  initconfig(&config);
  for(i=1;i<argc;i++)
  {
    if(argv[i][0]=='-')
    {
      if(!strcmp(argv[i],"-var"))
      {
        if(argc==i-1)
        {
          fprintf(stderr,"Missing argument after option '-var'.\n"
                 USAGE,argv[0]);
          return EXIT_FAILURE;
        }
        var=argv[++i];
      }
#ifdef USE_UDUNITS
      else if(!strcmp(argv[i],"-units"))
      {
        if(argc==i-1)
        {
          fprintf(stderr,"Missing argument after option '-units'.\n"
                 USAGE,argv[0]);
          return EXIT_FAILURE;
        }
        units=argv[++i];
      }
#endif
      else if(!strcmp(argv[i],"-o"))
      {
        if(argc==i-1)
        {
          fprintf(stderr,"Missing argument after option '-o'.\n"
                 USAGE,argv[0]);
          return EXIT_FAILURE;
        }
        outname=argv[++i];
      }
      else if(!strcmp(argv[i],"-scale"))
      {
        if(argc==i-1)
        {
          fprintf(stderr,"Missing argument after option '-scale'.\n"
                 USAGE,argv[0]);
          return EXIT_FAILURE;
        }
        scale=(float)strtod(argv[++i],&endptr);
        if(*endptr!='\0')
        {
          fprintf(stderr,"Invalid number '%s' for option '-scale'.\n",argv[i]);
          return EXIT_FAILURE;
        }
        if(scale==0)
        {
          fputs("Scale factor must not be zero.\n",stderr);
          return EXIT_FAILURE;
        }
      }
      else
      {
        fprintf(stderr,"Invalid option '%s'.\n"
                USAGE,argv[i],argv[0]);
        return EXIT_FAILURE;
      }
    }
    else
      break;
  }
  if(argc<i+2)
  {
    fprintf(stderr,"Missing arguments.\n"
            USAGE,argv[0]);
    return EXIT_FAILURE;
  }
  coord_filename.name=argv[i];
  coord_filename.fmt=CLM;
  coordfile=opencoord(&coord_filename);
  if(coordfile==NULL)
  {
    fprintf(stderr,"Error opening grid file '%s'.\n",coord_filename.name);
    return EXIT_FAILURE;
  }
  coords=newvec(Coord,numcoord(coordfile));
  if(coords==NULL)
  {
    printallocerr("coords");
    return EXIT_FAILURE;
  }
  config.ngridcell=numcoord(coordfile);
  header.cellsize=getcellsizecoord(coordfile);
  config.resolution.lat=config.resolution.lon=header.cellsize;
  for(j=0;j<numcoord(coordfile);j++)
    readcoord(coordfile,coords+j,config.resolution); 
  closecoord(coordfile);
  header.nyear=0;
  header.ncell=config.ngridcell;
  header.scalar=scale;
  header.order=CELLYEAR;
  header.firstcell=0;
  file=fopen(outname,"wb");
  if(file==NULL)
  {
    fprintf(stderr,"Error creating '%s': %s.\n",outname,strerror(errno));
    return EXIT_FAILURE;
  }
  fwriteheader(file,header,LPJ_CLIMATE_HEADER,LPJ_CLIMATE_VERSION);
  for(j=i+1;j<argc;j++)
  {
    /* printf("%s\n",argv[j]); */
    if(openclimate_netcdf(&climate,argv[j],var,units,&config))
    {
      fprintf(stderr,"Error opening '%s'.\n",argv[j]);
      return EXIT_FAILURE;
    }
    if(j==i+1)
    {
      header.firstyear=climate.firstyear;
      header.nbands=climate.isdaily ? NDAYYEAR : NMONTH;
      data=newvec(float,config.ngridcell*header.nbands);
      if(data==NULL)
      {
        printallocerr("data");
        return EXIT_FAILURE;
      }
      s=newvec(short,config.ngridcell*header.nbands);
      if(s==NULL)
      {
        printallocerr("short");
        return EXIT_FAILURE;
      }
    }
    else
    {
      if(climate.isdaily && header.nbands!=NDAYYEAR)
      {
        fprintf(stderr,"ERROR: daily output in '%s'.\n",argv[j]);
        return EXIT_FAILURE;
      }
      else if (!climate.isdaily && header.nbands!=NMONTH)
      {
        fprintf(stderr,"ERROR: monthly output in '%s'.\n",argv[j]);
        return EXIT_FAILURE;
      }
      if(climate.firstyear!=header.firstyear+header.nyear)
      {
        fprintf(stderr,"First year %d in '%s' not last year %d.\n",climate.firstyear,argv[j],header.firstyear+header.nyear);
        return EXIT_FAILURE;
      }
    }
    for(year=0;year<climate.nyear;year++)
    {
      if(readclimate2(&climate,data,year,coords,&config))
      {
        fprintf(stderr,"Error reading '%s' in year %d.\n",argv[j],year+climate.firstyear);
        return EXIT_FAILURE;
      }
      for(k=0;k<config.ngridcell*header.nbands;k++)
      {
        if((int)(data[k]/scale)<SHRT_MIN || (int)(data[k]/scale)>SHRT_MAX)
        {
          fprintf(stderr,"WARNING: Data overflow for cell %d ",k/header.nbands);
          fprintcoord(stderr,coords[k/header.nbands]);
          fprintf(stderr,") at %s %d in %d.\n",(climate.isdaily) ? "day" : "month",(k % header.nbands)+1,climate.firstyear+year);
        } 
        s[k]=(short)(data[k]/scale); 
      }
      if(fwrite(s,sizeof(short),config.ngridcell*header.nbands,file)!=config.ngridcell*header.nbands)
      {
        fprintf(stderr,"Error writing data in '%s' in year %d.\n",outname,year+climate.firstyear);
        return EXIT_FAILURE;
      }
    } /* of for(year=0;...) */
    header.nyear+=climate.nyear;
    nc_close(climate.ncid);
  }
  rewind(file);
  fwriteheader(file,header,LPJ_CLIMATE_HEADER,LPJ_CLIMATE_VERSION);
  fclose(file);
  return EXIT_SUCCESS;
#else
  fprintf(stderr,"ERROR401: NetCDF is not supported in this version of %s.\n",argv[0]);
  return EXIT_FAILURE;
#endif
} /* of 'main' */
