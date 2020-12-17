/***************************************************************************/
/**                                                                       **/
/**      o u t p u t _ g b w _ a g r i c u l t u t e _ t r e e . c        **/
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
#include "agriculture.h"
#include "agriculture_tree.h"

void output_gbw_agriculture_tree(Output *output,      /* output data */
                                 const Stand *stand,  /* stand pointer */
                                 Real frac_g_evap,
                                 Real evap,           /* evaporation (mm) */
                                 Real evap_blue,      /* evaporation of irrigation water (mm) */
                                 Real return_flow_b,  /* irrigation return flows from surface runoff, lateral runoff and percolation (mm) */
                                 const Real aet_stand[LASTLAYER],
                                 const Real green_transp[LASTLAYER],
                                 Real intercep_stand,  /* stand interception (mm) */
                                 Real intercep_stand_blue, /* stand interception from irrigation (mm) */
                                 int npft,     /* number of natural PFTs */
                                 int nagtree,
                                 int ncft,            /* number of CROPS */
                                 Bool pft_output_scaled
                                )
{
  int l,index;
  Real total_g,total_b;
  Irrigation *data;
  data=stand->data;
  total_g=total_b=0;

  total_g+=intercep_stand-intercep_stand_blue;
  total_b+=intercep_stand_blue;

  total_g+=evap*frac_g_evap;
  total_b+=evap_blue;


 index=agtree(ncft)+stand->pft_id-npft+nagtree+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree);

  forrootsoillayer(l)
  {
    total_g+=green_transp[l];
    total_b+=aet_stand[l]-green_transp[l];
  }
  if(pft_output_scaled)
  {
    output->cft_consump_water_g[index]+=total_g*stand->frac;
    output->cft_consump_water_b[index]+=total_b*stand->frac;
    forrootsoillayer(l)
    {
      output->cft_transp[index]+=aet_stand[l]*stand->frac;
      output->cft_transp_b[index]+=(aet_stand[l]-green_transp[l])*stand->frac;
    }

    output->cft_evap[index]+=evap*stand->frac;
    output->cft_evap_b[index]+=evap_blue*stand->frac;
    output->cft_interc[index]+=intercep_stand*stand->frac;
    output->cft_interc_b[index]+=intercep_stand_blue*stand->frac;
    output->cft_return_flow_b[index]+=return_flow_b*stand->frac;
  }
  else
  {
    output->cft_consump_water_g[index]+=total_g;
    output->cft_consump_water_b[index]+=total_b;
    forrootsoillayer(l)
    {
      output->cft_transp[index]+=aet_stand[l];
      output->cft_transp_b[index]+=aet_stand[l]-green_transp[l];
    }

    output->cft_evap[index]+=evap;
    output->cft_evap_b[index]+=evap_blue;
    output->cft_interc[index]+=intercep_stand;
    output->cft_interc_b[index]+=intercep_stand_blue;
    output->cft_return_flow_b[index]+=return_flow_b;
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
} /* of 'output_gbw_agriculture_tree' */

