/***************************************************************************/
/**                                                                       **/
/**                          p  r  i  n  t  c  l  m  .  c                 **/
/**                                                                       **/
/**     Print contents of *.clm files                                     **/
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

#include "lpj.h"

#define USAGE "Usage: %s [-header] [-data] [-longheader] [-type {byte|short|int|float|double}]\n       [-nbands n] [-start s] [-end e] [-first f] [-last l] filename ...\n"
#define NO_HEADER 1
#define NO_DATA 2

static char *types[]={"byte","short","int","float","double"};
static size_t typesizes[]={1,sizeof(short),sizeof(int),sizeof(float),
                           sizeof(double)};

static void printclm(const char *filename,int output,int nbands,int version,
                     int start,int stop,int first,int last,Type type)
{
  FILE *file;
  time_t mod_date;
  long long size;
  Header header;
  Restartheader restartheader;
  String id;
  short sdata;
  int idata;
  char byte;
  float fdata;
  double ddata;
  int year,cell,i;
  Bool swap,isrestart,isreservoir;
  Reservoir reservoir;
  file=fopen(filename,"rb");
  if(file==NULL)
  {
    fprintf(stderr,"Error opening '%s': %s.\n",filename,strerror(errno));
    return;
  }
  if(freadanyheader(file,&header,&swap,id,&version))
  {
    fprintf(stderr,"Error reading header in '%s', perhaps not a clm file.\n",
            filename);
    return;
  }
  if(nbands!=-1)
    header.nbands=nbands;
  isrestart=(!strcmp(id,RESTART_HEADER));
  isreservoir=(!strcmp(id,LPJRESERVOIR_HEADER));
  if((output & NO_HEADER)==0)
  {
    mod_date=getfiledate(filename);
    printf("Created:\t%s"
           "Id:\t\t%s\n"
           "Version:\t%d\n"
           "Byte order:\t",
           asctime(localtime(&mod_date)),id,version);
    if(bigendian())
      printf((swap) ? "Little endian" : "Big endian");
    else
      printf((swap) ? "Big endian" : "Little endian");
    putchar('\n');
    printheader(header);
    if(isrestart)
    {
      if(RESTART_VERSION==version)
      {
        freadrestartheader(file,&restartheader,swap);
        printf("Land use:\t\t%s\n"
               "River routing:\t\t%s\n"
               "Fixed sowing date:\t%s\n",
               bool2str(restartheader.landuse),
               bool2str(restartheader.river_routing),
               bool2str(restartheader.sdate_option));
        if(restartheader.datatype>=0 && restartheader.datatype<5)
          printf("Real type:\t%s\n",types[restartheader.datatype]);
      }
      else
        fprintf(stderr,"Warning: invalid restart version %d.\n",version);
    }
  }
  if(!isrestart && !isreservoir)
  {
    size=getfilesize(filename)-headersize(id,version);
    if(size!=typesizes[type]*header.ncell*header.nbands*header.nyear)
      fputs("Warning: file length does not match header.\n",stderr);
  }
  if((output & NO_DATA)==0)
  {
    if(last==INT_MAX)
      last=header.ncell-first;
    else
      last-=first-1;
    if(isrestart)
      fputs("File is restart file, use 'lpjprint' to print content.\n",stderr);
    else if(isreservoir)
    {
      puts("Cell  Year Capacity    Area   Inst. cap. Height Purpose\n"
           "----- ---- ----------- ------ ---------- ------ ---------");
      if(fseek(file,sizeof(Reservoir)*(first-header.firstcell),SEEK_CUR))
      {
        fprintf(stderr,"Error seeking in '%s' to position %d.\n",filename,first);
        fclose(file);
        return;
      }
      for(cell=0;cell<last;cell++)
      {
        if(readreservoir(&reservoir,swap,file))
        {
          fprintf(stderr,"Unexpected end of file in '%s' at position %d.\n",
                  filename,cell);
          break;
        }
        if(reservoir.year>0)
        {
          printf("%5d %4d %11g %6g %10d %6d",
                 cell,reservoir.year,reservoir.capacity,reservoir.area,
                 reservoir.inst_cap,reservoir.height);
          for(i=0;i<NPURPOSE;i++)
            printf(" %1d",reservoir.purpose[i]);
          putchar('\n');
        }
      } /* of for(cell=...) */
    }
    else
    {
      if(header.nyear<=0)
      {
        fprintf(stderr,"Invalid nyear=%d, set to one.\n",header.nyear);
        header.nyear=1;
      }
      if(start==INT_MAX)
        start=header.firstyear;
      else if(start<header.firstyear)
      {
        fprintf(stderr,"Invalid first year %d, set to %d.\n",start,header.firstyear);
        start=header.firstyear;
      }
      else if(start>header.firstyear+header.nyear-1)
      {
        fprintf(stderr,"Invalid first year %d>%d, no output written.\n",start,header.firstyear+header.nyear-1);
        fclose(file);
        return;
      }
      if(stop==INT_MAX)
        stop=header.firstyear+header.nyear-1;
      else if(stop>header.firstyear+header.nyear-1)
      {
        fprintf(stderr,"Invalid end year %d set to %d.\n",
                start,header.firstyear+header.nyear-1);
        stop=header.firstyear+header.nyear-1;
      }
      if(fseek(file,typesizes[type]*header.ncell*header.nbands*(start-header.firstyear),SEEK_CUR))
      {
        fprintf(stderr,"Error seeking to year %d.\n",start);
        fclose(file);
        return;
      }
      for(year=start;year<=stop;year++)
      {
        printf("Year: %d\n",year);
        fseek(file,typesizes[type]*header.nbands*(first-header.firstcell),SEEK_CUR);
        for(cell=0;cell<last;cell++)
        {
          printf("%5d:",cell+first);
          for(i=0;i<header.nbands;i++)
            switch(type)
            {
              case LPJ_BYTE:
                if(fread(&byte,1,1,file)!=1)
                {
                  putchar('\n');
                  fprintf(stderr,"Unexpected end of file at year %d, cell %d, band %d.\n",
                          year,cell,i);
                  return;
                }
                printf(" %3d",(int)byte);
                break;
              case LPJ_SHORT:
                if(freadshort(&sdata,1,swap,file)!=1)
                {
                  putchar('\n');
                  fprintf(stderr,"Unexpected end of file at year %d, cell %d, band %d.\n",
                          year,cell,i);
                  return;
                }
                printf(" %5d",(int)sdata);
                break;
              case LPJ_INT:
                if(freadint(&idata,1,swap,file)!=1)
                {
                  putchar('\n');
                  fprintf(stderr,"Unexpected end of file at year %d, cell %d, band %d.\n",
                          year,cell,i);
                  return;
                }
                printf(" %6d",idata);
                break;
              case LPJ_FLOAT:
                if(freadfloat(&fdata,1,swap,file)!=1)
                {
                  putchar('\n');
                  fprintf(stderr,"Unexpected end of file at year %d, cell %d, band %d.\n",
                          year,cell,i);
                  return;
                }
                printf(" %6g",fdata);
                break;
              case LPJ_DOUBLE:
                if(freaddouble(&ddata,1,swap,file)!=1)
                {
                  putchar('\n');
                  fprintf(stderr,"Unexpected end of file at year %d, cell %d, band %d.\n",
                          year,cell,i);
                  return;
                }
                printf(" %6g",ddata);
                break;
            }
          putchar('\n');
        } /* of for(cell=...) */
        fseek(file,typesizes[type]*header.nbands*(header.ncell-last-first+header.firstcell),SEEK_CUR);
      }
    }
  }
  fclose(file);
} /* of 'printclm' */

