/***************************************************************************/
/**                                                                       **/
/**           n  e  w  o  u  t  p  u  t  m  o  n  t  h  l  y  .  c        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
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

static float *newdata(Outputfile *files,int index,int count)
{
    float *data;
    if(isopen(files,index))
    {
        data=newvec(float,count*NMONTH);
        check(data);
    }
    else
        data=NULL;
    return data;
} /* of 'newdata' */

Outputmonth *newoutputmonthly(Outputfile *files,
        int count)
{
    Outputmonth *output;
    output=new(Outputmonth);
    check(output);
    output->mnpp=newdata(files,MNPP,count);
    output->mgpp=newdata(files,MGPP,count);
    output->mrh=newdata(files,MRH,count);
    output->mfpar=newdata(files,MFPAR,count);
    output->mrunoff=newdata(files,MRUNOFF,count);
    output->mdischarge=newdata(files,MDISCHARGE,count);
    output->mwateramount=newdata(files,MWATERAMOUNT,count);
    output->mtransp=newdata(files,MTRANSP,count);
    output->mtransp_b=newdata(files,MTRANSP_B,count);
    output->mevap=newdata(files,MEVAP,count);
    output->mevap_b=newdata(files,MEVAP_B,count);
    output->minterc=newdata(files,MINTERC,count);
    output->minterc_b=newdata(files,MINTERC_B,count);
    output->mconv_loss_evap=newdata(files,MCONV_LOSS_EVAP,count);
    output->mconv_loss_drain=newdata(files,MCONV_LOSS_DRAIN,count);
    output->mstor_return=newdata(files,MSTOR_RETURN,count);
    output->mpet=newdata(files,MPET,count);
    output->mswc1=newdata(files,MSWC1,count);
    output->mswc2=newdata(files,MSWC2,count);
    output->mswc3=newdata(files,MSWC3,count);
    output->mswc4=newdata(files,MSWC4,count);
    output->mswc5=newdata(files,MSWC5,count);
    output->mirrig=newdata(files,MIRRIG,count);
    output->mreturn_flow_b=newdata(files,MRETURN_FLOW_B,count);
    output->mwd_local=newdata(files,MWD_LOCAL,count);
    output->mwd_neighb=newdata(files,MWD_NEIGHB,count);
    output->mwd_res=newdata(files,MWD_RES,count);
    output->mevap_lake=newdata(files,MEVAP_LAKE,count);
    output->mevap_res=newdata(files,MEVAP_RES,count);
    output->mprec_res=newdata(files,MPREC_RES,count);
    output->mfirec=newdata(files,MFIREC,count);
    output->mnfire=newdata(files,MNFIRE,count);
    output->mfiredi=newdata(files,MFIREDI,count);
    output->mfireemission=newdata(files,MFIREEMISSION,count);
    output->mburntarea=newdata(files,MBURNTAREA,count);
    output->mtemp_image=newdata(files,MTEMP_IMAGE,count);
    output->mprec_image=newdata(files,MPREC_IMAGE,count);
    output->msun_image=newdata(files,MSUN_IMAGE,count);
    output->mwet_image=newdata(files,MWET_IMAGE,count);
    output->msoiltemp1=newdata(files,MSOILTEMP1,count);
    output->msoiltemp2=newdata(files,MSOILTEMP2,count);
    output->msoiltemp3=newdata(files,MSOILTEMP3,count);
    output->msoiltemp4=newdata(files,MSOILTEMP4,count);
    output->msoiltemp5=newdata(files,MSOILTEMP5,count);
    output->msoiltemp6=newdata(files,MSOILTEMP6,count);
    output->mprec=newdata(files,MPREC,count);
    output->mrain=newdata(files,MRAIN,count);
    output->msnowf=newdata(files,MSNOWF,count);
    output->mswe=newdata(files,MSWE,count);
    output->mmelt=newdata(files,MMELT,count);
    output->msnowrunoff=newdata(files,MSNOWRUNOFF,count);
    output->mrunoff_surf=newdata(files,MRUNOFF_SURF,count);
    output->mrunoff_lat=newdata(files,MRUNOFF_LAT,count);
    output->mseepage=newdata(files,MSEEPAGE,count);
    output->mrootmoist=newdata(files,MROOTMOIST,count);
    output->mgcons_rf=newdata(files,MGCONS_RF,count);
    output->mgcons_irr=newdata(files,MGCONS_IRR,count);
    output->mbcons_irr=newdata(files,MBCONS_IRR,count);
    output->mirrig_rw=newdata(files,MIRRIG_RW,count);
    output->munmet_demand=newdata(files,MUNMET_DEMAND,count);
    output->count=0;

    /* barrier.n: initialisation of outputs */
    output->mNO3discharge = newdata(files, MNO3_DIS, count);
    output->mPO4discharge = newdata(files, MPO4_DIS, count);
    output->mPO4adsdischarge = newdata(files, MPO4ADS_DIS, count);
    output->mNH4discharge = newdata(files, MNH4_DIS, count);
    output->mPhytoCdischarge = newdata(files, MPHY_DIS, count);
    output->mNO3_SOIL = newdata(files, MNO3_SOIL_SUR, count);
    output->mPO4_SOIL = newdata(files, MPO4_SOIL_SUR, count);
    output->mNH4_SOIL = newdata(files, MNH4_SOIL, count);
    output->mNO3_LAKE = newdata(files, MNO3_LAKE, count);
    output->mPO4_LAKE = newdata(files, MPO4_LAKE, count);
    output->mNH4_LAKE = newdata(files, MNH4_LAKE, count);
    output->mPHY_LAKE = newdata(files, MPHY_LAKE, count);
    output->mDECC_SOIL = newdata(files, MDECC_SOIL, count);
    output->mRESC_SOIL = newdata(files, MRESC_SOIL, count);
    output->mRESN_SOIL = newdata(files, MRESN_SOIL, count);
    output->mRESP_SOIL = newdata(files, MRESP_SOIL, count);
    output->mUPTRESDIFF = newdata(files, MUPTRESDIFF, count);
    output->mNWastewater = newdata(files,MNWASTEWATER, count);
    output->mPO4Wastewater = newdata(files,MPO4WASTEWATER, count);
    output->mNferti = newdata(files,MNFERTI, count);
    output->mPO4ferti = newdata(files,MPO4FERTI, count);
    output->mNmanure = newdata(files,MNMANURE, count);
    output->mPO4manure = newdata(files,MPO4MANURE, count);

    return output;
} /* of 'newoutputmonthly' */
