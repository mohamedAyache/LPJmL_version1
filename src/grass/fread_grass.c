/***************************************************************************/
/**                                                                       **/
/**               f  r  e  a  d  _  g  r  a  s  s  .  c                   **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 14.10.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"

Bool fread_grass(FILE *file,Pft *pft,Bool swap)
{
  Pftgrass *grass;
  grass=new(Pftgrass);
  pft->data=grass;
  if(grass==NULL)
  {
      return TRUE;
  }
  fread(grass, sizeof(Pftgrass), 1, file);
  return FALSE;
} /* of 'fread_grass' */
