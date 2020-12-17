/***************************************************************************/
/**                                                                       **/
/**               l i v e f u e l _ c o n s u m _ g r a s s . c           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Kirsten Thonicke                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 11.06.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"

Real livefuel_consum_grass(Litter * UNUSED(litter),Pft *pft,
                           const Fuel * UNUSED(fuel),
                           Livefuel *livefuel,
                           Bool *isdead,Real UNUSED(surface_fi),
                           Real fire_frac)
{

  Pftgrass *grass;
  Real livefuel_consum;
  /*  livegrass consumption */
  grass=pft->data;
  livefuel_consum = fuel_consumption_1hr(livefuel->dlm_livegrass,fire_frac)*grass->ind.leaf*pft->nind;
  /* livegrass update */
  if (pft->nind > 0)
    grass->ind.leaf -= livefuel_consum / pft->nind;
  *isdead=FALSE;
  return livefuel_consum;
} /* of 'livefuel_consum_grass' */
