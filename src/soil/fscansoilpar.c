/***************************************************************************/
/**                                                                       **/
/**               f  s  c  a  n  s  o  i  l  p  a  r  .  c                **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-11-06 11:17:06 +0100 (ven., 06 nov. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define UNDEF (-1)

#define fscanreal2(isout,file,var,name)\
  if(fscanreal(file,var,name,isout))\
  {\
    return 0;\
  }
#define fscanint2(isout,file,var,name) \
  if(fscanint(file,var,name,isout))\
  {\
    return 0; \
  }

Real soildepth[NSOILLAYER];
Real layerbound[NSOILLAYER];
Real midlayer[NSOILLAYER];
Real logmidlayer[NSOILLAYER];   /*log10(midlayer[l]/midlayer[NSOILLAYER-2]), for vertical soc profile*/
Real fbd_fac[NFUELCLASS];

int fscansoilpar(FILE *file,         /* file  pointer */
                 Soilpar **soilpar,  /* Pointer to Soilpar array */
                 Bool isout         /* generate error output */
                )                    /* returns number of elements in array */
{
  int nsoil,n,id,l;
  String s;
  Soilpar *soil;
  for(l=0;l<NSOILLAYER;l++)
    if(fscanreal(file,soildepth+l,"soildepth",isout))
      return 0;
  for(l=0;l<NSOILLAYER;l++)
  {
    if(fscanreal(file,layerbound+l,"layerbound",isout))
      return 0;
    midlayer[l]=l>0?(layerbound[l-1]+soildepth[l]/2.):soildepth[l]/2.;
  }
  foreachsoillayer(l)
  {
    logmidlayer[l]=log10(midlayer[l]/midlayer[NSOILLAYER-2]);
  }
  for(l=0;l<NFUELCLASS;l++)
    if(fscanreal(file,fbd_fac+l,"fbd_fac",isout))
      return 0;
  if(fscanint(file,&nsoil,"nsoil",isout))
    return 0;
  if(nsoil<1 || nsoil>USHRT_MAX)
  {
    if(isout)
      fprintf(stderr,"ERROR170: Invalid value for number of soil types=%d\n",
              nsoil);
    return 0;
  }
  *soilpar=newvec(Soilpar,nsoil);
  check(*soilpar);
  for(n=0;n<nsoil;n++)
    (*soilpar)[n].type=UNDEF;
  for(n=0;n<nsoil;n++)
  {
    if(fscanint(file,&id,"soiltype",isout))
      return 0;
    if(id<0 || id>=nsoil)
    {
      if(isout)
        fprintf(stderr,"ERROR115: Invalid range of soil type=%d in fscansoilpar(), valid range is [0,%d].\n",id,nsoil-1);
      return 0;
    }
    soil=(*soilpar)+id;
    if(soil->type!=UNDEF)
    {
      if(isout)
        fprintf(stderr,"ERROR177: Soil type=%d has been already defined in fscansoilpar().\n",id);
      return 0;
    }
    if(fscanstring(file,s))
    {
      if(isout)
        readstringerr("name");
      return 0;
    }
    soil->name=strdup(s);
    check(soil->name);
    soil->type=id;
    fscanreal2(isout,file,&soil->Ks,"Ks");
    fscanreal2(isout,file,&soil->Sf,"Sf");
    fscanreal2(isout,file,&soil->wpwp,"w_pwp");
    fscanreal2(isout,file,&soil->wfc,"w_fc");
    fscanreal2(isout,file,&soil->wsat,"w_sat");
    soil->beta_soil=-2.655/log10(soil->wfc/soil->wsat);
    soil->whcs_all=0.0;
    for(l=0;l<LASTLAYER;l++)
    {
      soil->whc[l]=soil->wfc-soil->wpwp;
      soil->whcs[l]=soil->whc[l]*soildepth[l];
      soil->wpwps[l]=soil->wpwp*soildepth[l];
      soil->wsats[l]=soil->wsat*soildepth[l];
      soil->bulkdens[l]=(1-soil->wsat)*MINERALDENS;
      if(soil->type==ROCK)
        soil->k_dry[l]=8.8;
      else
        soil->k_dry[l]=(0.135*soil->bulkdens[l]+64.7)/
               (MINERALDENS-0.947*soil->bulkdens[l]);
    }
    /*assume last layer is bedrock in 6-layer version */
    soil->whc[BOTTOMLAYER]=0.002;/*0.006 wsats - 0.002 whc - 0.001 wpwps = 0.003 for free water */
    soil->whcs[BOTTOMLAYER]=soil->whc[BOTTOMLAYER]*soildepth[BOTTOMLAYER];
    soil->wpwps[BOTTOMLAYER]=0.001*soildepth[BOTTOMLAYER];
    soil->wsats[BOTTOMLAYER]=0.006*soildepth[BOTTOMLAYER];
    soil->bulkdens[BOTTOMLAYER]=(1-soil->wsats[BOTTOMLAYER]/soildepth[BOTTOMLAYER])*MINERALDENS;
    soil->k_dry[BOTTOMLAYER]=0.039*pow(soil->wsats[BOTTOMLAYER]/soildepth[BOTTOMLAYER],-2.2);
    fscanint2(isout,file,&soil->hsg,"hsg");
    if(soil->hsg<1 || soil->hsg>NHSG)
    {
      if(isout)
        fprintf(stderr,"ERROR199: Invalid value=%d for hsg in soil '%s'.\n",
                soil->hsg,soil->name);
      return 0;
    }
    fscanreal2(isout,file,&soil->tdiff_0,"tdiff_0");
    fscanreal2(isout,file,&soil->tdiff_15,"tdiff_15");
    fscanreal2(isout,file,&soil->tdiff_100,"tdiff_100");
    fscanreal2(isout,file,&soil->tcond_pwp,"cond_pwp");
    fscanreal2(isout,file,&soil->tcond_100,"cond_100");
    fscanreal2(isout,file,&soil->tcond_100_ice,"cond_100_ice");

  } /* of 'for(n=0;...)' */
  return n;
} /* of 'fscansoilpar' */
