/***************************************************************************/
/**                                                                       **/
/**                     u p d a t e _ f b d _ t r e e .  c                **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Kirsten Thonicke, Ben Poulter                                     **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 12.03.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void update_fbd_tree(Litter* litter,Real fuelbulkdensity,Real carbon,
                     int fuelclass)
{
  Real ratio,fuel;
  if(carbon<=0)
    return; /* no change in fbd, if carbon is lost*/
  fuel=litter_ag_tree(litter,fuelclass);

  if(fuel>carbon)
  {
    ratio = carbon/fuel;
    litter->avg_fbd[fuelclass]=litter->avg_fbd[fuelclass]*(1-ratio)
                               +fuelbulkdensity*ratio;
  }
  else if(fuel>1e-8)
    litter->avg_fbd[fuelclass]=fuelbulkdensity;
  else
    litter->avg_fbd[fuelclass]=0;
  if (litter->avg_fbd[fuelclass] < 0 || litter->avg_fbd[fuelclass] > 30){
    printf("litter->avg_fbd=%.5f carbon=%.5f litter= %.5f fuelclass=%d\n",
           litter->avg_fbd[fuelclass],carbon,fuel,fuelclass);
    fflush(stdout);
  }
} /* of 'update_fbd_tree' */
