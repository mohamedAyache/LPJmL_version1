/***************************************************************************/
/**                                                                       **/
/**                  w i n d s p e e d _ f p c . c                        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 09.03.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real windspeed_fpc(Real windspeed,const Pftlist *pftlist)
{
  Real windsp_fpc;
  const Pft *pft;
  int p;
  windsp_fpc=0;
  foreachpft(pft,p,pftlist)
    windsp_fpc+=pft->fpc*pft->par->windspeed;
  return windsp_fpc*60*windspeed;
} /* of 'windspeed_fpc' */
