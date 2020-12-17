/***************************************************************************/
/**                                                                       **/
/**                o  u  t  p  u  t  s  i  z  e  .  c                     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function calculates number of items per cell in output file       **/
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

int outputsize(int index,    /* output index */
               int npft,     /* number of natural PFTs */
               int nbiomass, /* number of biomass PFTs */
               int nagtree,  /* number of agriculture tree PFTs */
               int ncft      /* number of crop PFTs */
              )              /* returns number of items per cell */
{
  switch(index)
  {
    case SDATE: case HDATE:
      return ncft*2;
    case PFT_NPP: case PFT_GCGP:
      return npft-nbiomass-nagtree+(ncft+NGRASS+NBIOMASSTYPE+nagtree)*2;
    case PFT_HARVEST: case PFT_RHARVEST: case CFT_CONSUMP_WATER_G: case CFT_CONV_LOSS_EVAP: case CFT_CONV_LOSS_DRAIN:
    case CFT_CONSUMP_WATER_B: case CFTFRAC: case CFT_AIRRIG: case CFT_FPAR: case CFT_RETURN_FLOW_B:
    case LUC_IMAGE: case CFT_INTERC: case CFT_INTERC_B: case CFT_NIR: case CFT_TRANSP: case CFT_TRANSP_B:
    case CFT_EVAP: case CFT_EVAP_B: case CFT_IRRIG_EVENTS:
      return (ncft+NGRASS+NBIOMASSTYPE+nagtree)*2;
    case FPC:
      return npft-nbiomass-nagtree+1;
    case SOILC_LAYER:
      return LASTLAYER;
    case CFT_PET:case GROWING_PERIOD:case CFT_TEMP:case CFT_PREC:
    case CFT_SRAD: case CFT_ABOVEGBM:
      return (ncft+NGRASS)*2;
    default:
      return 1;
  } /* of 'switch' */
} /* of 'outputsize' */
