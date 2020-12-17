/***************************************************************************/
/**                                                                       **/
/**                  u p d a t e _ f b d _ g r a s s . c                  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Kirsten Thonicke, Ben Poulter                                     **/
/**     Potsdam Institute for Landuse Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 07.03.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void update_fbd_grass(Litter* litter,Real fuelbulkdensity,Real carbon)
{
  Real ratio,fuel;
  if(carbon<=0)
    return; /* no change in fbd, if carbon is lost */
  fuel=litter_ag_grass(litter);

  if(fuel>carbon)
  {
    ratio = carbon/fuel;
    litter->avg_fbd[NFUELCLASS]=litter->avg_fbd[NFUELCLASS]*(1-ratio)
                                +fuelbulkdensity*ratio;
  }
  else if(fuel>1e-8)
    litter->avg_fbd[NFUELCLASS]=fuelbulkdensity;
  else
    litter->avg_fbd[NFUELCLASS]=0;
  if (litter->avg_fbd[NFUELCLASS] < 0 || litter->avg_fbd[NFUELCLASS] > 30){
    printf("litter->avg_fbd=%.5f carbon=%.5f litter= %.5f fuelclass=%d\n",
           litter->avg_fbd[NFUELCLASS],carbon,fuel,NFUELCLASS);
    fflush(stdout);
  }
} /* of 'update_fbd_grass' */
