/***************************************************************************/
/**                                                                       **/
/**                g  e  t  c  e  l  l  i  n  d  e  x  .  c               **/
/**                                                                       **/
/**     Finds cell index in CLM grid file for given latitude/longitude    **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2013-10-10 10:03:49 +0200 (jeu., 10 oct. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define USAGE  "Usage: getcellindex gridfile [lat lon] ...\n"

int main(int argc,char **argv)
{ 
  int i,n,index;
  char *endptr;
  Coordfile file;
  Coord *coords,res,pos;
  Filename name; 
  if(argc<2 || (argc-2) % 2)
  {
    fputs("Missing arguments.\n"
          USAGE,stderr);
    return EXIT_FAILURE;
  }
  name.name=argv[1];
  name.fmt=CLM;
  file=opencoord(&name);
  if(file==NULL)
  {
    fprintf(stderr,"Error opening '%s'.\n",argv[1]);
    return EXIT_FAILURE;
  }
  n=numcoord(file);
  res.lon=res.lat=getcellsizecoord(file);
  coords=newvec(Coord,n);
  if(coords==NULL)
  {
    printallocerr("coords");
    return EXIT_FAILURE;
  }
  for(i=0;i<n;i++)
    if(readcoord(file,coords+i,res))
    {
      fprintf(stderr,"Error reading grid at %d from '%s'.\n",i+1,argv[1]);
      return EXIT_FAILURE;
    }
  closecoord(file);
  if(argc==2)
  {
    puts("Lat     Lon");
    for(i=0;i<n;i++)
      printf("%7.2f %7.2f\n",coords[i].lat,coords[i].lon);
  }
  else
    for(i=2;i<argc;i+=2)
    {
      pos.lat=strtod(argv[i],&endptr);
      if(*endptr!='\0')
      {
        fprintf(stderr,"Invalid argument '%s' for latitude.\n",argv[i]);
        return EXIT_FAILURE;
      }
      pos.lon=strtod(argv[i+1],&endptr);
      if(*endptr!='\0')
      {
        fprintf(stderr,"Invalid argument '%s' for longitude.\n",argv[i+1]);
        return EXIT_FAILURE;
      }
      index=findcoord(pos,coords,n);
      if(index==NOT_FOUND)
        fprintf(stderr,"Coordinate not found for (%.2f, %.2f) in '%s'.\n",
                pos.lat,pos.lon,argv[1]);
      else
        printf("%d\n",index);
    }
  return EXIT_SUCCESS;
} /* of 'main' */
