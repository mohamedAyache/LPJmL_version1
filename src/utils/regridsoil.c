/***************************************************************************/
/**                                                                       **/
/**                    r  e  g  r  i  d  s  o  i  l  .  c                 **/
/**                                                                       **/
/**     soil data in 0.5 degree resolution is regridded to 0.25 degree    **/
/**     resolution.                                                       **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  21.01.2013                                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

int main(int argc,char **argv)
{
  FILE *file;
  Header header,header2;
  Intcoord *c,*c2;
  Bool swap,same;
  Byte *soil,zero=0;
  Coord coord,coord2;
  int i,j,version,setversion;
  setversion=READ_VERSION;
  same=FALSE;
  for(i=1;i<argc;i++)
    if(argv[i][0]=='-')
    {
      if(!strcmp(argv[i],"-longheader"))
        setversion=2;
      else if(!strcmp(argv[i],"-same"))
        same=TRUE;
      else
      {
        fprintf(stderr,"Invalid option '%s'.\n",argv[i]);
        return EXIT_FAILURE;
      }
    }
    else
      break;
  argc-=i-1;
  argv+=i-1;

  if(argc<5)
  {
    fprintf(stderr,"Missing arguments.\n"
           "Usage: %s [-same] [-longheader] coord0.5.clm coord0.25.clm soil0.5.bin soil0.25.bin\n",
            argv[0]);
    return EXIT_FAILURE;
  }
  file=fopen(argv[1],"rb");
  if(file==NULL)
  {
    fprintf(stderr,"Error opening '%s': %s.\n",argv[1],strerror(errno));
    return EXIT_FAILURE;
  }
  version=setversion;
  if(freadheader(file,&header,&swap,LPJGRID_HEADER,&version))
  {
    fprintf(stderr,"Error reading header in '%s'.\n",argv[1]);
    fclose(file);
    return EXIT_FAILURE;
  }
  if(version==1)
    header.scalar=0.01;
  c=newvec(Intcoord,header.ncell);
  if(c==NULL)
  {
    printallocerr("c");
    return EXIT_FAILURE;
  }
  for(i=0;i<header.ncell;i++)
    if(readintcoord(file,c+i,swap))
    {
      fprintf(stderr,"Error reading cell %d in '%s'.\n",i,argv[1]);
      return EXIT_FAILURE;
    }
  fclose(file);
  file=fopen(argv[2],"rb");
  if(file==NULL)
  {
    fprintf(stderr,"Error opening '%s': %s.\n",argv[2],strerror(errno));
    return EXIT_FAILURE;
  }
  version=setversion;
  if(freadheader(file,&header2,&swap,LPJGRID_HEADER,&version))
  {
    fprintf(stderr,"Error reading header in '%s'.\n",argv[2]);
    fclose(file);
    return EXIT_FAILURE;
  }
  if(version==1)
  {
    if(!same)
      header2.cellsize=0.25;
    header2.scalar=0.01;
  }
  same=(header.scalar==header2.scalar);
  c2=newvec(Intcoord,header2.ncell);
  if(c2==NULL)
  {
    printallocerr("c2");
    return EXIT_FAILURE;
  }
  for(i=0;i<header2.ncell;i++)
    if(readintcoord(file,c2+i,swap))
    {
      fprintf(stderr,"Error reading cell %d in '%s'.\n",i,argv[2]);
      return EXIT_FAILURE;
    }
  fclose(file);
  file=fopen(argv[3],"rb");
  if(file==NULL)
  {
    fprintf(stderr,"Error opening '%s': %s.\n",argv[3],strerror(errno));
    return EXIT_FAILURE;
  }
  soil=malloc(header.ncell);
  if(soil==NULL)
  {
    printallocerr("soil");
    return EXIT_FAILURE;
  }
  if(fread(soil,1,header.ncell,file)!=header.ncell)
  {
    fprintf(stderr,"Error reading '%s': %s.\n",argv[3],strerror(errno));
    return EXIT_FAILURE;
  }
  fclose(file);
  file=fopen(argv[4],"wb");
  if(file==NULL)
  {
    fprintf(stderr,"Error creating '%s': %s.\n",argv[4],strerror(errno));
    return EXIT_FAILURE;
  }
  for(i=0;i<header2.ncell;i++)
  {
    coord2.lat=c2[i].lat*header2.scalar;
    coord2.lon=c2[i].lon*header2.scalar;
    if(!same)
    {
      if(coord2.lat>=0)
        coord2.lat+=0.005;
      else
        coord2.lat-=0.005;
      if(coord2.lon>=0)
        coord2.lon+=0.005;
      else
        coord2.lon-=0.005;
      coord2.lat-=0.125;
      coord2.lon-=0.125;
    }
#ifdef DEBUG
   /* printf("%d ",i);
    printcoord(coord2);
    fputs(":",stdout);*/
#endif
    for(j=0;j<header.ncell;j++)
    {
      if(same)
      {
        coord.lat=c[j].lat*header.scalar;
        coord.lon=c[j].lon*header.scalar;
      }
      else
      {
        coord.lat=c[j].lat*header.scalar-0.25;
        coord.lon=c[j].lon*header.scalar-0.25;
      }
      if((same && coord2.lat==coord.lat && coord2.lon==coord.lon)||
         (!same && coord2.lat-coord.lat<=0.25 && coord2.lon-coord.lon<=0.25))
      {
#ifdef DEBUG
        //printf("%d  ",j);
        //printcoord(coord);
#endif
        break;
      }
    }
#ifdef DEBUG
    printf("\n");
#endif
    if(j==header.ncell)
      fwrite(&zero,1,1,file);
    else
      fwrite(soil+j,1,1,file);
  }
  fclose(file);
  return EXIT_SUCCESS;
}  /* of 'main' */
