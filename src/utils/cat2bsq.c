/* CAT2BSQ */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>
/*#include <sys/types.h>*/
#include <sys/stat.h>
#include "types.h"

#define FLOAT 0
#define SHORT 1
#define DOUBLE 2
#define INT 3
#define CHAR 4
#define NOT_FOUND -1
#define USAGE "Usage: cat2bsq [-type {float|double|short|int|char}] [-o outfile] [-bands n] infilename ...\n"

static int gettype(const char *s)
{
  int i;
  static char *types[]={"float","short","double","int","char"};
  for(i=0;i<5;i++)
    if(!strcmp(s,types[i]))
      return i;
  return NOT_FOUND;
}

int main(int argc,char **argv)
{
  static int sizes[]={sizeof(float),sizeof(short),sizeof(double),sizeof(int),sizeof(char)};
  FILE *ofp;
  FILE **ifp;
  void *data;
  int i,j,k,p,*n,runs,bands,sum;
  char *outfile;
  struct stat filestat;
  k=FLOAT;
  bands=103;
  outfile=NULL;
  for(i=0;i<argc;i++)
    if(argv[i][0]=='-')
    {
      if(!strcmp(argv[1],"-type"))
      {
        if(argc==i+1)
        {
          fprintf(stderr,"Argument missing.\n");
          fprintf(stderr,USAGE);
          return EXIT_FAILURE;
        }
        else
        {
          k=gettype(argv[++i]);
          if(k==NOT_FOUND)
          {
            fprintf(stderr,"Invalid argument '%s'.\n",argv[i]);
            fprintf(stderr,USAGE);
            return EXIT_FAILURE;
          }
        }
      }
      else if(!strcmp(argv[i],"-o"))
      {
        if(argc==i+1)
        {
          fprintf(stderr,"Argument missing.\n");
          fprintf(stderr,USAGE);
          return EXIT_FAILURE;
        }
        else
         outfile=argv[++i];
      }
      else if(!strcmp(argv[i],"-bands"))
      {
        if(argc==i+1)
        {
          fprintf(stderr,"Argument missing.\n");
          fprintf(stderr,USAGE);
          return EXIT_FAILURE;
        }
        else
         bands=atoi(argv[++i]);
      }
      else
      {
        fprintf(stderr,"Invalid option '%s'.\n",argv[i]);
        fprintf(stderr,USAGE);
        return EXIT_FAILURE;
      }
    }
    else
      break;
  runs=argc-i;
  if(i==0)
  {
    fprintf(stderr,"Argument is missing\n" USAGE);
    return EXIT_FAILURE;
  }
  if(outfile==NULL)
    ofp=stdout;
  else
  {
    ofp=fopen(outfile,"wb");
    if(ofp==NULL)
    {
      fprintf(stderr,"Error opening '%s': %s\n",outfile,strerror(errno));
      return EXIT_FAILURE;
    }
  }
  ifp=newvec(FILE *,runs);
  n=(int *)malloc(runs*sizeof(int));
  for(j=0;j<runs;j++)
  {

    if(!(ifp[j]=fopen(argv[i+j],"rb")))
    {
      fprintf(stderr,"Error: File open failed on path %s: %s\n",argv[i+j],strerror(errno));
      return EXIT_FAILURE;
    }
    fstat(fileno(ifp[j]),&filestat);
    if(filestat.st_size % (bands*sizes[k]))
    {
      fprintf(stderr,"Error: Filesize=%d is not divisible without remainder=%d, bands=%d, size=%d.",
           (int)filestat.st_size,(int)filestat.st_size % (bands*sizes[k]),bands,sizes[k]);
      return EXIT_FAILURE;
    }
    /* calculate number of pixels in each chunk and store it in array n[i] */
    n[j]=filestat.st_size/bands/sizes[k];
  }

  for(p=0;p<bands;p++)
  {
    for(i=0;i<runs;i++)
    {
      if(outfile!=NULL && p==0) 
        printf("run: %d n: %d\n",i,n[i]);
      fseek(ifp[i],sizes[k]*n[i]*p,SEEK_SET);
      data=malloc(n[i]*sizes[k]);
      fread(data,sizes[k],n[i],ifp[i]);
      fwrite(data,sizes[k],n[i],ofp);
      free(data);
    }
  }
  if(outfile!=NULL)
  {
    sum=0;
    for(i=0;i<runs;i++)
      sum+=n[i];
    printf("Number of cells: %d\n",sum); 
  }
  for(i=0;i<runs;i++) 
    fclose(ifp[i]);
  fclose(ofp);
  return EXIT_SUCCESS;
} /* of 'main' */
