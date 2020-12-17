/***************************************************************************/
/**                                                                       **/
/**                      c  a  t  c  l  m  .  c                           **/
/**                                                                       **/
/**     Concatenates LPJ climate data files                               **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 11.12.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

int main(int argc,char **argv)
{
  Header header,oldheader;
  String id;
  int i,j,k,firstyear,version,n,setversion;
  FILE *in,*out;
  short *values;
  Bool swap;
  setversion=READ_VERSION;
  if(argc>1 && !strcmp(argv[1],"-longheader"))
  {
    setversion=2;
    argc--;
    argv++;
  }
  if(argc<3)
  { 
    fprintf(stderr,"Error: Argument(s) missing.\n");
    return EXIT_FAILURE;
  }
  n=argc-2;
  out=fopen(argv[argc-1],"wb");
  if(out==NULL)
  {
    fprintf(stderr,"Error creating '%s': %s\n",argv[argc-1],strerror(errno));
    return EXIT_FAILURE;
  }
  for(i=0;i<n;i++)
  {
    in=fopen(argv[i+1],"rb");
    if(in==NULL)
    {
      fprintf(stderr,"Error opening '%s': %s\n",argv[i+1],strerror(errno));
      return EXIT_FAILURE;
    }
    if(i)
    {
      version=setversion;
      if(freadheader(in,&header,&swap,id,&version))
      {
        fprintf(stderr,"Error reading header in '%s'.\n",argv[i+1]);
        return EXIT_FAILURE;
      }
      if(header.order!=oldheader.order)
      {
        fprintf(stderr,"Error: Different order in '%s'.\n",argv[i+1]);
        return EXIT_FAILURE;
      }
      if(header.ncell!=oldheader.ncell)
      {
        fprintf(stderr,"Error: Different number of cells in '%s'.\n",argv[i+1]);
        return EXIT_FAILURE;
      }
      if(header.firstcell!=oldheader.firstcell)
      {
        fprintf(stderr,"Error: Different index of first cell in '%s'.\n",argv[i+1]);
        return EXIT_FAILURE;
      }
      if(header.nbands!=oldheader.nbands)
      {
        fprintf(stderr,"Error: Different number of bands in '%s'.\n",argv[i+1]);
        return EXIT_FAILURE;
      }
      if(header.firstyear!=oldheader.firstyear+oldheader.nyear)
      {
        fprintf(stderr,"Error: First year=%d in '%s' is different from %d.\n",header.firstyear,argv[i+1],oldheader.firstyear+oldheader.nyear);
        return EXIT_FAILURE;
      }
    }
    else
    {
      version=setversion;
      if(freadanyheader(in,&header,&swap,id,&version))
      {
        fprintf(stderr,"Error reading header in '%s'.\n",argv[i+1]);
        return EXIT_FAILURE;
      }
      fseek(out,headersize(id,version),SEEK_SET);
      firstyear=header.firstyear;
    }
    values=newvec(short,header.nbands*header.ncell);
    for(j=0;j<header.nyear;j++)
    {
      if(fread(values,sizeof(short),header.nbands*header.ncell,in)!=header.nbands*header.ncell)
      {
        fprintf(stderr,"Error reading from '%s'\n",argv[i+1]);
        return EXIT_FAILURE;
      }
      if(swap)
        for(k=0;k<header.nbands*header.ncell;k++)
          values[k]=swapshort(values[k]);
      if(fwrite(values,sizeof(short),header.nbands*header.ncell,out)!=header.nbands*header.ncell)
      {
        fprintf(stderr,"Error writing to '%s'\n",argv[argc-1]);
        return EXIT_FAILURE;
      }
    }
    free(values);
    fclose(in); 
    oldheader=header;
  }
  header.nyear=header.firstyear+header.nyear-firstyear;
  header.firstyear=firstyear;
  rewind(out);
  fwriteheader(out,header,id,version);
  fclose(out);
  return EXIT_SUCCESS;
} /* of 'main' */
