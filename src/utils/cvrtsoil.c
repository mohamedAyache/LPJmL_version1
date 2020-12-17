/***************************************************************************/
/**                                                                       **/
/**                      c  v  r  t  s  o  i  l  .  c                     **/
/**                                                                       **/
/**     Converts ASCII soil data file into binary file used by LPJmL      **/
/**     ASCII file has to be in the following format:                     **/
/**                                                                       **/
/**     Line 1                                                            **/
/**     ...                                                               **/
/**     Line 12                                                           **/
/**     lon1,lat1,soilcode1                                               **/ 
/**     lon2,lat2,soilcode2                                               **/ 
/**     ...                                                               **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  30.03.2012                                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define NSLICE 70000
#define NO_DATA -9999
#define USAGE "Usage: %s soilfile.txt coordfile.bin soilfile.bin\n"

typedef struct
{
  Real lon,lat;
  int soilcode;
} Soildata;

int main(int argc,char **argv)
{
  FILE *file;
  Coordfile coordfile;
  Filename filename;
  Coord resol={0.5,0.5};
  Coord *coords;
  int i,j,n,ncoord,nvalid,nsoil;
  char soilcode;
  String line;
  Soildata *soildata;
  if(argc!=4)
  {
    fprintf(stderr,"Invalid number of arguments\n"
                   USAGE,argv[0]);
    return EXIT_FAILURE;
  }
  file=fopen(argv[1],"r");
  if(file==NULL)
  {
    fprintf(stderr,"Error opening file '%s': %s\n",argv[1],strerror(errno));
    return EXIT_FAILURE;
  }
  /* skip first 12 lines in ASCII soildata file */
  for(i=0;i<12;i++)
   fgets(line,STRING_LEN,file); 
  n=0;
  soildata=newvec(Soildata,NSLICE);
  if(soildata==NULL)
  {
    fprintf(stderr,"Error allocating soil data.\n");
    return EXIT_FAILURE;
  }
  while(fscanf(file,"%lf,%lf,%d",&soildata[n].lon,&soildata[n].lat,&soildata[n].soilcode)==3)
  {
    if(soildata[n].soilcode!=-9999)
    {
      n++;
      if(n % NSLICE==0)
      {
        soildata=(Soildata *)realloc(soildata,(n+NSLICE)*sizeof(Soildata));
        if(soildata==NULL)
        {
          fprintf(stderr,"Error allocating soil data.\n");
          return EXIT_FAILURE;
        }
      }
    }
  } /* of 'while' */
  printf("Data read: %d\n",n);
  fclose(file);
  filename.fmt=CLM;
  filename.name=argv[2];
  coordfile=opencoord(&filename);
  if(coordfile==NULL)
  {
    fprintf(stderr,"Error opening file '%s': %s\n",argv[2],strerror(errno));
    return EXIT_FAILURE;
  }
  ncoord=numcoord(coordfile);
  coords=newvec(Coord,ncoord);
  if(coords==NULL)
  {
    fprintf(stderr,"Error allocating coords.\n");
    return EXIT_FAILURE;
  }
  for(i=0;i<ncoord;i++)
    if(readcoord(coordfile,coords+i,resol))
    {
      fprintf(stderr,"Error reading coord %d in '%s'.\n",i,argv[2]);
      return EXIT_FAILURE;
    }

  closecoord(coordfile);
  file=fopen(argv[3],"wb");
  if(file==NULL)
  {
    fprintf(stderr,"Error creating file '%s': %s\n",argv[3],strerror(errno));
    return EXIT_FAILURE;
  }
  nvalid=nsoil=0;
  for(i=0;i<ncoord;i++)
  {
    soilcode=0;
    for(j=0;j<n;j++)
      if(coords[i].lon==soildata[j].lon && coords[i].lat==soildata[j].lat)
      {
        /*printf("%.2f %.2f %d\n",coords[i].lon,coords[i].lat,soildata[j].soilcode); */
        soilcode=(char)soildata[j].soilcode;
        if(soilcode>0)
          nvalid++;
        if(soilcode>nsoil)
          nsoil=soilcode;
        break;
      }
    fwrite(&soilcode,1,1,file);
    if(j==n)
      fprintf(stderr,"Soil data not found for cell (%.2f, %.2f)\n",
              coords[i].lon,coords[i].lat);
  }
  printf("Valid soil codes:     %d\n"
         "Number of soil codes: %d\n",nvalid,nsoil);
  fclose(file);
  return EXIT_SUCCESS;
} /* of 'main' */ 
