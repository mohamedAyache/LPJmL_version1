/***************************************************************************/
/**                                                                       **/
/**                               g  p  .  c                              **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function calculates potential canopy conductance at full lead     **/
/**     cover                                                             **/
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

Real gp(Pft *pft,          /* PFT variables */
        Real co2,          /* atmospheric CO2 concentration (ppm) */
        Real temp,         /* temperature (deg C) */
        Real par,          /* photosynthetic active radiation flux */
        Real daylength     /* daylength (h) */
       )                   /* returns pot. canopy conduct. at full leaf cover */
{
  Real agd,adtmm,gp,rd;
  if(daylength==0)
    return 0;
  adtmm=photosynthesis(&agd,&rd,pft->par->path,LAMBDA_OPT,
                       temp_stress(pft->par,temp,daylength),ppm2Pa(co2),
                       temp,par*pft->fpc*pft->par->alphaa,daylength);
  gp=1.6*adtmm/(ppm2bar(co2)*(1.0-LAMBDA_OPT)*hour2sec(daylength));

  return (gp<1e-20 || pft->fpc<1e-20) ? 0 : gp;
} /* of 'gp' */
