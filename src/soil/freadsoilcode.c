/***************************************************************************/
/**                                                                       **/
/**              f  r  e  a  d  s  o  i  l  c  o  d  e  .  c              **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads soil code from file                                **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 11.12.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool freadsoilcode(FILE *file,   /* file pointer */
                   int *soilcode,/* soil code  (0..nsoil) */
                   Bool swap,    /* byte order has to be changed (TRUE/FALSE) */
                   int nsoil     /* Number of soil types */
                  )              /* returns TRUE on error */
{
  int rc;
  Byte soil_8;
  unsigned short soil_16;
  if(nsoil>UCHAR_MAX)
  {
    rc=fread(&soil_16,sizeof(soil_16),1,file);
    *soilcode=(swap) ? swapshort(soil_16) : soil_16;
  }
  else
  {
    rc=fread(&soil_8,sizeof(soil_8),1,file);
    *soilcode=soil_8;
  }
  return (rc!=1);
} /* of 'freadsoilcode */
