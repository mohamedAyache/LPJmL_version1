/***************************************************************************/
/**                                                                       **/
/**         f  r  e  e  o  u  t  p  u  t  m  o  n  t  h  l  y  .  c       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-06-19 15:14:31 +0200 (ven., 19 juin 2#$ **/
/**     By         : $Author:: jonasjae                        $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void freeoutputmonthly(Outputmonth *output)
{
  free(output->mnpp);
  free(output->mgpp);
  free(output->mrh);
  free(output->mfpar);
  free(output->mrunoff);
  free(output->mdischarge);
  free(output->mwateramount);
  free(output->mtransp);
  free(output->mtransp_b);
  free(output->mevap);
  free(output->mevap_b);
  free(output->minterc);
  free(output->minterc_b);
  free(output->mconv_loss_evap);
  free(output->mconv_loss_drain);
  free(output->mpet);
  free(output->mswc1);
  free(output->mswc2);
  free(output->mswc3);
  free(output->mswc4);
  free(output->mswc5);
  free(output->mirrig);
  free(output->mreturn_flow_b);
  free(output->mwd_local);
  free(output->mwd_neighb);
  free(output->mwd_res);
  free(output->mevap_lake);
  free(output->mevap_res);
  free(output->mprec_res);
  free(output->mfirec);
  free(output->mnfire);
  free(output->mfiredi);
  free(output->mfireemission);
  free(output->mburntarea);
  free(output->mtemp_image);
  free(output->mprec_image);
  free(output->msun_image);
  free(output->mwet_image);
  free(output->msoiltemp1);  
  free(output->msoiltemp2);
  free(output->msoiltemp3);
  free(output->msoiltemp4);
  free(output->msoiltemp5);
  free(output->msoiltemp6);
  free(output->mprec);
  free(output->mrain);
  free(output->msnowf);
  free(output->mswe);
  free(output->mmelt);
  free(output->msnowrunoff);
  free(output->mrunoff_surf);
  free(output->mrunoff_lat);
  free(output->mseepage);
  free(output->mrootmoist);
  free(output->mgcons_rf);
  free(output->mgcons_irr);
  free(output->mbcons_irr);
  free(output->mirrig_rw);
  free(output->munmet_demand);
  free(output);
} /* of 'freeoutputmonthly' */
