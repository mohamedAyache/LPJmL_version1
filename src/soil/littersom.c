/***************************************************************************/
/**                                                                       **/
/**                      l  i  t  t  e  r  s  o  m  .  c                  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**               Vertical soil carbon distribution                       **/
/**                                                                       **/
/**               Litter and soil decomposition                           **/
/**                                                                       **/
/**     Calculate daily litter decomposition using equation               **/
/**       (1) dc/dt = -kc     where c=pool size, t=time,                  **/
/**           k=decomposition rate                                        **/
/**     from (1),                                                         **/
/**       (2) c = c0*exp(-kt) where c0=initial pool size                  **/
/**     from (2), decomposition in any month given by                     **/
/**       (3) delta_c = c0 - c0*exp(-k)                                   **/
/**     from (4)                                                          **/
/**       (4) delta_c = c0*(1.0-exp(-k))                                  **/
/**     If LINEAR_DECAY is defined linear version of equations are used:  **/
/**       (3) delta_c = - c0*k                                            **/
/**                                                                       **/
/**     written by Ursula Heyder,Sibyll Schaphoff                         **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 20.02.2010                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define MOIST_DENOM 0.63212055882855767841 /* (1.0-exp(-1.0)) */
#define cpool_low 1e-5
#define K10_YEDOMA 0.025/NDAYYEAR
#define INTERC 0.04021601
#define MOIST_3 -5.00505434
#define MOIST_2 4.26937932
#define MOIST  0.71890122

Real littersom(Soil *soil,
               Real gtemp_soil[NSOILLAYER])
{

  Real response[NSOILLAYER];
  Real decay_litter;
  Pool cflux_soil[LASTLAYER];
  Real decom,decom_sum,soil_cflux,decom_litter;
  Real moist[NSOILLAYER];
  int i,p,l;
  Real yedoma_flux;

  foreachsoillayer(l) response[l]=0.0;
  decom_litter=soil_cflux=yedoma_flux=decom_sum=0.0;

  foreachsoillayer(l)
  {
    if(gtemp_soil[l]>0)
    {
/*    foreachsoillayer(l) moist[l]=(soil->w[l]*soil->par->whcs[l]+soil->ice_depth[l]+soil->par->wpwps[l]+soil->w_fw[l])/soil->par->wsats[l];*/

      if (soil->par->wsats[l]-soil->ice_depth[l]-soil->ice_fw[l]-(soil->par->wpwps[l]*soil->ice_pwp[l])>epsilon)
           moist[l]=(soil->w[l]*soil->par->whcs[l]+(soil->par->wpwps[l]*(1-soil->ice_pwp[l]))+soil->w_fw[l])
                    /(soil->par->wsats[l]-soil->ice_depth[l]-soil->ice_fw[l]-(soil->par->wpwps[l]*soil->ice_pwp[l]));
      else
	       moist[l]=epsilon;
      if (moist[l]<epsilon) moist[l]=epsilon;

/*      response[l]=gtemp_soil[l]*((1.0-exp(-((soil->w[l]*soil->par->whcs[l]+soil->ice_depth[l]+soil->par->wpwps[l])/(soil->par->wfc*soildepth[l]))))/MOIST_DENOM);*/
/*      response[l]=gtemp_soil[l]*((1.0-exp(-((soil->w[l]*soil->par->whcs[l]+soil->ice_depth[l]+soil->par->wpwps[l])/soil->par->wsats[l])))/MOIST_DENOM);*/
      response[l]=gtemp_soil[l]*(INTERC+MOIST_3*(moist[l]*moist[l]*moist[l])+MOIST_2*(moist[l]*moist[l])+MOIST*moist[l]);
/*      response[l]=gtemp_soil[l]*((1.0-exp(-moist[l]))/MOIST_DENOM);*/
      if (response[l]<epsilon)
        response[l]=0.0;
      if (response[l]>1)
        response[l]=1.0;

      if(l<LASTLAYER)
      {
#ifdef LINEAR_DECAY
        cflux_soil[l].slow=soil->cpool[l].slow*param.k_soil10.slow*response[l];
        cflux_soil[l].fast=soil->cpool[l].fast*param.k_soil10.fast*response[l];
#else
        cflux_soil[l].slow=soil->cpool[l].slow*(1.0-exp(-(param.k_soil10.slow*response[l])));
        cflux_soil[l].fast=soil->cpool[l].fast*(1.0-exp(-(param.k_soil10.fast*response[l])));
#endif
        soil->cpool[l].slow-=cflux_soil[l].slow;
        soil->cpool[l].fast-=cflux_soil[l].fast;
        soil_cflux+=cflux_soil[l].slow+cflux_soil[l].fast;
        soil->k_mean[l].fast+=(param.k_soil10.fast*response[l]);
        soil->k_mean[l].slow+=(param.k_soil10.slow*response[l]);
        soil->decomC[l]=cflux_soil[l].slow+cflux_soil[l].fast;
      }
      else
      {
        if (soil->YEDOMA>0.0 && response[l]>0.0)
        {
          yedoma_flux=soil->YEDOMA*(1.0-exp(-(K10_YEDOMA*response[l])));
          soil->YEDOMA-=yedoma_flux;
          soil_cflux+=yedoma_flux;
          soil->decomC[l]+=yedoma_flux;
        }
      }
    }
  } /*end foreachsoillayer*/

  /*
  *       Calculate daily decomposition rates (k, /month) as a function of
  *       temperature and moisture
  *
  */

  if(gtemp_soil[0]>0)
   for(p=0;p<soil->litter.n;p++)
   {
     decom_sum=0;
#ifdef LINEAR_DECAY
     decay_litter=soil->litter.ag[p].pft->k_litter10.leaf*response[0];
#else
     decay_litter=1.0-exp(-(soil->litter.ag[p].pft->k_litter10.leaf*response[0]));
#endif
     decom=soil->litter.ag[p].trait.leaf*decay_litter;
     soil->litter.ag[p].trait.leaf-=decom;
     decom_sum+=decom;
     for(i=0;i<NFUELCLASS;i++)
     {
#ifdef LINEAR_DECAY
       decay_litter=soil->litter.ag[p].pft->k_litter10.wood*response[0];
#else
       decay_litter=1.0-exp(-(soil->litter.ag[p].pft->k_litter10.wood*response[0]));
#endif
       decom=soil->litter.ag[p].trait.wood[i]*decay_litter;
       soil->litter.ag[p].trait.wood[i]-=decom;
       decom_sum+=decom;
     }
#ifdef LINEAR_DECAY
     decay_litter=param.k_litter10*response[0];
#else
     decay_litter=1.0-exp(-(param.k_litter10*response[0]));
#endif
     decom=soil->litter.bg[p]*decay_litter;
     soil->litter.bg[p]-=decom;
     decom_sum+=decom;
     decom_litter+=decom_sum;
     forrootsoillayer(l)
     {
       soil->cpool[l].fast+=param.fastfrac*(1-param.atmfrac)*decom_sum*soil->c_shift_fast[l][soil->litter.ag[p].pft->id];
  	   soil->cpool[l].slow+=(1-param.fastfrac)*(1-param.atmfrac)*decom_sum*soil->c_shift_slow[l][soil->litter.ag[p].pft->id];
     }
   }   /*end soil->litter.n*/
  /*sum for equilsom-routine*/
  soil->decomp_litter_mean+=decom_litter;
  soil->litter.decomC=decom_litter*param.atmfrac; /*only for mircobiological heating*/
  soil->count++;

  return decom_litter*param.atmfrac+soil_cflux;

} /* of 'littersom' */
