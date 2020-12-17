/***************************************************************************/
/**                                                                       **/
/**                    i  n  i  t  o  u  t  p  u  t  .  c                 **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function allocates output data                                    **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void initoutput(Output *output, /* Output data */
                int cft,        /* CFT for daily output */
                Bool irrigation,/* irrigation for daily output */
                int npft,       /* number of natural PFTs */
                int nbiomass,   /* number of biomass PFTs */
                int nagtree,    /* number of agriculture tree PFTs */
                int ncft        /* number of crop PFTs */
               )
{
  output->sdate=newvec(int,2*ncft);/* allocate memory for output */
  check(output->sdate);
  output->hdate=newvec(int,2*ncft);
  check(output->hdate);
  output->pft_npp=newvec(Real,(npft-nbiomass-nagtree)+2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->pft_npp);
  output->pft_gcgp=newvec(Real,(npft-nbiomass-nagtree)+2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->pft_gcgp);
  output->gcgp_count=newvec(Real,(npft-nbiomass-nagtree)+2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->gcgp_count);
  output->pft_harvest=newvec(Harvest,2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->pft_harvest);
  output->fpc = newvec(Real, (npft-nbiomass-nagtree)+1);
  check(output->fpc);
  output->cftfrac=newvec(Real,2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->cftfrac);
  output->cft_consump_water_g=newvec(Real,2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->cft_consump_water_g);
  output->cft_consump_water_b=newvec(Real,2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->cft_consump_water_b);
  output->growing_period=newvec(Real,2*(ncft+NGRASS));
  check(output->growing_period);
  output->cft_pet=newvec(Real,2*(ncft+NGRASS));
  check(output->cft_pet);
  output->cft_transp=newvec(Real,2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->cft_transp);
  output->cft_irrig_events=newvec(int,2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->cft_irrig_events);
  output->cft_evap=newvec(Real,2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->cft_evap);
  output->cft_transp_b=newvec(Real,2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->cft_transp_b);
  output->cft_interc=newvec(Real,2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->cft_interc);
  output->cft_evap_b=newvec(Real,2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->cft_evap_b);
  output->cft_nir=newvec(Real,2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->cft_nir);
  output->cft_interc_b=newvec(Real,2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->cft_interc_b);
  output->cft_return_flow_b=newvec(Real,2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->cft_return_flow_b);
  output->cft_temp=newvec(Real,2*(ncft+NGRASS));
  check(output->cft_temp);
  output->cft_prec=newvec(Real,2*(ncft+NGRASS));
  check(output->cft_prec);
  output->cft_srad=newvec(Real,2*(ncft+NGRASS));
  check(output->cft_srad);
  output->cft_aboveground_biomass=newvec(Real,2*(ncft+NGRASS));
  check(output->cft_aboveground_biomass);
  output->cft_airrig=newvec(Real,2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->cft_airrig);
  output->cft_fpar=newvec(Real,2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->cft_fpar);
  output->cft_luc_image=newvec(Real,2*(ncft+NGRASS+NBIOMASSTYPE));
  check(output->cft_luc_image);
  output->cft_conv_loss_evap=newvec(Real,2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->cft_conv_loss_evap);
  output->cft_conv_loss_drain=newvec(Real,2*(ncft+NGRASS+NBIOMASSTYPE+nagtree));
  check(output->cft_conv_loss_drain);
  
  initoutput_annual(output, npft, nbiomass,nagtree,ncft);
  output->daily.cft=cft;
  output->daily.irrigation=irrigation;
} /* of 'initoutput' */
