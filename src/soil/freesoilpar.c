/***************************************************************************/
/**                                                                       **/
/**               f  r  e  e  s  o  i  l  p  a  r  .  c                   **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 07.03.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void freesoilpar(Soilpar soilpar[],int nsoil)
{
  int i;
  for(i=0;i<nsoil;i++)
    free(soilpar[i].name);
  free(soilpar);
} /* of 'freesoilpar' */
