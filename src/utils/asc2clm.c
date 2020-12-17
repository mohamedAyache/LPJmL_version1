/***************************************************************************/
/**                                                                       **/
/**                      a  s  c  2  c  l  m  .  c                        **/
/**                                                                       **/
/**     Program converts ARC/Info ASCII (plain text) input data into the  **/
/**     CLM format for LPJ                                                **/
/**     Input must be in the following form:                              **/
/**     ncols   288                                                       **/
/**     nrows   288                                                       **/
/**     xllcorner       98.021                                            **/
/**     yllcorner       12.021                                            **/
/**     cellsize        0.042                                             **/
/**     NODATA_value    -9999.000                                         **/
/**     3636.0 3737.0 ...                                                 **/
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
#include <sys/stat.h>

#define ASC2CLM "1.0.001"
#define USAGE "Usage: asc2clm [-h] [-f] [-firstyear y] [-grid file] [-nbands n] [-header s]\n       [-scale s] infile ... clmfile\n"
#define FIRSTYEAR 1901

int main(int argc,char **argv)
{
  FILE *file,*gridfile,*out;
  Bool first;
  float scale;
  int row,col,nrows,ncols,nyear,nbands;
  int nrows_first,ncols_first,ncell_first;
  float xllcorner_first,yllcorner_first,cellsize_first;
  float xllcorner,yllcorner,cellsize,nodata,value;
  short **data;
  char *endptr;
  Coord grid;
  Header header;
  char *head;
  int i,m,y;
  Bool force;
  struct stat buf;
  char c;
  header.firstyear=FIRSTYEAR;
  header.firstcell=0;
  nyear=1;
  nbands=NMONTH;
  header.order=CELLYEAR;
  scale=1;
  head=LPJ_CLIMATE_HEADER;
  gridfile=NULL;
  force=FALSE;
  /* parse command line options */
  for(i=1;i<argc;i++)
    if(argv[i][0]=='-')
    {
      if(!strcmp(argv[i],"-h"))
      {
        printf("   asc2clm version " ASC2CLM " (" __DATE__ ") help\n"
               "   ==========================================\n\n"
               "Convert gridded ASCII files to clm data files for LPJ C version\n\n"
               USAGE
               "\nArguments:\n"
               "-h           print this help text\n"
               "-f           force overwrite of output file\n"
               "-firstyear y first year, default is %d\n"
               "-grid file   create grid file\n" 
               "-nbands n    number of bands, default is 12\n"
               "-header s    header string, default is " LPJ_CLIMATE_HEADER "\n"
               "-scale s     scale data by a factor of s\n"
               "infile       filename(s) of gridded data file\n"
               "clmfile      filename of clm data file\n",FIRSTYEAR);
        return EXIT_SUCCESS;
      }
      else if(!strcmp(argv[i],"-firstyear"))
      {
        if(i==argc-1)
        {
          fputs("Argument missing after '-firstyear' option.\n"
                USAGE,stderr);
          return EXIT_FAILURE;
        }
        header.firstyear=strtol(argv[++i],&endptr,10);
        if(*endptr!='\0')
        {
          fprintf(stderr,"Invalid number '%s' for option '-firstyear'.\n",
                  argv[i]);
          return EXIT_FAILURE;
        }
      }
      else if(!strcmp(argv[i],"-f"))
        force=TRUE;
      else if(!strcmp(argv[i],"-grid"))
      {
        if(i==argc-1)
        {
          fputs("Argument missing after '-grid' option.\n"
                USAGE,stderr);
          return EXIT_FAILURE;
        }
        gridfile=fopen(argv[++i],"wb");
        if(gridfile==NULL)
        {
          fprintf(stderr,"Error creating '%s': %s.\n",argv[i],strerror(errno));
          return EXIT_FAILURE;
        }
        fwriteheader(gridfile,header,LPJGRID_HEADER,LPJGRID_VERSION);

      }
      else if(!strcmp(argv[i],"-nbands"))
      {
        if(i==argc-1)
        {
          fputs("Argument missing after '-nbands' option.\n"
                USAGE,stderr);
          return EXIT_FAILURE;
        }
        nbands=strtol(argv[++i],&endptr,10);
        if(*endptr!='\0')
        {
          fprintf(stderr,"Invalid number '%s' for option '-nbands'.\n",
                  argv[i]);
          return EXIT_FAILURE;
        }
        if(nbands<1)
        {
          fputs("Error: number of bands must be >0.\n",stderr);
          return EXIT_FAILURE;
        }
      }
      else if(!strcmp(argv[i],"-header"))
      {
        if(i==argc-1)
        {
          fputs("Argument missing after '-header' option.\n"
                USAGE,stderr);
          return EXIT_FAILURE;
        }
        head=argv[++i];
      }
      else if(!strcmp(argv[i],"-scale"))
      {
        if(i==argc-1)
        {
          fputs("Argument missing after '-scale' option.\n"
                USAGE,stderr);
          return EXIT_FAILURE;
        }
        scale=(float)strtod(argv[++i],&endptr);
        if(*endptr!='\0')
        {
          fprintf(stderr,"Invalid number '%s' for option '-scale'.\n",
                  argv[i]);
          return EXIT_FAILURE;
        }
        if(scale==0.0)
        {
          fputs("Error: scale factor must not be zero.\n",stderr);
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
    fputs("Filename(s) missing.\n"
          USAGE,stderr);
    return EXIT_FAILURE;
  }
  argc-=i;
  argv+=i;
  if((argc-1)% nbands)
  {
    fputs("Number of input files is not multiple of nbands.\n",stderr);
    return EXIT_FAILURE;
  }
  nyear=(argc-1)/nbands;
  if(!force)
  {
    if(!stat(argv[argc-1],&buf))
    {
      fprintf(stderr,"File '%s' already exists, ovewrite (y/n)?\n",argv[argc-1]);
      scanf("%c",&c);
      if(c!='y')
        return EXIT_FAILURE;
    }
  }
  out=fopen(argv[argc-1],"wb");
  if(out==NULL)
  {
    fprintf(stderr,"Error creating '%s': %s.\n",argv[argc-1],strerror(errno));
    return EXIT_FAILURE;
  }
  header.nbands=nbands;
  fwriteheader(out,header,head,LPJ_CLIMATE_VERSION);
  first=TRUE;
  for(y=0;y<nyear;y++)
  {
    for(m=0;m<nbands;m++)
    {
      file=fopen(argv[y*nbands+m],"r");
      if(file==NULL)
      {
        fprintf(stderr,"Error opening '%s': %s.\n",argv[y],strerror(errno));
        return EXIT_FAILURE;
      }
      /* parse header  */
      if(fscanf(file,"ncols %d\n",&ncols)!=1)
      {
        fputs("Error reading ncols.\n",stderr);
        return EXIT_FAILURE;
      }
      if(fscanf(file,"nrows %d\n",&nrows)!=1)
      {
        fputs("Error reading nrows.\n",stderr);
        return EXIT_FAILURE;
      }
      if(fscanf(file,"xllcorner %f\n",&xllcorner)!=1)
      {
        fputs("Error reading xllcorner.\n",stderr);
        return EXIT_FAILURE;
      }
      if(fscanf(file,"yllcorner %f\n",&yllcorner)!=1)
      {
        fputs("Error reading yllcorner.\n",stderr);
        return EXIT_FAILURE;
      }
      if(fscanf(file,"cellsize %f\n",&cellsize)!=1)
      {
        fputs("Error reading cellsize.\n",stderr);
        return EXIT_FAILURE;
      }
      if(fscanf(file,"NODATA_value %f\n",&nodata)!=1)
      {
        fputs("Error reading nodata value.\n",stderr);
        return EXIT_FAILURE;
      }
      if(first)
      {
        data=newvec(short *,nrows*ncols);
        if(data==NULL)
        {
          printallocerr("data");
          return EXIT_FAILURE;
        }
        /* allocate data array */
        for(i=0;i<nrows*ncols;i++)
        {
          data[i]=newvec(short,nbands);
          if(data[i]==NULL)
          {
            printallocerr("data");
            return EXIT_FAILURE;
          }
        }
        nrows_first=nrows;
        ncols_first=ncols;
        xllcorner_first=xllcorner;
        yllcorner_first=yllcorner;
        cellsize_first=cellsize;
      }
      else if(nrows_first!=nrows)
      {
        fprintf(stderr,"Number of rows %d in '%s' differs from %d.\n",
                nrows,argv[y],nrows_first);
        return EXIT_FAILURE;
      }
      else if(ncols_first!=ncols)
      {
        fprintf(stderr,"Number of columns %d in '%s' differs from %d.\n",
                ncols,argv[y],ncols_first);
        return EXIT_FAILURE;
      }
      else if(xllcorner_first!=xllcorner)
      {
        fprintf(stderr,"xllcorner %g in '%s' differs from %g.\n",
                xllcorner,argv[y],xllcorner_first);
        return EXIT_FAILURE;
      }
      else if(yllcorner_first!=yllcorner)
      {
        fprintf(stderr,"yllcorner %g in '%s' differs from %g.\n",
                yllcorner,argv[y],yllcorner_first);
        return EXIT_FAILURE;
      }
      else if(cellsize_first!=cellsize)
      {
        fprintf(stderr,"cellsize %g in '%s' differs from %g.\n",
                cellsize,argv[y],cellsize_first);
        return EXIT_FAILURE;
      }
      header.cellsize=cellsize;
      header.ncell=0;
      for(row=0;row<nrows;row++)
        for(col=0;col<ncols;col++)
        {
          if(fscanf(file,"%f",&value)!=1)
          {
            fprintf(stderr,"Error reading data at (%d,%d) at year %d and band %d.\n",
                    row+1,col+1,header.firstyear+y,m+1);
            return EXIT_FAILURE;
          }
          if(value!=nodata)
          {
            if(first && gridfile!=NULL)
            {
              grid.lon=xllcorner+col*cellsize+cellsize*0.5;
              grid.lat=yllcorner+(nrows-row-1)*cellsize+cellsize*0.5; 
              writecoord(gridfile,grid);
            }
            data[header.ncell][m]=(short)(value*scale);
            header.ncell++;
          }
        }
      if(first)
      {
        ncell_first=header.ncell;
        printf("Number of valid grid cells: %d\n",header.ncell);
      }
      else if(header.ncell!=ncell_first)
      {
        fprintf(stderr,"Number of cells %d in '%s' differs from %d.\n",
                header.ncell,argv[y*nbands+m],ncell_first);
        return EXIT_FAILURE;
      }
      first=FALSE;
      fclose(file);
    } 
    for(i=0;i<header.ncell;i++)
      if(fwrite(data[i],sizeof(short),nbands,out)!=nbands)
      {
        fprintf(stderr,"Error writing output: %s.\n",strerror(errno));
        return EXIT_FAILURE;
      }
  }  
  if(gridfile!=NULL)
  {
    rewind(gridfile);
    header.nbands=2;
    header.nyear=1;
    header.scalar=0.01;
    fwriteheader(gridfile,header,LPJGRID_HEADER,LPJGRID_VERSION);
    fclose(gridfile);
  }
  rewind(out);
  header.nyear=nyear;
  header.nbands=nbands;
  header.scalar=scale;
  fwriteheader(out,header,head,LPJ_CLIMATE_VERSION);
  fclose(out);
  return EXIT_SUCCESS;
} /* of 'main' */
