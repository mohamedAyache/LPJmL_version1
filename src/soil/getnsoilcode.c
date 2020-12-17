/***************************************************************************/
/**                                                                       **/
/**              g  e  t  n  s  o  i  l  c  o  d  e  .  c                 **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function gets number of soil code values in file                  **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 13.09.2011                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

int getnsoilcode(const Filename *filename, /* filename of soil code file */
                 int nsoil,                /* number of soil types */
                 Bool isout                /* error output? (TRUE/FALSE) */
                )                          /* returns number of soil codes */
{
  Coord_netcdf coord;
  int n, version;
  FILE *file;
  Bool swap;
  Header header;
  if(filename->fmt==CDF)
  {
    coord=opencoord_netcdf(filename->name,filename->var,isout);
    if(coord==NULL)
    {
      if(isout)
        fputs("ERROR165: Cannot get number of cells from soil code file.\n",stderr);
      return -1;
    }
    n=numcoord_netcdf(coord);
    closecoord_netcdf(coord);
    return n;
  }
  if(filename->fmt==RAW)
  {
    n=getfilesize(filename->name);
    if(n<0)
    {
      if(isout)
        fprintf(stderr,"ERROR165: Cannot get number of cells from soil code file '%s': %s.\n",
                filename->name,strerror(errno));
      return n;
    }
    if(nsoil<=UCHAR_MAX)
      return n;
    else
      return n/sizeof(short);
  }
  else /* file is in CLM/CLM2 format */
  {
    file=fopen(filename->name,"rb"); /* open CLM file */
    if(file==NULL)
    {
      if(isout)
        printfopenerr(filename->name);
      return -1;
    }
    if(filename->fmt==CLM)
      version=READ_VERSION;
    else
      version=2;
    /* read header */
    if(freadheader(file,&header,&swap,LPJSOIL_HEADER,&version))
    {
      if(isout)
        fprintf(stderr,"ERROR154: Invalid header in '%s'.\n",filename->name);
      fclose(file);
      return -1;
    }
    fclose(file);
    return header.ncell; /* return number of cells in soil code file */
  }
} /* of 'getnsoilcode' */
