/***************************************************************************/
/**                                                                       **/
/**                   f  r  e  e  s  o  i  l  .  c                        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function deallocates memory for soil                              **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 15.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void freesoil(Soil *soil)
{
  int l;
  freelitter(&soil->litter);
  for (l=0;l<LASTLAYER;l++)
  {
    free(soil->c_shift_fast[l]);
    free(soil->c_shift_slow[l]);
  }
} /* of 'freesoil' */
