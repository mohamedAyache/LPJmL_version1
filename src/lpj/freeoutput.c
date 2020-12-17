/***************************************************************************/
/**                                                                       **/
/**        f  r  e  e  o  u  t  p  u  t  .  c                             **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 26.02.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void freeoutput(Output *output)
{
  free(output->sdate);
  free(output->hdate);
  free(output->pft_harvest);
  free(output->cft_consump_water_g);
  free(output->cft_consump_water_b);
  free(output->growing_period);  
  free(output->pft_npp);
  free(output->pft_gcgp);
  free(output->gcgp_count);
  free(output->fpc);
  free(output->cftfrac);
  /*free(output->cft_consump_water_b);
  free(output->cft_consump_water_g);*/
  free(output->cft_pet);
  free(output->cft_transp);
  free(output->cft_transp_b);
  free(output->cft_evap);
  free(output->cft_evap_b);
  free(output->cft_interc);
  free(output->cft_interc_b);
  free(output->cft_return_flow_b);
  free(output->cft_nir);
  free(output->cft_fpar);
  free(output->cft_temp);
  free(output->cft_prec);
  free(output->cft_srad);
  free(output->cft_aboveground_biomass);
  free(output->cft_airrig);
  free(output->cft_conv_loss_evap);
  free(output->cft_conv_loss_drain);
  free(output->cft_luc_image);
  free(output->cft_irrig_events);
  
  output->sdate=output->hdate=NULL;
  output->pft_harvest=NULL;
  output->cft_consump_water_g=output->cft_consump_water_b=output->cft_pet=NULL;
  output->cft_transp=output->cft_transp_b=output->cft_evap=output->cft_evap_b=output->cft_interc=output->cft_interc_b=
    output->cft_return_flow_b=output->cft_nir=output->cft_temp=output->cft_prec=output->cft_srad=output->cft_fpar=
    output->cft_aboveground_biomass=output->cft_conv_loss_evap=output->cft_conv_loss_drain=NULL;
  output->growing_period=NULL;
  output->cft_irrig_events=NULL;
  output->pft_npp=output->fpc=output->cftfrac=output->cft_airrig=output->cft_luc_image=NULL;
} /* of 'freeoutput' */
