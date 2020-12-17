/***************************************************************************/
/**                                                                       **/
/**                    n  p  p  _  g  r  a  s  s  .  c                    **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 11.06.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"

Real npp_grass(Pft *pft, /* PFT variables */
               Real UNUSED(gtemp_air), /* value of air temperature response function */
               Real gtemp_soil, /* value of soil temperature response function */
               Real assim   /* assimilation (gC/m2) */
             ) /* returns net primary productivity (gC/m2) */
{
  Pftgrass *grass;
  const Pftgrasspar *par;
  Real npp,mresp,gresp;
  grass=pft->data;
  par=pft->par->data;
  mresp=grass->ind.root*pft->nind*par->cn_ratio.root*gtemp_soil*pft->phen;
  gresp=(assim-mresp)*param.r_growth;
  if (gresp<0.0) gresp=0.0;
#ifdef DEBUG3
  printf("mresp=%g gresp=%g assim=%g bminc=%g\n",mresp,gresp,assim,pft->bm_inc);
#endif
  npp=assim-mresp-gresp;
  pft->bm_inc+=npp;
  return npp;
} /* of 'npp_grass' */
