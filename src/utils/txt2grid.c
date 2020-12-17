/***************************************************************************/
/**                                                                       **/
/**                      t  x  t  2  g  r  i  d  .  c                     **/
/**                                                                       **/
/**     Program converts text data to grid clm file format. Data must be  **/
/**     in the following format:                                          **/
/**                                                                       **/
/**     Id,Lon,Lat,X_CRU,Y_CRU                                            **/
/**     1,-179.75,-16.25,1,148                                            **/
/**     2,..                                                              **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2013-09-30 15:43:25 +0200 (lun., 30 sept. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define TXT2GRID_VERSION "1.0.001"
#define USAGE "Usage: txt2grid [-h] [-fmt s] [-skip n] [-cellsize size] gridfile clmfile\n"

int main(int argc,char **argv)
{
  FILE *file,*gridfile;
  const char *fmt;
  Coord grid;
  String line;
  int i,nskip,n;
  float lon,lat;
  Header header;
  char *endptr;
  fmt="%*d,%f,%f,%*d,%*d";
  nskip=1;
  header.cellsize=0.5;
  for(i=1;i<argc;i++)
    if(argv[i][0]=='-')
    {
      if(!strcmp(argv[i],"-h"))
      {
        printf("txt2grid " TXT2GRID_VERSION " (" __DATE__ ") - convert text file to\n"
             "         clm grid file for lpj C version\n\n"
             USAGE
             "\nArguments:\n"
             "-h             print this help text\n" 
             "-fmt s         format string for text input, default is '%s'\n"
             "-cellsize size cell size, default is %g\n"
             "-skip n        skip first n lines, default is one\n" 
             "gridfile       filename of grid text file\n"
             "clmfile        filename of clm data file\n",fmt,header.cellsize);
        return EXIT_SUCCESS;
      }
      if(!strcmp(argv[i],"-fmt"))
      {
        if(i==argc-1)
        {
           fputs("Argument missing after '-fmt' option.\n"
                 USAGE,stderr);
           return EXIT_FAILURE;
        }
        fmt=argv[++i];
      }
      else if(!strcmp(argv[i],"-cellsize"))
      {
        if(i==argc-1)
        {
           fputs("Argument missing after '-cellsize' option.\n"
                 USAGE,stderr);
           return EXIT_FAILURE;
        }
        header.cellsize=(float)strtod(argv[++i],&endptr);
        if(*endptr!='\0')
        {
          fprintf(stderr,"Invalid number '%s' for '-cellsize' option.\n",argv[i]);
          return EXIT_FAILURE;
        }
      }

      else if(!strcmp(argv[i],"-skip"))
      {
        if(i==argc-1)
        {
           fputs("Argument missing after '-skip' option.\n"
                 USAGE,stderr);
           return EXIT_FAILURE;
        }
        nskip=strtol(argv[++i],&endptr,10);
        if(*endptr!='\0')
        {
          fprintf(stderr,"Invalid number '%s' for '-skip' option.\n",argv[i]);
          return EXIT_FAILURE;
        }
      }
      else
      {
        fprintf(stderr,"Error: invalid option '%s'.\n"
                USAGE,argv[i]);
        return EXIT_FAILURE;
      }
    }
    else 
      break;
  if(argc<i+2)
  {
    fputs("Filenames missing.\n"
          USAGE,stderr);
    return EXIT_FAILURE;
  }
  file=fopen(argv[i],"r");
  if(file==NULL)
  {
    fprintf(stderr,"Error opening '%s': %s.\n",argv[i],strerror(errno));
    return EXIT_FAILURE;
  }
  for(n=0;n<nskip;n++)
    if(fgets(line,STRING_LEN,file)==NULL)
    {
      fprintf(stderr,"Error skipping file header in '%s'.\n",argv[i]);
      return EXIT_FAILURE;
    }
  header.ncell=0;
  header.nbands=2;
  header.order=CELLYEAR;
  header.firstcell=0;
  header.firstyear=0;
  header.nyear=1;
  header.scalar=0.01;
  gridfile=fopen(argv[i+1],"wb");
  if(gridfile==NULL)
  {
    fprintf(stderr,"Error creating '%s': %s.\n",argv[i+1],strerror(errno));
    return EXIT_FAILURE;
  }
  fwriteheader(gridfile,header,LPJGRID_HEADER,LPJGRID_VERSION);
  while(fscanf(file,fmt,&lon,&lat)==2)
  {
    grid.lon=lon;
    grid.lat=lat;
    writecoord(gridfile,grid);
    header.ncell++;
  }
  printf("Number of grid cells: %d\n",header.ncell);
  rewind(gridfile);
  fwriteheader(gridfile,header,LPJGRID_HEADER,LPJGRID_VERSION);
  fclose(gridfile);
  fclose(file);
  return EXIT_SUCCESS;
} /* of 'main' */
