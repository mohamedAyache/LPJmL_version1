/***************************************************************************/
/**                                                                       **/
/**                     c  o  o  r  d  .  c                               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Implementation of datatype Coord                                  **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-10-08 16:30:26 +0200 (mer., 08 oct. 2#$ **/
/**     By         : $Author:: sibylls                         $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include <sys/stat.h>

struct coordfile
{
  FILE *file;  /* file pointer */
  int first;   /* index of first coordinate */
  int n;       /* number of coordinates in file */
  int version;
  float cellsize;
  float scalar;
  Bool swap;   /* byte order has to be changed */
  int fmt;     /* file format (RAW/CLM) */
};

Coordfile opencoord(const Filename *filename /* filename of coord file */
                   )                         /* returns open coord file
                                                or NULL on error       */
{
  Coordfile coordfile;
  Header header;
  struct stat filestat;
  coordfile=new(struct coordfile);
  if(coordfile==NULL)
    return NULL;
  coordfile->file=fopen(filename->name,"rb");

  if(coordfile->file==NULL)
  {
    free(coordfile);
    return NULL;
  }
  if(filename->fmt==RAW)
  {
    fstat(fileno(coordfile->file),&filestat);
    coordfile->n=filestat.st_size/sizeof(Intcoord);
    coordfile->first=0;
    coordfile->swap=FALSE;
    coordfile->version=1;
    coordfile->scalar=0.01;
    coordfile->cellsize=0.5;
  }
  else
  {
    if(filename->fmt==CLM)
      coordfile->version=READ_VERSION;
    else
      coordfile->version=2;
    if(freadheader(coordfile->file,&header,&coordfile->swap,
                   LPJGRID_HEADER,&coordfile->version))
    {
      fclose(coordfile->file);
      free(coordfile);
      return NULL;
    }
    coordfile->n=header.ncell;
    coordfile->first=header.firstcell;
    coordfile->cellsize=header.cellsize;
    coordfile->scalar=(coordfile->version==1) ? 0.01 : header.scalar;
  }
  coordfile->fmt=filename->fmt;
  return coordfile;
} /* of 'opencoord' */

Real getcellsizecoord(const Coordfile coordfile)
{
  return round(coordfile->cellsize*1000)/1000.;
} /* of 'numcoord' */

int numcoord(const Coordfile coordfile)
{
  return round(coordfile->n);
} /* of 'numcoord' */

void closecoord(Coordfile coordfile)
{
  if(coordfile!=NULL)
  {
    fclose(coordfile->file);
    free(coordfile);
  }
} /* of 'closecoord' */

Bool readintcoord(FILE *file,Intcoord *coord,Bool swap)
{
  if(fread(coord,sizeof(Intcoord),1,file)!=1)
    return TRUE;
  if(swap)
  {
    /* data is in different byte order */
    coord->lat=swapshort(coord->lat);
    coord->lon=swapshort(coord->lon);
  }
  return FALSE;
} /* of 'readintcoord' */

Bool readcoord(Coordfile coordfile,
               Coord *coord,         /* cell coordinate read from file */
               Coord resol           /* resolution (deg) */
              )                      /* returns FALSE for successful read */
{
  Intcoord icoord;
  if(readintcoord(coordfile->file,&icoord,coordfile->swap))
    return TRUE;
  coord->lat=icoord.lat*coordfile->scalar;
  coord->lon=icoord.lon*coordfile->scalar;
  if(resol.lat==0.25)
  {
    if(coord->lat>=0)
      coord->lat+=0.005;
    else
      coord->lat-=0.005;
  }
  if(resol.lon==0.25)
  {
    if(coord->lon>=0)
      coord->lon+=0.005;
    else
      coord->lon-=0.005;
  }
  /* calculate cell area */
  coord->area=cellarea(*coord,resol);
  return FALSE;
} /* of 'readcoord' */

Bool writecoord(FILE *file,        /* file pointer */
                const Coord coord /* cell coordinate written to file */
               )                   /* returns FALSE for successful write */
{
  Intcoord icoord;
  icoord.lat=(short)(coord.lat*100);
  icoord.lon=(short)(coord.lon*100);
  return fwrite(&icoord,sizeof(icoord),1,file)!=1;
} /* of 'writecoord' */

int seekcoord(Coordfile coordfile, /* file pointer */
              int pos              /* position in file */
             )                     /* returns return code of fseek */
{
  return fseek(coordfile->file,
               (coordfile->fmt==RAW) ? pos*sizeof(Intcoord) :
               (pos-coordfile->first)*sizeof(Intcoord)+headersize(LPJGRID_HEADER,coordfile->version),
               SEEK_SET);
} /* of 'seekcoord' */

Real cellarea(Coord coord, /* cell coordinate */
              Coord resol  /* resolution (deg) */
             )             /* returns area of cell (m^2) */
{
  return (111194.9*resol.lat)*(111194.9*resol.lon)*cos(deg2rad(coord.lat));
} /* of 'cellarea' */

Bool fscancoord(FILE *file,   /* File pointer of text file */
                Coord *coord,
                Bool isout
               )              /* returns TRUE on error */
{
  if(fscanreal(file,&coord->lon,"longitude",isout))
    return TRUE;
  if(fscanreal(file,&coord->lat,"latitude",isout))
    return TRUE;
  return FALSE;
} /* of 'fscancoord' */

int findcoord(Coord c,const Coord array[],int size)
{
  int i;
  for(i=0;i<size;i++)
    if(array[i].lon==c.lon && array[i].lat==c.lat)
      return i;
  return NOT_FOUND;
} /* of 'findcoord' */

void fprintcoord(FILE *file,const Coord coord)
{
  if(coord.lat<0)
    fprintf(file,"%.3fS",-coord.lat);
  else
    fprintf(file,"%.3fN",coord.lat);
  if(coord.lon<0)
    fprintf(file," %.3fW",-coord.lon);
  else
    fprintf(file," %.3fE",coord.lon);
} /* of 'fprintcoord' */
