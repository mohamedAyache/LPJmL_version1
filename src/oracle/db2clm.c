/***************************************************************************/
/**                                                                       **/
/**                       d  b  2  c  l  m  .  c                          **/
/**                                                                       **/
/**     Program reads from PIK Oracle database climate variables and      **/
/**     stores them into clm files for LPJmL                              **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  19.02.2013                                          **/
/**                                                                       **/
/***************************************************************************/

#include <time.h>
#include <unistd.h>
#include "lpj.h"
#include "database.h"

#define DB2CLM_VERSION "1.0.003"
#define USAGE "Usage: %s [-h] [-l] database/passwd [scenario coordfile tempfile cloudfile precfile]\n"

static int find(const int *array,int n,int s)
{
  int i;
  for(i=0;i<n;i++)
    if(s==array[i])
      return TRUE;
  return FALSE;
} /* of 'find' */

int main(int argc,char **argv)
{
  char *title[3];
  const char *progname,*username;
  char *name;
  String line;
  Bool islist;
  int i,j,scenario,first,last,year,n_scenario,*scenarios,perc;
  time_t tstart,tend;
  short value[NDAYYEAR];
  FILE *file,*temp_file,*cloud_file,*prec_file;
  Header header;
  float *lon,*lat,temp[NDAYYEAR+1],cloud[NDAYYEAR+1],prec[NDAYYEAR+1];
  Coord coord;
  int n;
  progname=strippath(argv[0]);
  if(argc==2 && !strcmp(argv[1],"-h"))
  {
    printf("%s Version %s - read climate data from Oracle database for LPJmL\n",
           progname,DB2CLM_VERSION);
    printf(USAGE,progname);
    printf("Arguments:\n"
           "-h              print this help text\n"
           "-l              print table of scenarios in database\n"
           "database/passwd database and password to connect\n"
           "scenario        scenario id\n"
           "coordfile       filename of grid coordinates\n"
           "tempfile        filename of temperature data\n"
           "cloudfile       filename of cloudiness data\n"
           "precfile        filename of precipitation data\n");
    return EXIT_SUCCESS;
  }
  if(argc>2 && !strcmp(argv[1],"-l"))
  {
    islist=TRUE;
    username=argv[2];
  }
  else if(argc<7)
  {
    fprintf(stderr,"Error: Missing argument(s).\n"
            USAGE,progname); 
    return EXIT_FAILURE;
  }
  else
  {
    islist=FALSE;
    username=argv[1];
  }
  snprintf(line,STRING_LEN,
           "%s Version %s (" __DATE__ ")",progname,DB2CLM_VERSION);
  title[0]=line;
  title[1]="Reading climate data from Oracle database for LPJmL " LPJ_VERSION;
  title[2]="(c)2008 PIK Potsdam";
  banner(title,3,78);
  name=getpath(username);
  name[strlen(name)-1]='\0';
  printf("\nConnecting to %s database...\n\n",name);
  free(name);
  db_connect(username,NULL);
  if(islist)
  {
    printf("Table of defined scenarios:\n\n");
    db_printscenarios();
    return EXIT_SUCCESS;
  } 
  n_scenario=db_getnscenarios();
  scenarios=newvec(int,n_scenario);
  db_getscenarios(scenarios,n_scenario);
  scenario=atoi(argv[2]);
  if(!find(scenarios,n_scenario,scenario))
  {
    fprintf(stderr,"Error: Invalid scenario id=%d, must be",scenario);
    for(i=0;i<n_scenario;i++)
    {
      if(i)
        fprintf(stderr,",");
      fprintf(stderr," %d",scenarios[i]);
    }
    fprintf(stderr,".\n");
    return EXIT_FAILURE;
  }
  time(&tstart);
  n=db_getncoords();
  lon=newvec(float,n);
  lat=newvec(float,n);
  db_getcoords(lon,lat,n);
  header.nyear=1;
  header.firstyear=0;
  header.order=0;
  header.firstcell=0;
  header.ncell=n;
  header.nbands=2;
  header.cellsize=0.5;
  header.scalar=0.01;           /*TODO check how these data are stored*/
  file=fopen(argv[3],"wb");
  if(file==NULL)
  {
    fprintf(stderr,"Error creating file '%s': %s\n",argv[3],strerror(errno));
    return EXIT_FAILURE;
  }
  fwriteheader(file,header,LPJGRID_HEADER,LPJGRID_VERSION);
  for(i=0;i<n;i++)
  {
    coord.lon=lon[i];
    coord.lat=lat[i];
    writecoord(file,coord);
  }
  free(lon);
  free(lat);
  fclose(file);
  db_getyears(&first,&last,scenario);
  db_getdescr(line,scenario);
  printf("==============================================================================\n"
         "Scenario:\t\t%s\n"
         "Number of cells:\t%d\n"
         "First year:\t\t%d\n"
         "Last year:\t\t%d\n"
         "==============================================================================\n\n",
         line,n,first,last);
  header.nyear=last-first+1;
  header.firstyear=first;
  header.order=CELLYEAR;
  header.firstcell=0;
  header.ncell=n;
  header.nbands=NDAYYEAR;
  temp_file=fopen(argv[4],"wb");
  if(temp_file==NULL)
  {
    fprintf(stderr,"Error creating file '%s': %s\n",argv[4],strerror(errno));
    return EXIT_FAILURE;
  }
  cloud_file=fopen(argv[5],"wb");
  if(cloud_file==NULL)
  {
    fprintf(stderr,"Error creating file '%s': %s\n",argv[5],strerror(errno));
    return EXIT_FAILURE;
  }
  prec_file=fopen(argv[6],"wb");
  if(prec_file==NULL)
  {
    fprintf(stderr,"Error creating file '%s': %s\n",argv[6],strerror(errno));
    return EXIT_FAILURE;
  }
  header.scalar=0.1;
  fwriteheader(temp_file,header,LPJ_CLIMATE_HEADER,LPJ_CLIMATE_VERSION);
  header.scalar=1;
  fwriteheader(prec_file,header,LPJ_CLIMATE_HEADER,LPJ_CLIMATE_VERSION);
  fwriteheader(cloud_file,header,LPJ_CLIMATE_HEADER,LPJ_CLIMATE_VERSION);
  if(isatty(fileno(stdout)))
    printf("Reading year:      [   0%]");
  for(year=first;year<=last;year++)
  {
    if(isatty(fileno(stdout)))
    {
      perc=(float)(year-first)/(last-first)*100;
      printf("\b\b\b\b\b\b\b\b\b\b\b%d [%3d%]",year,perc);
      fflush(stdout);
    }
    for(i=0;i<n;i++)
    {
      db_getclimate(temp,cloud,prec,i,year,scenario);
      for(j=0;j<NDAYYEAR;j++)
        value[j]=(short)(temp[j]*10);
      fwrite(value,sizeof(short),NDAYYEAR,temp_file);
      for(j=0;j<NDAYYEAR;j++)
        value[j]=(short)cloud[j];
      fwrite(value,sizeof(short),NDAYYEAR,cloud_file);
      for(j=0;j<NDAYYEAR;j++)
        value[j]=(short)prec[j];
      fwrite(value,sizeof(short),NDAYYEAR,prec_file);
    }
  }
  if(isatty(fileno(stdout)))
    printf("\n");
  time(&tend);
  printf("Run time: %d sec.\n",(int)(tend-tstart));
  db_close();
  fclose(temp_file);
  fclose(cloud_file);
  fclose(prec_file);
  return EXIT_SUCCESS;
} /* of 'main' */
