/***************************************************************************/
/**                                                                       **/
/**               f  r  e  e  r  e  g  i  o  n  p  a  r  .  c             **/
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

void freeregionpar(Regionpar regionpar[],int nregions)
{
  int i;
  for(i=0;i<nregions;i++)
    free(regionpar[i].name);
  free(regionpar);
} /* of 'freeregionpar' */
