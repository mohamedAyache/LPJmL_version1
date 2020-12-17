/***************************************************************************/
/**                                                                       **/
/**         i  n  i  t  o  u  t  p  u  t  _  a  n  n  u  a  l  .  c       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function initializes annual output data to zero                   **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 15:07:59 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void initoutput_annual(Output *output, /* Output data */
                       int npft,       /* number of natural PFTs */
                       int nbiomass,   /* number of biomass PFTs */
                       int nagtree,    /* number of agriculture tree PFTs */
                       int ncft        /* number of crop PFTs */
                      )
{
  int i;
  output->firec=output->firef=output->flux_harvest=output->flux_estab=0;
  output->input_lake=output->flux_firewood=output->flux_rharvest_burnt=output->flux_rharvest_burnt_in_field=0;
  output->atransp=output->aevap=output->ainterc=output->airrig=output->aconv_loss_evap=output->aconv_loss_drain=output->awateruse_hil=0;
  output->awd_unsustainable=output->aevap_lake=output->aevap_res=0;
  output->soil_storage=0;
  output->prod_turnover=output->deforest_emissions=output->fburn=output->ftimber=output->timber_harvest=0;
  output->product_pool_fast=output->product_pool_slow=output->trad_biofuel=0;

  /* memory allocation now in newgrid.c */
  for(i=0;i<(ncft+NGRASS+NBIOMASSTYPE)*2;i++)
    output->cft_luc_image[i]=0;

  for(i=0;i<(ncft+NGRASS+NBIOMASSTYPE+nagtree)*2;i++)
    output->pft_harvest[i].harvest=output->pft_harvest[i].residual=output->cftfrac[i]=
    output->cft_consump_water_g[i]=output->cft_consump_water_b[i]=
    output->cft_transp[i]=output->cft_transp_b[i]=output->cft_evap[i]=output->cft_evap_b[i]=
    output->cft_interc[i]=output->cft_interc_b[i]=output->cft_return_flow_b[i]=output->cft_nir[i]=
    output->cft_airrig[i]=output->cft_fpar[i]=output->cft_conv_loss_evap[i]=output->cft_conv_loss_drain[i]=output->cft_irrig_events[i]=0;
  for(i=0;i<(ncft+NGRASS)*2;i++)
    output->growing_period[i]=output->cft_pet[i]=
    output->cft_temp[i]=output->cft_prec[i]=output->cft_srad[i]=
    output->cft_aboveground_biomass[i]=0;
  for(i=0;i<(ncft*2);i++)
    output->sdate[i]=output->hdate[i]=0;
  for(i=0;i<(ncft+NGRASS+NBIOMASSTYPE+nagtree)*2+npft-nbiomass-nagtree;i++)
  {
    output->pft_npp[i]=0;
    output->pft_gcgp[i]=0;
    output->gcgp_count[i]=0;
  }
  for (i=0; i<npft-nbiomass+1-nagtree;++i)
    output->fpc[i] = 0;
} /* of 'initoutput_annual' */
