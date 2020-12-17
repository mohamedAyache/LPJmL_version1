/***************************************************************************/
/**                                                                       **/
/**                  c  f  t  2  c  l  m  .  c                            **/
/**                                                                       **/
/**     Program converts cftfile to LPJ *.clm file                        **/
/**                                                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 24.09.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include <sys/stat.h>
#include "lpj.h"

#define NBAND 26
#define NYEAR 306
#define SYEAR 1700 

#define headercft_VERSION "1.0.003"
#define USAGE "Usage: %s [-h] [-swap] [-nbands n] [-firstyear s] [-nyear y] [-lastyear e] cftfile outfile\n"

int main(int argc,char **argv)
{
  FILE *ifile,*ofile;
  short *data;
  int i,n,k,year,count,band;
  Header header;
  Bool revorder;
  struct stat filestat;
  revorder=FALSE;
  header.nyear=NYEAR;
  header.firstyear=SYEAR;
  header.order=CELLYEAR;
  header.firstcell=0;
  header.nbands=NBAND;
  header.cellsize=0.5;
  header.scalar=0.001;
  for(i=1;i<argc;i++)
    if(argv[i][0]=='-')
    {
      if(!strcmp(argv[i],"-h"))
      {
        printf("%s " headercft_VERSION " (" __DATE__ ") - adds header to cftfile for lpj C version\n",argv[0]);
        printf(USAGE
               "-h           print this help text\n" 
               "-swap        change byte order in cftfile\n"
               "-nbands n    number of bands, default is %d\n"
               "-firstyear s first year, default is %d\n"
               "-lastyear e  last year, default is %d\n"
               "-nyear y     number of years, default is %d\n"
               "cftfile      filename of input cft data file\n"
               "outfile      filename of output cft data file\n",argv[0],NBAND,SYEAR,SYEAR+NYEAR-1,NYEAR);
        return EXIT_SUCCESS;
      }
      else if(!strcmp(argv[i],"-swap"))
        revorder=TRUE;
      else if(!strcmp(argv[i],"-nbands"))
        header.nbands=atoi(argv[++i]);
      else if(!strcmp(argv[i],"-nyear"))
        header.nyear=atoi(argv[++i]);
      else if(!strcmp(argv[i],"-firstyear"))
        header.firstyear=atoi(argv[++i]);
      else if(!strcmp(argv[i],"-lastyear"))
        header.nyear=atoi(argv[++i])-header.firstyear+1;
      else
      {
        fprintf(stderr,"Error: invalid option '%s'.\n",argv[i]);
        fprintf(stderr,USAGE,argv[0]);
        return EXIT_FAILURE;
      }
    }
    else 
      break;
  if(argc<i+2)
  {
    fprintf(stderr,"Filenames missing.\n");
    fprintf(stderr,USAGE,argv[0]);
    return EXIT_FAILURE;
  }
  ifile=fopen(argv[i],"rb");
  if(ifile==NULL)
  {
    fprintf(stderr,"Error opening '%s': %s\n",argv[i],strerror(errno));
    return EXIT_FAILURE;
  }
  fstat(fileno(ifile),&filestat);
  n=filestat.st_size/sizeof(short)/header.nyear;
  if(filestat.st_size % (sizeof(short)*header.nyear*header.nbands))
    fprintf(stderr,"Warning: File size is multiple of nbands * nyear\n"); 
  printf("Number of cells: %d\n",n/header.nbands);
  ofile=fopen(argv[i+1],"wb");
  if(ofile==NULL)
  {
    fprintf(stderr,"Error creating '%s': %s\n",argv[i+1],strerror(errno));
    return EXIT_FAILURE;
  }
  header.ncell=n/header.nbands;
  fwriteheader(ofile,header,LPJ_LANDUSE_HEADER,LPJ_LANDUSE_VERSION);

  data=(short *)malloc(n*sizeof(short));
  for(year=0;year<header.nyear;year++)
  {
    fread(data,sizeof(short),n,ifile);
    if(revorder)
      for(k=0;k<n;k++)
	data[k]=swapshort(data[k]);
    count=0;
    for(band=0;band<n;band++)
      if(data[band]>0)
	count++;

    printf("year:%d count:%d\n",year,count);

    fwrite(data,sizeof(short),n,ofile);
  }
  fclose(ifile);
  free(data);
  fclose(ofile);
  return EXIT_SUCCESS;
} /* of 'main' */
