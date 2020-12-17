/***************************************************************************/
/**                                                                       **/
/**                c  o  p  y  s  o  i  l  .  c                           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function copies soil variables                                    **/
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

void copysoil(Soil *dst,const Soil *src, int ntotpft)
{
  int i,l,p;
  dst->par=src->par;
  forrootsoillayer(l)
  {
    dst->cpool[l].fast=src->cpool[l].fast;
    dst->cpool[l].slow=src->cpool[l].slow;
    dst->k_mean[l].slow=src->k_mean[l].slow;
    dst->k_mean[l].fast=src->k_mean[l].fast;
    for(p=0;p<ntotpft;p++)
    {
      dst->c_shift_fast[l][p]=src->c_shift_fast[l][p];
      dst->c_shift_slow[l][p]=src->c_shift_slow[l][p];
    }
    // barrier.n: copy of soil nutrients
    dst->NO3[l] = src->NO3[l];
    dst->NH4[l] = src->NH4[l];
    dst->PO4[l] = src->PO4[l];
    dst->PO4adsorbed[l] = src->PO4adsorbed[l];
    dst->DOC[l] = src->DOC[l];
    dst->DOP[l] = src->DOP[l];
    dst->DON[l] = src->DON[l];
    dst->RESC[l] = src->RESC[l];
    dst->RESN[l] = src->RESN[l];
    dst->RESP[l] = src->RESP[l];
    dst->DECC[l] = src->DECC[l];
  }

  dst->YEDOMA=src->YEDOMA;
  dst->litter.decomC=src->litter.decomC;
  dst->alag=src->alag;
  dst->amp=src->amp;
  dst->meanw1=src->meanw1;
  dst->snowpack=src->snowpack;
  dst->maxthaw_depth=src->maxthaw_depth;
  dst->mean_maxthaw=src->mean_maxthaw;
  dst->drainage_class=src->drainage_class;
  dst->decomp_litter_mean=src->decomp_litter_mean;
  freelitter(&dst->litter);
  dst->litter.n=src->litter.n;
  if(src->litter.n>0)
  {
      dst->litter.ag=newvec(Litteritem,src->litter.n);
      check(dst->litter.ag);
      dst->litter.bg=newvec(Real,src->litter.n);
      check(dst->litter.bg);
      for(i=0;i<src->litter.n;i++)
      {
          dst->litter.ag[i]=src->litter.ag[i];
          dst->litter.bg[i]=src->litter.bg[i];
      }
  }
  for(i=0;i<NFUELCLASS+1;i++)
      dst->litter.avg_fbd[i]=src->litter.avg_fbd[i];
  for(i=0;i<NSOILLAYER;i++)
  {
      dst->w[i]=src->w[i];
      dst->w_fw[i]=src->w_fw[i];
      dst->ice_fw[i]=src->ice_fw[i];
      dst->ice_depth[i]=src->ice_depth[i];
      dst->freeze_depth[i]=src->freeze_depth[i];
      dst->ice_pwp[i]=src->ice_pwp[i];
      dst->state[i]=src->state[i];
      dst->layer_exists[i]=src->layer_exists[i];
      dst->perc_energy[i]=src->perc_energy[i];
      dst->micro_heating[i]=src->micro_heating[i];
      dst->decomC[i]=src->decomC[i];
  }
  dst->w_evap=src->w_evap;
  for(i=0;i<NSOILLAYER+1;i++)
      dst->temp[i]=src->temp[i];
  dst->rw_buffer=src->rw_buffer;

} /* of 'copysoil' */
