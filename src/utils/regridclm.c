/***************************************************************************/
/**                                                                       **/
/**                    r  e  g  r  i  d  c  l  m  .  c                    **/
/**                                                                       **/
/**     CLM data in 0.5 degree resolution is regridded to 0.25 degree     **/
/**     resolution.                                                       **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  22.01.2013                                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

int main(int argc,char **argv)
{
  FILE *file,*data_file;
  Header header,header2,header3,header4;
  Intcoord *c,*c2;
  Bool swap,isint,same;
  short *data,*zero;
  int *idata,version;
  long long size;
  Coord coord,coord2;
  int i,j,*index,data_version,setversion;
  String id;
  isint=same=FALSE;
  setversion=READ_VERSION;
  for(i=1;i<argc;i++)
    if(argv[i][0]=='-')
    {
      if(!strcmp(argv[i],"-size4"))
        isint=TRUE;
      else if(!strcmp(argv[i],"-longheader"))
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
    fprintf(stderr,"Error: Missing arguments.\n"
            "Usage: %s [-size4] [-same] [-longheader] coord0.5.clm coord0.25.clm data0.5.clm data0.25.clm\n",
            argv[1-i]);
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
    fclose(file);
    printallocerr("c");
    return EXIT_FAILURE;
  }
  for(i=0;i<header.ncell;i++)
    if(readintcoord(file,c+i,swap))
    {
      fclose(file);
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
    header2.scalar=0.01;
    if(!same)
      header2.cellsize=0.25;
  }
  if(header.cellsize==header2.cellsize)
    same=TRUE;
  c2=newvec(Intcoord,header2.ncell);
  if(c2==NULL)
  {
    fclose(file);
    printallocerr("c2");
    return EXIT_FAILURE;
  }
  for(i=0;i<header2.ncell;i++)
    if(readintcoord(file,c2+i,swap))
    {
      fclose(file);
      fprintf(stderr,"Error reading cell %d in '%s'.\n",i,argv[2]);
      return EXIT_FAILURE;
    }
  fclose(file);
  data_file=fopen(argv[3],"rb");
  if(data_file==NULL)
  {
    fprintf(stderr,"Error opening '%s': %s.\n",argv[3],strerror(errno));
    return EXIT_FAILURE;
  }
  data_version=setversion;
  if(freadanyheader(data_file,&header3,&swap,id,&data_version))
  {
    fprintf(stderr,"Error reading header in '%s'.\n",argv[3]);
    return EXIT_FAILURE;
  }
  if(header3.nyear<=0)
  {
    fprintf(stderr,"Invalid nyear=%d, set to one.\n",header3.nyear);
    header3.nyear=1;
  }
  if(header3.ncell!=header.ncell)
  {
    fprintf(stderr,"Invalid number of cells %d in '%s', not %d.\n",
            header3.ncell,argv[3],header.ncell);
    return EXIT_FAILURE;
  }
  size=getfilesize(argv[3])-headersize(id,data_version);
  if(isint)
  {
    idata=newvec(int,(long long)header3.ncell*header3.nbands);
    if(idata==NULL)
    {
      printallocerr("idata");
      return EXIT_FAILURE;
    }
    if(size!=(long long)header3.ncell*header3.nyear*header3.nbands*sizeof(int))
    {
      header3.nyear=size/(sizeof(int)*header3.ncell*header3.nbands); 
      fprintf(stderr,"File '%s' too short, number of years set to %d.\n",argv[3],header3.nyear);
    }
  }
  else
  {
    data=newvec(short,(long long)header3.ncell*header3.nbands);
    zero=newvec(short,header3.nbands);
    for(i=0;i<header3.nbands;i++)
      zero[i]=0;
    if(data==NULL)
    {
      printallocerr("data");
      return EXIT_FAILURE;
    }
    if(size!=(long long)header3.ncell*header3.nyear*header3.nbands*sizeof(short))
    {
      header3.nyear=size/(sizeof(short)*header3.ncell*header3.nbands); 
      fprintf(stderr,"File '%s' too short, number of years set to %d.\n",argv[3],header3.nyear);
    }
  }
  file=fopen(argv[4],"wb");
  if(file==NULL)
  {
    fprintf(stderr,"Error creating '%s': %s.\n",argv[4],strerror(errno));
    return EXIT_FAILURE;
  }
  header4=header3;
  header4.ncell=header2.ncell;
  if(fwriteheader(file,header4,id,data_version))
  {
    fprintf(stderr,"Error writing header in '%s'.\n",argv[4]);
    return EXIT_FAILURE;
  }
  index=newvec(int,header2.ncell);
  if(index==NULL)
  {
    printallocerr("index");
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
    for(j=0;j<header.ncell;j++)
    {
      if(same)
      {
        if(coord2.lat==c[j].lat*header.scalar && coord2.lon==c[j].lon*header.scalar)
          break;
      }
      else
      {
        coord.lat=c[j].lat*header.scalar-0.25;
        coord.lon=c[j].lon*header.scalar-0.25;
        if(coord2.lat-coord.lat<=0.25 && coord2.lon-coord.lon<=0.25)
          break;
      }
    }
    if(j==header.ncell)
    {
      fputs("Coord ",stderr);
      fprintcoord(stderr,coord2);
      fputs(" not found.\n",stderr);
      index[i]=-1;
      /* return EXIT_FAILURE; */
    }
    else
      index[i]=j;
  }
  if(isint)
    for(i=0;i<header4.nyear;i++)
    {
      if(freadint(idata,(long long)header3.ncell*header3.nbands,swap,data_file)!=(long long)header3.ncell*header3.nbands)
      {
        fprintf(stderr,"Error reading '%s' in year %d.\n",argv[3],header4.firstyear+i);
        return EXIT_FAILURE;
      }
      for(j=0;j<header4.ncell;j++)
      {
        if(fwrite(idata+index[j]*header3.nbands,sizeof(int),header3.nbands,file)!=header3.nbands)
          fprintf(stderr,"Error writing file '%s: %s.\n",argv[4],strerror(errno));
      }
    }
  else
    for(i=0;i<header4.nyear;i++)
    {
      if(freadshort(data,(long long)header3.ncell*header3.nbands,swap,data_file)!=(long long)header3.ncell*header3.nbands)
      {
        fprintf(stderr,"Error reading '%s' in year %d.\n",argv[3],header4.firstyear+i);
        return EXIT_FAILURE;
      }
      for(j=0;j<header4.ncell;j++)
      {
        if(index[j]==-1)
          fwrite(zero,sizeof(short),header3.nbands,file);
        else 
        {
          if(fwrite(data+index[j]*header3.nbands,sizeof(short),header3.nbands,file)!=header3.nbands)
            fprintf(stderr,"Error writing file '%s: %s.\n",argv[4],strerror(errno));
        }
      }
    }
  fclose(file);
  fclose(data_file);
  return EXIT_SUCCESS;
} /* of 'main' */
