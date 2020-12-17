/***************************************************************************/
/**                                                                       **/
/**                   f  i  l  e  s  e  x  i  s  t  .  c                  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Functions checks whether input files exist and have the correct   **/
/**     format                                                            **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-11-06 11:17:06 +0100 (ven., 06 nov. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include <sys/stat.h>
#include "lpj.h"

#ifndef S_ISDIR /* macro is not defined on Windows */
#define S_ISDIR(a) (a & _S_IFDIR)
#endif

static int checkfile(const char *filename)
{
  if(getfilesize(filename)==-1)
  {
    fprintf(stderr,"Error opening '%s': %s.\n",filename,strerror(errno));
    return 1;
  }
  else
    return 0;
} /* of 'checkfile' */

static int checkinputfile(const Config *config,const Filename *filename)
{
  FILE *file;
  Header header;
  String headername;
  int version;
  Bool swap;
  Input_netcdf input;
  if(filename->fmt==CDF)
  {
    input=openinput_netcdf(filename->name,filename->var,NULL,-1,config);
    if(input==NULL)
      return 1;
    closeinput_netcdf(input);
  }
  else
  {
    file=openinputfile(&header,&swap,filename,headername,&version,config);
    if(file==NULL)
      return 1;
    fclose(file);
  }
  return 0;
}
static int checkdatafile(const Config *config,const Filename *filename)
{
  FILE *file;
  Header header;
  String headername;
  int version;
  Bool swap;
  Climatefile input;
  if(filename->fmt==CDF)
  {
    if(openfile_netcdf(&input,filename->name,filename->var,NULL,config))
       return 1;
    closeclimate_netcdf(&input,TRUE);
  }
  else
  {
    file=openinputfile(&header,&swap,filename,headername,&version,config);
    if(file==NULL)
      return 1;
    fclose(file);
  }
  return 0;
} /* of 'checkdatafile' */

static int checkclmfile(const Config *config,const Filename *filename)
{
  FILE *file;
  Header header;
  String headername;
  int version;
  Bool swap;
  char *s,*name;
  Climatefile input;
  int first,last,year,count;
  if(filename->fmt==FMS)
    return 0;
  if(filename->fmt==CDF)
  {
    input.oneyear=FALSE;
    s=strstr(filename->name,"[");
    if(s!=NULL && sscanf(s,"[%d-%d]",&first,&last)==2)
    {
      s=mkfilename(filename->name);
      name=malloc(strlen(s)+8);
      count=0;
      for(year=first;year<=last;year++)
      {
        sprintf(name,s,year);
        if(openclimate_netcdf(&input,name,filename->var,NULL,config))
        {
          count++;
        }
        else
          closeclimate_netcdf(&input,TRUE);
      }
      free(s);
      free(name);
      return count;
    }
    else
    {
      if(openclimate_netcdf(&input,filename->name,filename->var,NULL,config))
        return 1;
      closeclimate_netcdf(&input,TRUE);
    }
  }
  else
  {
    file=openinputfile(&header,&swap,filename,headername,&version,config);
    if(file==NULL)
      return 1;
    fclose(file);
  }
  return 0;
} /* of 'checkclmfile' */

static int checkrestartfile(Config *config, const char *filename)
{
  FILE *file;
  Bool swap;
  file=openrestart(filename,config,
                   config->npft[GRASS]+config->npft[TREE]+config->npft[CROP],
                   &swap);
  if(file==NULL)
    return 1;
  fclose(file);
  return 0;
} /* of 'checkrestartfile' */

static int checksoilfile(Config *config,const Filename *filename)
{
  int size;
  size=getnsoilcode(filename,config->nsoil,TRUE);
  if(size==-1)
    return 1;
  if(size<config->nall+config->firstgrid)
  {
    fprintf(stderr,"File '%s' too short.\n",filename->name);
    return 1;
  }
  return 0;
} /* of 'checksoilfile' */

static int checkbinfile(Config *config,const char *filename)
{
  int size;
  size=getfilesize(filename);
  if(size==-1)
  {
    fprintf(stderr,"Error opening '%s': %s.\n",filename,strerror(errno));
    return 1;
  }
  if(size<config->nall+config->firstgrid)
  {
    fprintf(stderr,"File '%s' is too short.\n",filename);
    return 1;
  }
  return 0;
} /* of 'checkbinfile' */

