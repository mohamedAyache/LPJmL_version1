/***************************************************************************/
/**                                                                       **/
/**                      t  x  t  2  c  l  m  .  c                        **/
/**                                                                       **/
/**     Converts CRU data files into LPJ climate data files               **/
/**     CRU data files have to be in the format specified in              **/
/**     http://www.cru.uea.ac.uk/~timm/grid/CRU_TS_2_1.html               **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2013-10-15 10:24:05 +0200 (mar., 15 oct. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#undef USE_MPI /* no MPI */

#include "lpj.h"

#define TXT2CLM_VERSION "1.0.004"
#define USAGE "Usage: txt2clm [-h] [-yearcell] [-scale s] crufile clmfile [gridfile]\n"

typedef short Data[NMONTH];

int main(int argc,char **argv)
{
  FILE *file,*gridfile;
  float scale,multiplier;
  char *endptr;
  char num[6];
  int missing,lon,lat;
  Data *data;
  Coord start,end,grid;
  String line;
  Header header;
  int i,j,k,item,n,xsize,ysize;
  header.order=CELLYEAR;
  multiplier=1;
  /* parse command line options */
  for(i=1;i<argc;i++)
    if(argv[i][0]=='-')
    {
      if(!strcmp(argv[i],"-h"))
      {
        printf("txt2clm " TXT2CLM_VERSION " (" __DATE__ ") - convert cru ASCII files to\n"
               "        clm data files for LPJ C version\n");
        printf(USAGE
               "Arguments:\n"
               "-h          print this help text\n"
               "-yearcell   does not revert order in cru file\n"
               "-scale s    scale data by a factor of s\n"
               "crufile     filename of cru data file\n"
               "clmfile     filename of clm data file\n"
               "gridfile    filename of grid data file\n");
        return EXIT_SUCCESS;
      }
      if(!strcmp(argv[i],"-yearcell"))
        header.order=YEARCELL;
      else if(!strcmp(argv[i],"-scale"))
      {
        if(i==argc-1)
        {
          fputs("Argument missing after '-scale' option.\n"
                USAGE,stderr);
          return EXIT_FAILURE;
        }
        multiplier=(float)strtod(argv[++i],&endptr);
        if(*endptr!='\0')
        {
          fprintf(stderr,"Invalid value '%s' for option '-scale'.\n",
                  argv[i]);
          return EXIT_FAILURE;
        }
      }
      else
      {
        fprintf(stderr,"Error: invalid option '%s'.\n",argv[i]);
        fprintf(stderr,USAGE);
        return EXIT_FAILURE;
      }
    }
    else
      break;
  if(argc<i+2)
  {
    fputs("Filename(s) missing.\n"
          USAGE,stderr);
    return EXIT_FAILURE;
  }
  file=fopen(argv[i],"r");
  if(file==NULL)
  {
    fprintf(stderr,"Error opening '%s': %s\n",argv[i],strerror(errno));
    return EXIT_FAILURE;
  }
  /* skip first three lines */
  for(n=0;n<3;n++)
  {
    fgets(line,STRING_LEN,file);
    printf("HDR: %s",line);
  }
  /* parse header */
  fscanf(file,"[Long=%lf, %lf] [Lati= %lf, %lf] [Grid X,Y= %d, %d]\n",
         &start.lon,&end.lon,&start.lat,&end.lat,&xsize,&ysize);
  fscanf(file,"[Boxes= %d] [Years=%d-%d] [Multi= %f] [Missing=%d]\n",
         &header.ncell,&header.firstyear,&header.nyear,&scale,&missing);
  printf("First year:%d\n"
         "Last year: %d\n"
         "Number of cells: %d\n"
         "Resolution: %g x %g\n"
         "Byte order in output files: %s\n",
         header.firstyear,header.nyear,header.ncell,
         (end.lon-start.lon)/xsize,(end.lat-start.lat)/ysize,
         bigendian() ? "big-endian" : "little-endian");
  header.firstcell=0;
  header.cellsize=0.5;
  data=newvec(Data,header.ncell*header.nyear);
  if(data==NULL)
  {
    printallocerr("data");
    return EXIT_FAILURE;
  }
  if(argc>i+2)
  {
    gridfile=fopen(argv[i+2],"wb");
    if(gridfile==NULL)
    {
      fprintf(stderr,"Error creating '%s': %s\n",argv[i+2],strerror(errno));
      return EXIT_FAILURE;
    }
    header.nyear=1;
    header.nbands=2;
    header.scalar=0.01;
    fwriteheader(gridfile,header,LPJGRID_HEADER,LPJGRID_VERSION);
  }
  else
    gridfile=NULL;
  num[5]='\0';
  header.nyear-=header.firstyear-1;
  header.scalar=multiplier/scale;
  for(n=0;n<header.ncell;n++)
  {
    if(fscanf(file,"Grid-ref= %d, %d\n",&lon,&lat)!=2)
    {
      fprintf(stderr,"Error reading cell=%d\n",n);
      return EXIT_FAILURE;
    }
#ifdef DEBUG
    //printf("%d %d\n",lon,lat);
#endif
    if(gridfile!=NULL)
    {
      grid.lon=start.lon+(end.lon-start.lon)/xsize*(lon-1);
      grid.lat=start.lat+(end.lat-start.lat)/ysize*(lat-1);
#ifdef DEBUG
     // printf("%f %f\n",grid.lon,grid.lat);
#endif
      writecoord(gridfile,grid);
    }
    for(k=0;k<header.nyear;k++)
    {
      fgets(line,STRING_LEN,file);
      for(j=0;j<NMONTH;j++)
      {
        item=atoi(strncpy(num,line+j*5,5));
        data[k+n*header.nyear][j]=(short)(item*scale);
      }
    }
  }
  if(gridfile!=NULL)
    fclose(gridfile);
  file=fopen(argv[i+1],"wb");
  if(file==NULL)
  {
    fprintf(stderr,"Error creating '%s': %s\n",argv[i+1],strerror(errno));
    return EXIT_FAILURE;
  }
  header.nbands=NMONTH;
  fwriteheader(file,header,LPJ_CLIMATE_HEADER,LPJ_CLIMATE_VERSION);
  if(header.order==CELLYEAR)
  {
    for(i=0;i<header.nyear;i++)
      for(j=0;j<header.ncell;j++)
        fwrite(data+j*header.nyear+i,sizeof(Data),1,file);
  }
  else
    fwrite(data,sizeof(Data),header.nyear*header.ncell,file);
  free(data);
  fclose(file);
  return EXIT_SUCCESS;
} /* of 'main' */
