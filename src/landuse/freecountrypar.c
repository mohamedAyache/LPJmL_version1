/***************************************************************************/
/**                                                                       **/
/**               f  r  e  e  c  o  u  n  t  r  y  p  a  r  .  c          **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-23 18:57:23 +0200 (jeu., 23 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void freecountrypar(Countrypar countrypar[],int ncountries)
{
  int i;
  for(i=0;i<ncountries;i++)
  {
    free(countrypar[i].name);
    free(countrypar[i].laimax_cft);
    free(countrypar[i].k_est);
  }
  free(countrypar);
} /* of 'freecountrypar' */
