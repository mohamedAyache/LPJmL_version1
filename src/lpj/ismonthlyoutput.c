/***************************************************************************/
/**                                                                       **/
/**          i  s  m  o  n  t  h  l  y  o  u  t  p  u  t  .  c            **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function determines whether output is monthly data                **/
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

Bool ismonthlyoutput(int index)
{
    switch(index)
    {
        case MNPP: case MRH: case MFPAR: case MRUNOFF: case MDISCHARGE:
        case MTRANSP: case MTRANSP_B: case MWATERAMOUNT: case MROOTMOIST:
        case MEVAP: case MEVAP_B: case MINTERC: case MINTERC_B: case MPET: case MSWC1: case MSWC2: case MIRRIG:
        case MRETURN_FLOW_B: case MEVAP_LAKE: case MGPP: case MEVAP_RES:
        case MPREC_RES: case MFIREC: case MNFIRE: case MFIREDI:
        case MFIREEMISSION: case MBURNTAREA: case MTEMP_IMAGE: case MPREC_IMAGE: case MSUN_IMAGE:
        case MWET_IMAGE: case MSOILTEMP1: case MSOILTEMP2: case MSOILTEMP3:
        case MSOILTEMP4: case MSOILTEMP5: case MSOILTEMP6: case MWD_UNSUST: case MUNMET_DEMAND:
        case MPREC: case MRAIN: case MSNOWF: case MMELT: case MSWE: case MSNOWRUNOFF:
        case MRUNOFF_SURF: case MRUNOFF_LAT: case MSEEPAGE: case MIRRIG_RW:
        case MSWC3: case MSWC4: case MSWC5: case MWD_LOCAL: case MWD_NEIGHB: case MWD_RES:
        case MWD_RETURN: case MCONV_LOSS_EVAP: case MCONV_LOSS_DRAIN: case MRES_STORAGE: case MRES_DEMAND: case MSTOR_RETURN:
        case MTARGET_RELEASE: case MRES_CAP: case MGCONS_RF: case MGCONS_IRR: case MBCONS_IRR:
            /* barrier.n: adding possibilities to save monthly nut variables */
        case MNO3_DIS: case MPO4_DIS: case MNH4_DIS: case MPHY_DIS:
        case MNO3_SOIL_SUR: case MNO3_SOIL_DEP: case MPO4_SOIL_SUR: case MPO4_SOIL_DEP: case MNH4_SOIL: case MDECC_SOIL:
        case MRESN_SOIL: case MRESP_SOIL: case MRESC_SOIL:
        case MNO3_LAKE: case MPO4_LAKE: case MNH4_LAKE: case MPHY_LAKE:case MPO4ADS_DIS: case MUPTRESDIFF:
            return TRUE;
        default:
            return FALSE;
    }
} /* of 'ismonthlyoutput' */
