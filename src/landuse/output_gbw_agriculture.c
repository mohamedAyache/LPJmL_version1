/***************************************************************************/
/**                                                                       **/
/** o  u  t  p  u  t  _  g  b  w  _  a  g  r  i  c  u  l  t  u  r  e  .  c**/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     written by Stefanie Rost                                          **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "agriculture.h"

void output_gbw_agriculture(Output *output,      /* output data */
                            const Stand *stand,  /* stand pointer */
                            Real frac_g_evap,
                            Real evap,           /* evaporation (mm) */
                            Real evap_blue,      /* evaporation of irrigation water (mm) */
                            Real return_flow_b,  /* irrigation return flows from surface runoff, lateral runoff and percolation (mm) */
                            const Real aet_stand[LASTLAYER],
                            const Real green_transp[LASTLAYER],
                            Real intercep_stand,      /* total stand interception (mm) */
                            Real intercep_stand_blue, /* stand interception from irrigation (mm) */
                            int npft,            /* number of natural PFTs */
                            int nagtree,         /* number of agriculture tree PFTs */
                            int ncft,            /* number of CROPS */
                            Bool pft_output_scaled
                           )
{
  Pft *pft;
  int l,p;
  Real total_g,total_b;
  Irrigation *data;
  data=stand->data;
  total_g=total_b=0;

  total_g+=intercep_stand-intercep_stand_blue;
  total_b+=intercep_stand_blue;

  total_g+=evap*frac_g_evap;
  total_b+=evap_blue;
  forrootsoillayer(l)
  {
    total_g+=green_transp[l];
    total_b+=aet_stand[l]-green_transp[l];
  }
  foreachpft(pft,p,&stand->pftlist)
  {
    if(pft_output_scaled)
    {
      output->cft_consump_water_g[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=total_g*stand->frac;
      output->cft_consump_water_b[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=total_b*stand->frac;
      forrootsoillayer(l)
      {
        output->cft_transp[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=aet_stand[l]*stand->frac;
        output->cft_transp_b[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=(aet_stand[l]-green_transp[l])*stand->frac;
      }
      output->cft_evap[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=evap*stand->frac;
      output->cft_evap_b[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=evap_blue*stand->frac;
      output->cft_interc[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=intercep_stand*stand->frac;
      output->cft_interc_b[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=intercep_stand_blue*stand->frac;
      output->cft_return_flow_b[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=return_flow_b*stand->frac;
    }
    else
    {
      output->cft_consump_water_g[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=total_g;
      output->cft_consump_water_b[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=total_b;
      forrootsoillayer(l)
      {
        output->cft_transp[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=aet_stand[l];
        output->cft_transp_b[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=(aet_stand[l]-green_transp[l]);
      }
      output->cft_evap[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=evap;
      output->cft_evap_b[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=evap_blue;
      output->cft_interc[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=intercep_stand;
      output->cft_interc_b[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=intercep_stand_blue;
      output->cft_return_flow_b[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=return_flow_b;
    }

    if(data->irrigation)
    {
      output->mgcons_irr+=total_g*stand->frac;
      output->mbcons_irr+=total_b*stand->frac;
    }
    else
    {
      output->mgcons_rf+=total_g*stand->frac;
      output->mgcons_rf+=total_b*stand->frac;
    }

  } /* of 'foreachpft' */
} /* of 'output_gbw_agriculture' */
