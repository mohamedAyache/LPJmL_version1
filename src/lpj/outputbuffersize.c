/***************************************************************************/
/**                                                                       **/
/**      o  u  t  p  u  t  b  u  f  f  e  r  s  i  z  e  .  c             **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function calculates memory size for output buffers.               **/
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

long long outputbuffersize(int npft,      /* number of natural pfts */
        int ncft,      /* number of crop pfts */
        const Config *config /* LPJ configuration */
        )               /* returns size of buffer in bytes */
{
    long long sum,size;
    int i;
    size=(long long)config->count*(config->lastyear-config->firstyear+1);
    sum=0;
    for(i=0;i<config->n_out;i++)
        if(config->outputvars[i].id>=D_LAI && config->outputvars[i].id<=D_PET)
            sum+=size*NDAYYEAR*sizeof(float);
        else
            switch(config->outputvars[i].id)
            {
                case FIREC: case FLUX_FIREWOOD: case MAXTHAW_DEPTH: case FIREF:
                case FLUX_ESTAB: case HARVEST: case VEGC: case MG_VEGC:
                case SOILC: case MG_SOILC: case LITC: case MG_LITC: case APREC:
                case INPUT_LAKE: case PROD_TURNOVER: case DEFOREST_EMIS:
                case AFRAC_WD_UNSUST: case ACONV_LOSS_EVAP: case ACONV_LOSS_DRAIN: case SOILC_SLOW: case AWATERUSE_HIL:
                    sum+=size*sizeof(float);
                    break;
                case SOILC_LAYER:
                    sum+=size*LASTLAYER*sizeof(float);
                    break;
                case ADISCHARGE:
                    sum+=(long long)config->ngridcell*(config->lastyear-config->firstyear+1)*sizeof(float);
                    break;
                case SDATE: case HDATE:
                    sum+=size*ncft*2*sizeof(short);
                    break;
                case CFT_IRRIG_EVENTS:
                    sum+=size*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)*2*sizeof(short);
                    break;
                case PFT_NPP: case PFT_GCGP:
                    sum+=sizeof(float)*size*((npft-config->nbiomass-config->nagtree)+(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)*2);
                    break;
                case CFT_PET:case GROWING_PERIOD:case CFT_TEMP:case CFT_PREC:
                case CFT_SRAD:case CFT_ABOVEGBM:
                    sum+=sizeof(float)*size*(ncft+NGRASS)*2;
                    break;
                case PFT_HARVEST: case PFT_RHARVEST: case CFT_CONSUMP_WATER_G:
                case CFT_CONSUMP_WATER_B: case CFTFRAC:case CFT_TRANSP: case CFT_TRANSP_B: case CFT_EVAP: case CFT_EVAP_B:
                case CFT_INTERC: case CFT_INTERC_B: case CFT_RETURN_FLOW_B: case CFT_NIR: case LUC_IMAGE: case CFT_FPAR: case CFT_AIRRIG: case CFT_CONV_LOSS_EVAP: case CFT_CONV_LOSS_DRAIN:
                    sum+=sizeof(float)*size*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree)*2;
                    break;
                case FPC:
                    sum+=sizeof(float)*size*((npft-config->nbiomass-config->nagtree)+1);
                    break;
                case MNPP: case MRH: case MFPAR: case MRUNOFF: case MDISCHARGE:
                case MTRANSP: case MTRANSP_B: case MEVAP: case MEVAP_B: case MINTERC: case MINTERC_B: case MPET:
                case MSWC1: case MSWC2: case MIRRIG: case MWD_UNSUST: case MUNMET_DEMAND:
                case MRETURN_FLOW_B: case MEVAP_LAKE: case MGPP: case MEVAP_RES:
                case MPREC_RES: case MFIREC: case MNFIRE: case MFIREDI:
                case MFIREEMISSION: case MBURNTAREA: case MSOILTEMP1: case MSOILTEMP2:
                case MSOILTEMP3: case MSOILTEMP4: case MSOILTEMP5: case MSOILTEMP6:
                case MTEMP_IMAGE: case MPREC_IMAGE: case MSUN_IMAGE: case MWET_IMAGE:
                case MPREC: case MRAIN: case MSNOWF: case MMELT: case MSWE: case MSNOWRUNOFF:
                case MRUNOFF_SURF: case MRUNOFF_LAT: case MSEEPAGE: case MIRRIG_RW:
                case MSWC3: case MSWC4: case MSWC5: case MWATERAMOUNT:
                case MWD_LOCAL: case MWD_NEIGHB: case MWD_RES: case MWD_RETURN:
                case MCONV_LOSS_EVAP: case MCONV_LOSS_DRAIN: case MRES_DEMAND: case MTARGET_RELEASE: case MRES_STORAGE: case MSTOR_RETURN:
                case MRES_CAP: case MGCONS_RF: case MGCONS_IRR:
                case MROOTMOIST:
                    /* barrier.n: adding sizes of nut outputs */
                case MNO3_DIS: case MPO4_DIS: case MNH4_DIS: case MPHY_DIS:
                case MNO3_SOIL_SUR : case MNO3_SOIL_DEP: case MPO4_SOIL_SUR : case MPO4_SOIL_DEP: case MNH4_SOIL:case MNO3_LAKE: case MPO4_LAKE: case MNH4_LAKE: case MPHY_LAKE:
                case MRESC_SOIL: case MRESP_SOIL: case MRESN_SOIL: case MDECC_SOIL: case MPO4ADS_DIS: case MUPTRESDIFF:
                    sum+=sizeof(float)*NMONTH*size;
                    break;

            } /* of 'switch' */
    return sum;
} /* of 'outputbuffersize' */
