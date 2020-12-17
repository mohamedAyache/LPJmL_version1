/***************************************************************************/
/**                                                                       **/
/**   u  p  d  a  t  e  _  o  u  t  p  u  t  m  o  n  t  h  l  y  .  c    **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
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

void update_outputmonthly(Outputmonth *output,
        const Output *data,
        int total,
        int month)
{
    if(output->mnpp!=NULL)
        output->mnpp[output->count+month*total]=(float)data->mnpp;
    if(output->mgpp!=NULL)
        output->mgpp[output->count+month*total]=(float)data->mgpp;
    if(output->mrh!=NULL)
        output->mrh[output->count+month*total]=(float)data->mrh;
    if(output->mfpar!=NULL)
        output->mfpar[output->count+month*total]=(float)data->mfpar;
    if(output->mrunoff!=NULL)
        output->mrunoff[output->count+month*total]=(float)data->mrunoff;
    if(output->mdischarge!=NULL)
        output->mdischarge[output->count+month*total]=(float)data->mdischarge;
    if(output->mwateramount!=NULL)
        output->mwateramount[output->count+month*total]=(float)data->mwateramount;
    if(output->mevap!=NULL)
        output->mevap[output->count+month*total]=(float)data->mevap;
    if(output->mevap_b!=NULL)
        output->mevap_b[output->count+month*total]=(float)data->mevap_b;
    if(output->mtransp!=NULL)
        output->mtransp[output->count+month*total]=(float)data->mtransp;
    if(output->mtransp_b!=NULL)
        output->mtransp_b[output->count+month*total]=(float)data->mtransp_b;
    if(output->minterc!=NULL)
        output->minterc[output->count+month*total]=(float)data->minterc;
    if(output->minterc_b!=NULL)
        output->minterc_b[output->count+month*total]=(float)data->minterc_b;
    if(output->mconv_loss_evap!=NULL)
        output->mconv_loss_evap[output->count+month*total]=(float)data->mconv_loss_evap;
    if(output->mconv_loss_drain!=NULL)
        output->mconv_loss_drain[output->count+month*total]=(float)data->mconv_loss_drain;
    if(output->mstor_return!=NULL)
        output->mstor_return[output->count+month*total]=(float)data->mstor_return;
    if(output->mpet!=NULL)
        output->mpet[output->count+month*total]=(float)data->mpet;
    if(output->mswc1!=NULL)
        output->mswc1[output->count+month*total]=(float)data->mswc[0];
    if(output->mswc2!=NULL)
        output->mswc2[output->count+month*total]=(float)data->mswc[1];
    if(output->mswc3!=NULL)
        output->mswc3[output->count+month*total]=(float)data->mswc[2];
    if(output->mswc4!=NULL)
        output->mswc4[output->count+month*total]=(float)data->mswc[3];
    if(output->mswc5!=NULL)
        output->mswc5[output->count+month*total]=(float)data->mswc[4];
    if(output->mrootmoist!=NULL)
        output->mrootmoist[output->count+month*total]=(float)data->mrootmoist;
    if(output->mirrig!=NULL)
        output->mirrig[output->count+month*total]=(float)data->mirrig;
    if(output->mreturn_flow_b!=NULL)
        output->mreturn_flow_b[output->count+month*total]=(float)data->mreturn_flow_b;
    if(output->mwd_local!=NULL)
        output->mwd_local[output->count+month*total]=(float)data->mwd_local;
    if(output->mwd_neighb!=NULL)
        output->mwd_neighb[output->count+month*total]=(float)data->mwd_neighb;
    if(output->mwd_res!=NULL)
        output->mwd_res[output->count+month*total]=(float)data->mwd_res;
    if(output->mevap_lake!=NULL)
        output->mevap_lake[output->count+month*total]=(float)data->mevap_lake;
    if(output->mfirec!=NULL)
        output->mfirec[output->count+month*total]=(float)data->mfirec;
    if(output->mnfire!=NULL)
        output->mnfire[output->count+month*total]=(float)data->mnfire;
    if(output->mfiredi!=NULL)
        output->mfiredi[output->count+month*total]=(float)data->mfiredi;
    if(output->mfireemission!=NULL)
        output->mfireemission[output->count+month*total]=(float)data->mfireemission;
    if(output->msoiltemp1!=NULL)
        output->msoiltemp1[output->count+month*total]=(float)data->msoiltemp1;
    if(output->msoiltemp2!=NULL)
        output->msoiltemp2[output->count+month*total]=(float)data->msoiltemp2;
    if(output->msoiltemp3!=NULL)
        output->msoiltemp3[output->count+month*total]=(float)data->msoiltemp3;
    if(output->msoiltemp4!=NULL)
        output->msoiltemp4[output->count+month*total]=(float)data->msoiltemp4;
    if(output->msoiltemp5!=NULL)
        output->msoiltemp5[output->count+month*total]=(float)data->msoiltemp5;
    if(output->msoiltemp6!=NULL)
        output->msoiltemp6[output->count+month*total]=(float)data->msoiltemp6;
    if(output->mburntarea!=NULL)
        output->mburntarea[output->count+month*total]=(float)data->mburntarea;
    if(output->mtemp_image!=NULL)
        output->mtemp_image[output->count+month*total]=(float)data->mtemp_image;
    if(output->mprec_image!=NULL)
        output->mprec_image[output->count+month*total]=(float)data->mprec_image;
    if(output->mwet_image!=NULL)
        output->mwet_image[output->count+month*total]=(float)data->mwet_image;
    if(output->msun_image!=NULL)
        output->msun_image[output->count+month*total]=(float)data->msun_image;
    if(output->mprec!=NULL)
        output->mprec[output->count+month*total]=(float)data->mprec;
    if(output->msnowf!=NULL)
        output->msnowf[output->count+month*total]=(float)data->msnowf;
    if(output->mrain!=NULL)
        output->mrain[output->count+month*total]=(float)data->mrain;
    if(output->mmelt!=NULL)
        output->mmelt[output->count+month*total]=(float)data->mmelt;
    if(output->msnowrunoff!=NULL)
        output->msnowrunoff[output->count+month*total]=(float)data->msnowrunoff;
    if(output->mswe!=NULL)
        output->mswe[output->count+month*total]=(float)data->mswe;
    if(output->mseepage!=NULL)
        output->mseepage[output->count+month*total]=(float)data->mseepage;
    if(output->mrunoff_surf!=NULL)
        output->mrunoff_surf[output->count+month*total]=(float)data->mrunoff_surf;
    if(output->mrunoff_lat!=NULL)
        output->mrunoff_lat[output->count+month*total]=(float)data->mrunoff_lat;
    if(output->mgcons_rf!=NULL)
        output->mgcons_rf[output->count+month*total]=(float)data->mgcons_rf;
    if(output->mgcons_irr!=NULL)
        output->mgcons_irr[output->count+month*total]=(float)data->mgcons_irr;
    if(output->mbcons_irr!=NULL)
        output->mbcons_irr[output->count+month*total]=(float)data->mbcons_irr;
    if(output->mirrig_rw!=NULL)
        output->mirrig_rw[output->count+month*total]=(float)data->mirrig_rw;
    if(output->munmet_demand!=NULL)
        output->munmet_demand[output->count+month*total]=(float)data->munmet_demand;

    /* barrier.n: adding the saving of nutrients */
    if(output->mNO3discharge!=NULL)
        output->mNO3discharge[output->count+month*total]=(float)data->mNO3discharge;
    if(output->mPO4discharge!=NULL)
        output->mPO4discharge[output->count+month*total]=(float)data->mPO4discharge;
    if(output->mPO4adsdischarge!=NULL)
        output->mPO4adsdischarge[output->count+month*total]=(float)data->mPO4adsdischarge;
    if(output->mNH4discharge!=NULL)
        output->mNH4discharge[output->count+month*total]=(float)data->mNH4discharge;
    if(output->mPhytoCdischarge!=NULL)
        output->mPhytoCdischarge[output->count+month*total]=(float)data->mPhytoCdischarge;
    if(output->mNO3_SOIL!=NULL)
        output->mNO3_SOIL[output->count+month*total]=(float)data->mNO3_SOIL[0];
    if(output->mPO4_SOIL!=NULL)
        output->mPO4_SOIL[output->count+month*total]=(float)data->mPO4_SOIL[0];
    if(output->mNH4_SOIL!=NULL)
        output->mNH4_SOIL[output->count+month*total]=(float)data->mNH4_SOIL[0];
    if(output->mDECC_SOIL!=NULL)
        output->mDECC_SOIL[output->count+month*total]=(float)data->mDECC_SOIL[0];
    if(output->mRESN_SOIL!=NULL)
        output->mRESN_SOIL[output->count+month*total]=(float)data->mRESN_SOIL[0];
    if(output->mRESP_SOIL!=NULL)
        output->mRESP_SOIL[output->count+month*total]=(float)data->mRESP_SOIL[0];
    if(output->mRESC_SOIL!=NULL)
        output->mRESC_SOIL[output->count+month*total]=(float)data->mRESC_SOIL[0];
    if(output->mNO3_LAKE!=NULL)
        output->mNO3_LAKE[output->count+month*total]=(float)data->mNO3_LAKE;
    if(output->mNH4_LAKE!=NULL)
        output->mNH4_LAKE[output->count+month*total]=(float)data->mNH4_LAKE;
    if(output->mPO4_LAKE!=NULL)
        output->mPO4_LAKE[output->count+month*total]=(float)data->mPO4_LAKE;
    if(output->mPHY_LAKE!=NULL)
        output->mPHY_LAKE[output->count+month*total]=(float)data->mPHY_LAKE;
    if(output->mUPTRESDIFF!=NULL)
        output->mUPTRESDIFF[output->count+month*total]=(float)data->mUPTRESDIFF;
    if(output->mNWastewater!=NULL)
        output->mNWastewater[output->count+month*total]=(float)data->mNWastewater;
    if(output->mPO4Wastewater!=NULL)
        output->mPO4Wastewater[output->count+month*total]=(float)data->mPO4Wastewater;
    if(output->mNferti!=NULL)
        output->mNferti[output->count+month*total]=(float)data->mNferti;
    if(output->mPO4ferti!=NULL)
        output->mPO4ferti[output->count+month*total]=(float)data->mPO4ferti;
    if(output->mNmanure!=NULL)
        output->mNmanure[output->count+month*total]=(float)data->mNmanure;
    if(output->mPO4manure!=NULL)
        output->mPO4manure[output->count+month*total]=(float)data->mPO4manure;

} /* of 'update_outputmonthly' */
