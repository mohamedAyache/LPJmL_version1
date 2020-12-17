/***************************************************************************/
/**                                                                       **/
/**                    c h e c k _ b a l a n c e .  c                     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Jens Heinke                                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-31 11:06:28 +0200 (ven., 31 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define FAIL /* abort on global water balance error */

void check_balance(Flux flux,           /* global carbon and water fluxes */
                   int year,            /* simulation year (AD) */
                   const Config *config /* LPJ configuration */
                  )
{

  Real balance=0;

  if(config->river_routing)
    balance=flux.prec+flux.wd_unsustainable-flux.evap-flux.transp-flux.interc-flux.evap_lake-flux.evap_res
            -flux.discharge-flux.conv_loss_evap-flux.delta_surface_storage-flux.delta_soil_storage-flux.wateruse;

  /*check total outto irrig from reservoir, totla in to irrig from res hb 10-9-09 */
  /* printf("balance: %.6f total out res: %.6f total in irrig res: %.6f\n",balance,flux.total_reservoir_out,flux.total_irrig_from_reservoir); */

  balance=(flux.area>0) ? balance/flux.area : 0.0;

  if(year>config->firstyear+1 && fabs(balance)>1e-3)
  {
#ifdef FAIL
    fail(INVALID_WATER_BALANCE_ERR,TRUE,
#else
    fprintf(stderr,"ERROR030: "
#endif
    "y: %d GlobW_BALANCE-error: %.5f prec:%.2f wd_unsustainable:%.2f vapour_flux:%.2f discharge:%.2f delta_storage:%.2f\n",
    /*printf("y: %d GlobW_BALANCE-error: %.5f prec:%.2f wd_unsustainable:%.2f vapour_flux:%.2f discharge:%.2f delta_storage:%.2f\n",*/
         year,balance*flux.area,flux.prec,flux.wd_unsustainable,
         (flux.evap+flux.transp+flux.interc+flux.evap_lake+flux.evap_res+flux.conv_loss_evap),
         flux.discharge,(flux.delta_surface_storage+flux.delta_soil_storage));
    fflush(stdout);
  }

} /* of 'check_balance' */
