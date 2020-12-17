/***************************************************************************/
/**                                                                       **/
/**              s  o  i  l  h  e  a  t  c  a  p  .  c                    **/
/**                                                                       **/
/**     Computation of heat capacity of the upper soil layer taking into  **/
/**     account the amount of water and ice in the soil.                  **/
/**                                                                       **/
/**     written by Roy van den Houdt, Bart Strengers                      **/
/**     MNP/KMD                                                           **/
/**     PO Box 303                                                        **/
/**     3720 AH Bilthoven/The Netherlands                                 **/
/**                                                                       **/
/**     Last change: 30.08.2006                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real soilheatcap(const Soil *soil, /* pointer to soil data */
                 int layer         /* soil layer */
                )                  /* returns heat capacity (J/m2/K) */
{
  Real water_layer, ice_layer, dry_soil_layer; /* Layers in millimeter */
  Real heatcap; /* heat capacity of the soil (J/m2) */
  /* plant available water and water below permanent wilting point*/
  water_layer =soil->par->whcs[layer]*soil->w[layer]+soil->w_fw[layer]+soil->par->wpwps[layer]*(1-soil->ice_pwp[layer]);/*[mm]*/
  ice_layer = soil->ice_depth[layer]+soil->ice_fw[layer]+soil->par->wpwps[layer]*soil->ice_pwp[layer];
  dry_soil_layer=soildepth[layer]-soil->par->wsats[layer];
  heatcap = (c_mineral*dry_soil_layer+c_water*water_layer+c_ice*ice_layer)/1000; /* [J/m2/K] */
  return heatcap;
} /* of 'soilheatcap' */

/* CVS information:*/
#ifdef INCLUDE_RCSID             
static char const rcsid[] = 
    "@(#) $Id: soilheatcap.c 134 2008-12-17 15:41:39Z heinke $"; /*survives gcc optimization*/ 
GENERATE_RCSID(rcsid2,
    "@(#) $Id: soilheatcap.c 134 2008-12-17 15:41:39Z heinke $"); /*survives xlc optimization*/
#endif
