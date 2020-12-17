/***************************************************************************/
/**                                                                       **/
/**     m  i  x  _  v  e  g  _  g  r  a  s  s  .  c                       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 27.09.2004                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"

void mix_veg_grass(Pft *pft,Real scaler)
{
  Pftgrass *grass;
  grass=pft->data;
  
  grass->ind.leaf*=scaler;
  grass->ind.root*=scaler;
  grass->turn.root*=scaler;
  grass->turn.leaf*=scaler;

  pft->bm_inc*=scaler;
  
} /* of 'mix_veg_grass' */