static int checkdir(const char *path)
{
  struct stat filestat;
  if(stat(path,&filestat))
  {
    fprintf(stderr,"Directory '%s' cannot be openend: %s.\n",path,
            strerror(errno));
    return 1;
  }
  else if(!S_ISDIR(filestat.st_mode))
  {
    fprintf(stderr,"'%s' is not a directory.\n",path);
    return 1;
  }
  else
    return 0;
} /* of 'checkdir' */
static int checkcoordfile(Config *config,const Filename *filename)
{
  Coord_netcdf coord_netcdf;
  coord_netcdf=opencoord_netcdf(filename->name,filename->var,TRUE);
  if(coord_netcdf==NULL)
    return 1;
  getresolution_netcdf(coord_netcdf,&config->resolution);
  closecoord_netcdf(coord_netcdf);
  return 0;
}
Bool filesexist(Config config, /* LPJ configuration */
                int npft,      /* number of natural PFTs */
                int ncft,      /* number of crop PFTs */
                Bool isout     /* write output on stdout (TRUE/FALSE) */
               )               /* returns TRUE on error */
{
  int i,bad,badout;
  long long size;
  char *path,*oldpath;
  bad=0;
  if(config.soil_filename.fmt!=CDF)
  {
    bad+=checkclmfile(&config,&config.coord_filename);
    bad+=checksoilfile(&config,&config.soil_filename);
  }
  else
    bad+=checkcoordfile(&config,&config.soil_filename);
  bad+=checkinputfile(&config,&config.drainclass_filename);
  if(config.river_routing)
  {
    bad+=checkinputfile(&config,&config.drainage_filename);
    bad+=checkinputfile(&config,&config.lakes_filename);
    if(config.withlanduse!=NO_LANDUSE)
      bad+=checkinputfile(&config,&config.neighb_irrig_filename);
  }
  if(config.ispopulation)
    bad+=checkdatafile(&config,&config.popdens_filename);
#ifdef NEW_GRASS
  if(config.grassfix_filename.name!=NULL)
    bad+=checkinputfile(&config,&config.grassfix_filename);
  if(config.grassharvest_filename.name!=NULL)
    bad+=checkinputfile(&config,&config.grassharvest_filename);
#endif
  if(config.fire==SPITFIRE)
  {
    bad+=checkclmfile(&config,&config.wind_filename);
    bad+=checkclmfile(&config,&config.tamp_filename);
    if(config.tamp_filename.fmt==CDF && config.tmax_filename.name!=NULL)
      bad+=checkclmfile(&config,&config.tmax_filename);
    bad+=checkclmfile(&config,&config.lightning_filename);
    bad+=checkinputfile(&config,&config.human_ignition_filename);
  }
  if(config.wateruse_filename.name!=NULL)
    bad+=checkdatafile(&config,&config.wateruse_filename);
  bad+=checkclmfile(&config,&config.temp_filename);
  bad+=checkclmfile(&config,&config.prec_filename);
  if(config.with_radiation)
  {
    if(config.with_radiation==RADIATION || config.with_radiation==RADIATION_LWDOWN)
      bad+=checkclmfile(&config,&config.lwnet_filename);
    bad+=checkclmfile(&config,&config.swdown_filename);
  }
  else
    bad+=checkclmfile(&config,&config.cloud_filename);
  if(config.co2_filename.fmt!=FMS)
    bad+=checkfile(config.co2_filename.name);
  if(config.wet_filename.name!=NULL)
    bad+=checkclmfile(&config,&config.wet_filename);
  if(config.restart_filename!=NULL)
    bad+=checkrestartfile(&config,config.restart_filename);
  if(config.withlanduse!=NO_LANDUSE)
  {
    if(config.withlanduse!=ALL_CROPS)
      bad+=checkdatafile(&config,&config.landuse_filename);
    if(config.sdate_option==PRESCRIBED_SDATE)
      bad+=checkclmfile(&config,&config.sdate_filename);
    bad+=checkinputfile(&config,&config.countrycode_filename);
    if(config.reservoir)
    {
      bad+=checkinputfile(&config,&config.elevation_filename);
      bad+=checkinputfile(&config,&config.reservoir_filename);
    }
  }
  badout=0;
  oldpath=strdup("");
  if(config.n_out)
    size=outputbuffersize(npft,ncft,&config);
  for(i=0;i<config.n_out;i++)
  {
    path=getpath(config.outputvars[i].filename.name);
    if(strcmp(path,oldpath))
    {
      if(checkdir(path))
        badout++;
      else if(diskfree(path)<size)
        fprintf(stderr,"Warning: Disk space on '%s' is insufficient for output files.\n",path);
      free(oldpath);
      oldpath=path;
    }
    else
      free(path);
  }
  free(oldpath);
  if(config.write_restart_filename!=NULL)
  {
    path=getpath(config.write_restart_filename);
    badout+=checkdir(path);
    free(path);
  }
  if((config.n_out || config.write_restart_filename!=NULL) && badout==0)
  {
    if(isout)
      puts("All output directories exist.");
  }
  else
    fprintf(stderr,"%d output directories do not exist.\n",badout);
  if(bad)
    fprintf(stderr,"%d input files do not exist.\n",bad);
  else if(isout)
    puts("All input files exist.");
  return bad>0 || badout>0;
} /* of 'filesexist' */
