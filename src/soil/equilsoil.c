/***************************************************************************/
/**                                                                       **/
/**                     e  q  u  i  l  s  o  i  l  . c                    **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     SOIL DECOMPOSITION EQUILIBRIUM CALCULATION                        **/
/**                                                                       **/
/**     Analytical solution of differential flux equations for fast and   **/
/**     slow soil carbon pools.  Implemented after (soil_equil_year)      **/
/**     simulation years, when annual litter inputs should be close to    **/
/**     equilibrium.  Assumes average climate (temperature and soil       **/
/**     moisture) from all years up to soil_equil_year.                   **/
/**                                                                       **/
/**       Rate of change of soil pool size = litter input - decomposition **/
/**         (5) dc/dt = litter_decom - kc                                 **/
/**       At equilibrium,                                                 **/
/**         (6) dc/dt = 0                                                 **/
/**       From (5) & (6),                                                 **/
/**         (7) c = litter_decom / k                                      **/
/**                                                                       **/
/**     written by Sibyll Schaphoff                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 05.06.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void equilsoil(Soil *soil,int ntotpft,const Pftpar pftpar[])
{
  int l,p,f;
  Real sumlitter,pftlitter,wood=0,socfraction;
  Pool *cpool,*sum_cpool;
  Pool *equilc[LASTLAYER],equilc_tot[LASTLAYER],*equil_shift[LASTLAYER],*sum_shift;
  Pool *equilc_pft;
  Pool *k_mean,*c0;
  Pool *sum;

  cpool=newvec(Pool,ntotpft);
  check(cpool);
  sum_cpool=newvec(Pool,ntotpft);
  check(sum_cpool);
  sum_shift=newvec(Pool,ntotpft);
  check(sum_shift);
  equilc_pft=newvec(Pool,ntotpft);
  check(equilc_pft);
  k_mean=newvec(Pool,ntotpft);
  check(k_mean);
  c0=newvec(Pool,ntotpft);
  check(c0);
  sum=newvec(Pool,ntotpft);
  check(sum);
 
  for(l=0;l<LASTLAYER;l++)
  {
    equilc[l]=newvec(Pool,ntotpft);
    check(equilc[l]);
    equil_shift[l]=newvec(Pool,ntotpft);
    check(equil_shift[l]);
  }

  sumlitter=littersum(&soil->litter);
  soil->decomp_litter_mean/=(soil_equil_year-veg_equil_year);
  forrootsoillayer(l)
  {
     soil->k_mean[l].fast/=(soil_equil_year-veg_equil_year);
     soil->k_mean[l].slow/=(soil_equil_year-veg_equil_year);
  }
  for(p=0;p<ntotpft;p++)
	cpool[p].slow=cpool[p].fast=sum_shift[p].fast=sum_shift[p].slow=
	equilc_pft[p].slow=equilc_pft[p].fast=k_mean[p].fast=k_mean[p].slow=sum[p].fast=sum[p].slow=0.0;
  forrootsoillayer(l)
    for(p=0;p<ntotpft;p++)
    {
       socfraction=pow(10,pftpar[p].soc_k*logmidlayer[l])
                 - (l>0 ? pow(10,pftpar[p].soc_k*logmidlayer[l-1]): 0);
       k_mean[p].fast+=soil->k_mean[l].fast*socfraction;
       k_mean[p].slow+=soil->k_mean[l].slow*socfraction;
    }
  for(p=0;p<ntotpft;p++)
  {
    c0[p].fast=k_mean[p].fast>epsilon ? (1-param.atmfrac)*param.fastfrac*(soil->decomp_litter_mean)/k_mean[p].fast : 0;
    c0[p].slow=k_mean[p].slow>epsilon ? (1-param.atmfrac)*(1.0-param.fastfrac)*(soil->decomp_litter_mean)/k_mean[p].slow : 0;
  }

  if(soil->decomp_litter_mean>epsilon)
  {
    forrootsoillayer(l)
    {
      soil->cpool[l].slow=soil->cpool[l].fast=0;
      equilc_tot[l].fast=soil->k_mean[l].fast>epsilon ? (1-param.atmfrac)*param.fastfrac*soil->decomp_litter_mean/soil->k_mean[l].fast : 0;
      equilc_tot[l].slow=soil->k_mean[l].slow>epsilon ? (1-param.atmfrac)*(1.0-param.fastfrac)*soil->decomp_litter_mean/soil->k_mean[l].slow : 0;
      /*  equilc_tot[l].fast=soil->k_mean[l].fast>0 ? soilfrac*fastfrac*(soil->decomp_litter_mean)/
                     ((1.0-exp(-soil->k_mean[l].fast))) : 0;
      equilc_tot[l].slow=soil->k_mean[l].slow>0 ? soilfrac*(1.0-fastfrac)*(soil->decomp_litter_mean)/
                   ((1.0-exp(-soil->k_mean[l].slow))) : 0;*/


      for(p=0;p<ntotpft;p++)
      {
        socfraction=pow(10,pftpar[p].soc_k*logmidlayer[l])
                - (l>0 ? pow(10,pftpar[p].soc_k*logmidlayer[l-1]): 0);
        equil_shift[l][p].slow=soil->k_mean[l].slow*socfraction;
        equil_shift[l][p].fast=soil->k_mean[l].fast*socfraction;
        sum_shift[p].fast+=equil_shift[l][p].fast;
        sum_shift[p].slow+=equil_shift[l][p].slow;
/*          equilc[l][p].fast=equilc_tot[l].fast*socfraction;
          equilc[l][p].slow=equilc_tot[l].slow*socfraction;
          equilc_pft[p].fast+=equilc[l][p].fast;
          equilc_pft[p].slow+=equilc[l][p].slow;*/
      }
    }
    forrootsoillayer(l)
    {
/*	  for(p=0;p<ntotpft;p++){
	    soil->c_shift_fast[l][p]=equilc[l][p].fast/equilc_pft[p].fast;
	    soil->c_shift_slow[l][p]=equilc[l][p].slow/equilc_pft[p].slow;
	    soil->c_shift_fast[l][p]=equil_shift[l][p].fast/sum_shift[p].fast;
            soil->c_shift_slow[l][p]=equil_shift[l][p].slow/sum_shift[p].slow;

	  }*/
      for(p=0;p<soil->litter.n;p++)
      {
        wood=0;
        for(f=0;f<NFUELCLASS;f++)
          wood+=soil->litter.ag[p].trait.wood[f];
        pftlitter=soil->litter.bg[p]+soil->litter.ag[p].trait.leaf+wood;
        socfraction=pow(10,soil->litter.ag[p].pft->soc_k*logmidlayer[l])
                    - (l>0 ? pow(10,soil->litter.ag[p].pft->soc_k*logmidlayer[l-1]): 0);
/*	    soil->cpool[l].slow+=equilc_tot[l].slow*soil->c_shift_slow[l][soil->litter.ag[p].pft->id]*pftlitter/(sumlitter>epsilon?sumlitter:1);
            soil->cpool[l].fast+=equilc_tot[l].fast*soil->c_shift_fast[l][soil->litter.ag[p].pft->id]*pftlitter/(sumlitter>epsilon?sumlitter:1);
 */         soil->cpool[l].slow+=c0[soil->litter.ag[p].pft->id].slow*socfraction*pftlitter/sumlitter;
            soil->cpool[l].fast+=c0[soil->litter.ag[p].pft->id].fast*socfraction*pftlitter/sumlitter;
      }
    }
    forrootsoillayer(l)
    {
      for(p=0;p<ntotpft;p++)
      {
        socfraction=pow(10,pftpar[p].soc_k*logmidlayer[l])
                  - (l>0 ? pow(10,pftpar[p].soc_k*logmidlayer[l-1]): 0);
        soil->c_shift_fast[l][p]=(c0[p].fast*socfraction-c0[p].fast*socfraction*(1-soil->k_mean[l].fast))/((1-param.atmfrac)*param.fastfrac*(soil->decomp_litter_mean));
        soil->c_shift_slow[l][p]=(c0[p].slow*socfraction-c0[p].slow*socfraction*(1-soil->k_mean[l].slow))/((1-param.atmfrac)*(1.0-param.fastfrac)*(soil->decomp_litter_mean));
 /*           fprintf(stdout,"equilsoil socfraction=%.5f soil_c_shift[%d][%d].fast=%.5f soil_c_shift[%d][%d].slow=%.5f\n",socfraction,l,p,soil->c_shift_fast[l][p],l,p,soil->c_shift_slow[l][p]);*/
        sum[p].fast+=soil->c_shift_fast[l][p];
        sum[p].slow+=soil->c_shift_slow[l][p];
      }

      soil->k_mean[l].slow=soil->k_mean[l].fast=0.0;
    }
    for(p=0;p<ntotpft;p++)
    {
      if(sum[p].fast<1.0 && sum[p].fast>0.0)
        for (l=0;l<LASTLAYER;l++) soil->c_shift_fast[l][p]=soil->c_shift_fast[l][p]/sum[p].fast;
      else if (sum[p].fast<epsilon)
      {
        soil->c_shift_fast[0][p]=1.0;
        for (l=1;l<LASTLAYER;l++) soil->c_shift_fast[l][p]=0;
      }
      if(sum[p].slow<1.0 && sum[p].slow>0.0)
        for (l=0;l<LASTLAYER;l++) soil->c_shift_slow[l][p]=soil->c_shift_slow[l][p]/sum[p].slow;
      else if (sum[p].slow<epsilon)
      {
        soil->c_shift_slow[0][p]=1.0;
        for (l=1;l<LASTLAYER;l++) soil->c_shift_slow[l][p]=0;
      }
    }
    soil->count=0;
    soil->decomp_litter_mean=0.0;
  }
  /* freeing memory */
  free(cpool);
  free(sum_cpool);
  free(sum_shift);
  free(equilc_pft);
  free(k_mean);
  free(c0);
  free(sum);
 
  for(l=0;l<LASTLAYER;l++)
  {
    free(equilc[l]);
    free(equil_shift[l]);
  }

} /* of 'equilsoil' */
