/**************************************************************************/
/**                                                                       **/
/**                     g  p  _  s  u  m  .  c                            **/
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

Real gp_sum(const Pftlist *pftlist, /* Pft list */
            Real co2,              /* atmospheric CO2 concentration (ppm) */
            Real temp,             /* temperature (deg C) */
            Real par,              /* photosynthetic active radiation flux */
            Real daylength,        /* daylength (h) */
            Real *gp_stand_leafon, /* pot. canopy conduct.at full leaf cover */
            Real gp_pft[],         /* pot. canopy conductance for PFTs & CFTs*/
            Real *fpc              /* total fpc of all Pfts */
           )
{
  int p;
  const Pft *pft;
  Real agd,adtmm,gp,gp_stand,rd,fpc_total;
  *gp_stand_leafon=gp=*fpc=gp_stand=0;
  if(daylength<1e-20)
  {
    foreachpft(pft,p,pftlist)
      gp_pft[getpftpar(pft,id)]=0;
    return 0;
  }
  foreachpft(pft,p,pftlist)
  {
    adtmm=photosynthesis(&agd,&rd,pft->par->path,LAMBDA_OPT,
                         temp_stress(pft->par,temp,daylength),ppm2Pa(co2),
                         temp,par*pft->fpc*alphaa(pft),
                         daylength);
    gp=(1.6*adtmm/(ppm2bar(co2)*(1.0-LAMBDA_OPT)*hour2sec(daylength)))+
                    pft->par->gmin*pft->fpc;
    gp_pft[getpftpar(pft,id)]=gp*pft->phen;
    gp_stand+=gp*pft->phen;
    *gp_stand_leafon+=gp;
                    
    *fpc+=pft->fpc;
  }
  fpc_total=*fpc;
  *gp_stand_leafon= (gp_stand<1e-20 || fpc_total<1e-20) ? 0 : *gp_stand_leafon/fpc_total;
  return (gp_stand<1e-20 || fpc_total<1e-20) ? 0 : gp_stand/fpc_total;
} /* of 'gp_sum' */