int main(int argc,char **argv)
{
  int i,output,index,start,stop,first,last,nbands,version;
  Type type;
  const char *progname;
  char *endptr;
  progname=strippath(argv[0]);
  output=0;
  first=0;
  start=stop=last=INT_MAX;
  type=LPJ_SHORT;
  nbands=-1;
  version=READ_VERSION;
  for(i=1;i<argc;i++)
    if(argv[i][0]=='-')
    {
      if(!strcmp(argv[i],"-header"))
        output|=NO_HEADER;
      else if(!strcmp(argv[i],"-data"))
        output|=NO_DATA;
      else if(!strcmp(argv[i],"-longheader"))
        version=2;
      else if(!strcmp(argv[i],"-first"))
      {
        if(argc-1==i)
        {
          fprintf(stderr,"Argument missing for option '-first'.\n"
                  USAGE,progname);
          return EXIT_FAILURE;
        }
        first=strtol(argv[++i],&endptr,10);
        if(*endptr!='\0')
        {
          fprintf(stderr,"Invalid number '%s' for options -first.\n",argv[i]);
          return EXIT_FAILURE;
        }
      }
      else if(!strcmp(argv[i],"-last"))
      {
        if(argc-1==i)
        {
          fprintf(stderr,"Argument missing for option '-last'.\n"
                  USAGE,progname);
          return EXIT_FAILURE;
        }
        last=strtol(argv[++i],&endptr,10);
        if(*endptr!='\0')
        {
          fprintf(stderr,"Invalid number '%s' for options -last.\n",argv[i]);
          return EXIT_FAILURE;
        }
      }
      else if(!strcmp(argv[i],"-start"))
      {
        if(argc-1==i)
        {
          fprintf(stderr,"Argument missing for option '-start'.\n"
                  USAGE,progname);
          return EXIT_FAILURE;
        }
        start=strtol(argv[++i],&endptr,10);
        if(*endptr!='\0')
        {
          fprintf(stderr,"Invalid number '%s' for options -start.\n",argv[i]);
          return EXIT_FAILURE;
        }
      }
      else if(!strcmp(argv[i],"-nbands"))
      {
        if(argc-1==i)
        {
          fprintf(stderr,"Argument missing for option '-nbands'.\n"
                  USAGE,progname);
          return EXIT_FAILURE;
        }
        nbands=strtol(argv[++i],&endptr,10);
        if(*endptr!='\0')
        {
          fprintf(stderr,"Invalid number '%s' for options -nbands.\n",argv[i]);
          return EXIT_FAILURE;
        }
        if(nbands<1)
        {
          fputs("Invalid value for number of bands, must be >0.\n",stderr);
          return EXIT_FAILURE;
        }
      }
      else if(!strcmp(argv[i],"-end"))
      {
        if(argc-1==i)
        {
          fprintf(stderr,"Argument missing for option '-end'.\n"
                  USAGE,progname);
          return EXIT_FAILURE;
        }
        stop=strtol(argv[++i],&endptr,10);
        if(*endptr!='\0')
        {
          fprintf(stderr,"Invalid number '%s' for options -end.\n",argv[i]);
          return EXIT_FAILURE;
        }
        if(start!=INT_MAX && stop<start)
        {
          fprintf(stderr,"Error: argument of '-end' must be  >=%d.\n",start);
          return EXIT_FAILURE;
        }
      }
      else if(!strcmp(argv[i],"-type"))
      {
        if(argc-1==i)
        {
          fprintf(stderr,"Argument missing for option '-type'.\n"
                  USAGE,progname);
          return EXIT_FAILURE;
        }
        index=findstr(argv[++i],types,5);
        if(index==NOT_FOUND)
        {
          fprintf(stderr,"Invalid argument for option '-type'.\n"
                  USAGE,progname);
          return EXIT_FAILURE;
        }
        type=(Type)index;
      }
      else
      {
        fprintf(stderr,"Invalid option '%s'.\n"
                USAGE,argv[i],progname);
        return EXIT_FAILURE;
      }
    }
    else
      break;
  argc-=i;
  argv+=i;
  if(argc==0)
  {
    fprintf(stderr,"Error: Filename missing.\n"
            USAGE,progname);
    return EXIT_FAILURE;
  }
  for(i=0;i<argc;i++)
  {
    if(argc>1)
      printf("Filename:\t%s\n",argv[i]);
    printclm(argv[i],output,nbands,version,start,stop,first,last,type);
  }
  return EXIT_SUCCESS;
} /* of 'main' */
