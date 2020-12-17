/***************************************************************************/
/**                                                                       **/
/**                    f  i  r  e  p  f  t  .  c                          **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 10.03.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real firepft(Litter *litter,   /* Litter pool */
             Pftlist *pftlist, /* list of established PFTs */
             Real fire_frac    /* fire fraction (0..1) */
            )                  /* returns fire flux (gC/m2) */
{
  int i,p;
  Pft *pft;
  Real flux,flux_litter;
  if(isempty(pftlist)) /*if(pftlist->n==0)*/
    return 0.0;
  flux=flux_litter=0;
  foreachpft(pft,p,pftlist)
  {
    flux+=fire(pft,&fire_frac);
  }
  for(p=0;p<litter->n;p++)
  {
    flux_litter+=litter->ag[p].trait.leaf;
    for(i=0;i<NFUELCLASS;i++)
      flux_litter+=litter->ag[p].trait.wood[i];
    litter->ag[p].trait.leaf*=(1-fire_frac);
    for(i=0;i<NFUELCLASS;i++)
      litter->ag[p].trait.wood[i]*=(1-fire_frac);
  } /* of 'for(p=0;...)' */
  return flux+flux_litter*fire_frac; 
} /* of 'firepft' */
