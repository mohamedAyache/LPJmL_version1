/***************************************************************************/
/**                                                                       **/
/**                 s  o  i  l  c  o  n  d  u  c  t  .  c                 **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 13.10.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define JOHANSEN
#define K_DRY 0.237 /* assuming a bulk density of 1600 kg/m3, following Farouki (1981)*/
#define K_SOLID 8
#define K_ICE 2.2
#define K_WATER 0.57

Real soilconduct(const Soil *soil, /* pointer to soil data */
                 int layer         /* soil layer */
                )                  /* returns soil conductivity (W/m/K) */
{
  Real conduct,
       k_sat,sat,ke,
       por;
  /* assuming a linear dependence of conductivity on water / ice content beyond 
   * the permanent wilting point and additivity of the water / ice component;
   * see E.D. Yershov: General Geocryology (1998) and
   * G.S. Campbell / J.M. Norman: An Introduction to Environmental Biophysics,
   * 2nd Ed. (1998)for further reference and a more accurate treatment
   */
   por=soil->par->wsats[layer]/soildepth[layer];  /*porosity*/
#ifdef JOHANSEN
   k_sat = pow(K_SOLID,(1-por))*pow(K_ICE,(por*soil->freeze_depth[layer]/soildepth[layer]))
           *pow(K_WATER,(por*(1-soil->freeze_depth[layer]/soildepth[layer])));
   /* saturation */
   sat=(soil->w[layer]*soil->par->whcs[layer]+soil->ice_depth[layer]+soil->par->wpwps[layer]
        +soil->w_fw[layer]+soil->ice_fw[layer])/soil->par->wsats[layer];
   /*Kersten number*/
   ke=(soil->freeze_depth[layer]>epsilon || sat < 0.1) ? sat : log10(sat)+1;
   conduct = (k_sat-soil->par->k_dry[layer])*ke+soil->par->k_dry[layer];
#else
  conduct_w=(soil->w[layer]*soil->par->whcs[layer]+soil->fw[layer]+soil->par->wpwps[layer])
             /(soil->par->wsats[layer])*(soil->par->tcond_100-soil->par->tcond_pwp);
  conduct_ice=(soil->ice_depth[layer]+soil->ice_fw[layer]+soil->par->wpwps[layer])/soil->par->wsats[layer]*(soil->par->tcond_100_ice-soil->par->tcond_pwp);
  conduct = soil->par->tcond_pwp + conduct_w + conduct_ice;
#endif
  return conduct;
} /* of 'soilconduct' */

/* CVS information:*/
#ifdef INCLUDE_RCSID             
static char const rcsid[] = 
    "@(#) $Id: soilconduct.c 120 2008-12-15 14:27:13Z heyder $"; /*survives gcc optimization*/ 
GENERATE_RCSID(rcsid2,
    "@(#) $Id: soilconduct.c 120 2008-12-15 14:27:13Z heyder $"); /*survives xlc optimization*/
#endif
