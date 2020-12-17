/***************************************************************************/
/**                                                                       **/
/**        i  n  i  t  o  u  t  p  u  t  _  m  o  n  t  h  l  y  .  c     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function initializes monthly output data to zero                  **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-06-19 15:16:31 +0200 (ven., 19 juin 2#$ **/
/**     By         : $Author:: jonasjae                        $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void initoutput_monthly(Output *output)
{
  int l;

  output->mnpp=output->mgpp=output->mrh=output->mfpar=output->mtransp=output->mtransp_b=
  output->mrunoff=output->mpet=output->mwateramount=
  output->mdischarge=output->mevap=output->mevap_b=output->minterc=output->minterc_b=output->mirrig=output->mwd_unsustainable=
  output->mevap_lake=output->mreturn_flow_b=output->mevap_res=output->munmet_demand=
  output->mprec_res=output->mnfire=output->mfireemission=
  output->mburntarea=
  output->mfirec=output->mfiredi=output->mtemp_image=output->mprec_image=
  output->msun_image=output->mwet_image=output->msoiltemp1=output->msoiltemp2=output->msoiltemp3=output->msoiltemp4=
  output->msoiltemp5=output->msoiltemp6=output->mres_storage=
  output->mres_demand=output->mtarget_release=output->mres_cap=
  output->mwd_local=output->mwd_neighb=output->mwd_res=output->mwd_return=
  output->mconv_loss_evap=output->mconv_loss_drain=output->mprec=output->mrain=output->msnowf=output->mmelt=output->msnowrunoff=output->mswe=output->mstor_return=0.0;
  output->mrunoff_lat=output->mrunoff_surf=output->mseepage=output->mrootmoist=0.0;
  output->mgcons_rf=output->mgcons_irr=output->mbcons_irr=output->mirrig_rw=0.0;
  output->mPhytoCdischarge=output->mNO3discharge=output->mPO4discharge=output->mPO4adsdischarge=output->mNH4discharge=0.0;
  output->mNWastewater=output->mPO4Wastewater=output->mNferti=output->mPO4ferti=output->mNmanure=output->mPO4manure=0.0;
  for(l=0;l<NSOILLAYER;l++){
    output->mswc[l]=0;
    output->mRESC_SOIL[l]=0;
    output->mRESN_SOIL[l]=0;
    output->mRESP_SOIL[l]=0;
    output->mDECC_SOIL[l]=0;
    output->mNO3_SOIL[l]=0;
    output->mPO4_SOIL[l]=0;
    output->mNH4_SOIL[l]=0;
  }
  output->mNO3_LAKE=0;
  output->mNH4_LAKE=0;
  output->mPO4_LAKE=0;
  output->mPHY_LAKE=0;
} /* of 'initoutput_monthly' */
