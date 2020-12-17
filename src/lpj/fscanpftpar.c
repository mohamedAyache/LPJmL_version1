/***************************************************************************/
/**                                                                       **/
/**               f  s  c  a  n  p  f  t  p  a  r  .  c                   **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Functions reads parameter for all defined PFTs from               **/
/**     configuration file                                                **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 01.03.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define UNDEF (-1)

#define fscanpftreal(isout,file,var,pft,name) \
  if(fscanreal(file,var,name,isout)) \
  { \
    if(isout)\
    fprintf(stderr,"ERROR110: Cannot read PFT '%s' in %s().\n",pft,__FUNCTION__); \
    return NULL; \
  }
#define fscanpftint(isout,file,var,pft,name) \
  if(fscanint(file,var,name,isout)) \
  { \
    if(isout)\
    fprintf(stderr,"ERROR110: Cannot read PFT '%s' in %s().\n",pft,__FUNCTION__); \
    return NULL; \
  }
#define fscanpftlimit(isout,file,var,pft,name) \
  if(fscanlimit(file,var,isout)) \
  { \
    if(isout)\
    fprintf(stderr,"ERROR112: Cannot read limit '%s' of PFT '%s' in %s().\n",name,pft,__FUNCTION__); \
    return NULL; \
  }
#define fscanpftemissionfactor(isout,file,var,pft,name) \
  if(fscanemissionfactor(file,var)) \
  { \
    if(isout)\
    fprintf(stderr,"ERROR128: Cannot read emission factor '%s' of PFT '%s' in %s().\n",name,pft,__FUNCTION__); \
    return NULL; \
  }

int *fscanpftpar(FILE  *file,         /* File pointer to ASCII file */
                 Pftpar **pftpar,     /* Pointer to PFT parameter array */
                 const Fscanpftparfcn scanfcn[], /* array of PFT-specific scan
                                                    functions */
                 int ntypes,          /* number of PFT classes */
                 Bool isout           /* generate error output */
                )                     /* returns array of size ntypes or NULL */
{
  int *npft,n,id,l,count;
  String s;
  Pftpar *pft;
  Real totalroots;
  Bool isbiomass;
  /* Read total number of defined PFTs */
  if(fscanint(file,&count,"npft",isout))
    return NULL;
  if(count<0 || count>UCHAR_MAX)
  {
    if(isout)
      fprintf(stderr,"ERROR171: Invalid number of PFTs=%d\n",count);
    return NULL;
  }
  npft=newvec(int,ntypes);
  check(npft);
  for(n=0;n<ntypes;n++)
    npft[n]=0;
  *pftpar=newvec(Pftpar,count);
  check(*pftpar);
  for(n=0;n<count;n++)
    (*pftpar)[n].id=UNDEF;
  isbiomass=FALSE;
  for(n=0;n<count;n++)
  {
    /* Read pft->id, defined in pftpar.h */
    if(fscanint(file,&id,"id",isout))
      return NULL;
    if(id<0 || id>=count)
    {
      if(isout)
        fprintf(stderr,"ERROR120: Invalid range of 'id'=%d in fscanpftpar().\n",
                id);
      return NULL;
    }
    pft=(*pftpar)+id;
    if(pft->id!=UNDEF)
    {
      if(isout)
        fprintf(stderr,
                "ERROR176: PFT id=%d has been already defined in fscanpftpar().\n",id);
      return NULL;
    }
    pft->id=id;

    /* Read pft->name */
    if(fscanstring(file,s))
    {
      if(isout)
        readstringerr("name");
      return NULL;
    }
    pft->name=strdup(s); /* store PFT name */
    check(pft->name);

    /* Read pft->type, defined in pftpar.h */
    fscanpftint(isout,file,&pft->type,pft->name,"type");
    fscanpftint(isout,file,&pft->cultivation_type,pft->name,"cultivation type");
    if(pft->cultivation_type<0 || pft->cultivation_type>ANNUAL_TREE)
    {
      if(isout)
        fprintf(stderr,"ERROR201: Invalid value %d for cultivation type of PFT '%s'.\n",pft->cultivation_type,pft->name);
      return NULL;
    }
    if(isbiomass && pft->cultivation_type==NONE)
    {
      if(isout)
        fprintf(stderr,"ERROR210: Natural PFT '%s' must be put before biomass plantation PFT.\n",pft->name);
      return NULL;
    }
    else if(pft->cultivation_type==BIOMASS)
      isbiomass=TRUE;
    for(l=0;l<NHSG;l++)
      fscanpftreal(isout,file,pft->cn+l,pft->name,"cn");
    /*
    pft->rootdist[BOTTOMLAYER]=1;
    for(l=0;l<BOTTOMLAYER;l++)
    {
      fscanpftreal(isout,file,pft->rootdist+l,pft->name,"rootdist");
      pft->rootdist[BOTTOMLAYER]-=pft->rootdist[l];
    }
    */
    fscanpftreal(isout,file,&pft->beta_root,pft->name,"beta_root");
    totalroots=1 - pow(pft->beta_root,layerbound[BOTTOMLAYER-1]/10);
    pft->rootdist[0]=(1 - pow(pft->beta_root,layerbound[0]/10))/totalroots;
    for(l=1;l<BOTTOMLAYER;l++)
      pft->rootdist[l]=(pow(pft->beta_root,layerbound[l-1]/10) - pow(pft->beta_root,layerbound[l]/10))/totalroots;
    fscanpftreal(isout,file,&pft->minwscal,pft->name,"minwscal");
    fscanpftreal(isout,file,&pft->gmin,pft->name,"gmin");
    fscanpftreal(isout,file,&pft->respcoeff,pft->name,
                 "respcoeff");
    fscanpftreal(isout,file,&pft->nmax,pft->name,"nmax");
    fscanpftreal(isout,file,&pft->resist,pft->name,"resist");
    fscanpftreal(isout,file,&pft->longevity,pft->name,
                 "longevity");
    pft->sla=2e-4*exp(6.15-0.46*log(pft->longevity*12));
    fscanpftreal(isout,file,&pft->lmro_ratio,pft->name,
                 "lmro_ratio");
    fscanpftreal(isout,file,&pft->ramp,pft->name,"ramp");
    pft->ramp=1/pft->ramp; /* store reciprocal to speed up calculations */
    fscanpftreal(isout,file,&pft->lai_sapl,pft->name,"lai_sapl");
    fscanpftreal(isout,file,&pft->gdd5min,pft->name,"gdd5min");
    fscanpftreal(isout,file,&pft->twmax,pft->name,"twmax");
    fscanpftreal(isout,file,&pft->gddbase,pft->name,"gddbase");
    fscanpftreal(isout,file,&pft->min_temprange,pft->name,
                 "min_temprange");
    fscanpftreal(isout,file,&pft->emax,pft->name,"emax");
    fscanpftreal(isout,file,&pft->intc,pft->name,"intc");
    fscanpftreal(isout,file,&pft->alphaa,pft->name,"alphaa");
    fscanpftint(isout,file,&pft->phenology,pft->name,"phenology");
    if(pft->phenology<0 || pft->phenology>CROPGREEN)
    {
      if(isout)
        fprintf(stderr,"ERROR201: Invalid value %d for phenology of PFT '%s'.\n",pft->phenology,pft->name);
      return NULL;
    }
    fscanpftint(isout,file,&pft->path,pft->name,"path");
    if(pft->path<0 || pft->path>C4)
    {
      if(isout)
        fprintf(stderr,"ERROR201: Invalid value %d for path of PFT '%s'.\n",pft->path,pft->name);
      return NULL;
    }
    fscanpftlimit(isout,file,&pft->temp_co2,pft->name,"temp_co2");
    fscanpftlimit(isout,file,&pft->temp_photos,pft->name,
                  "temp_photos");
    /* store precalculated k1, k2, k3 values */
    pft->k1=2*log(1/0.99-1)/(pft->temp_co2.low-pft->temp_photos.low);
    pft->k2=(pft->temp_co2.low+pft->temp_photos.low)*0.5;
    pft->k3=log(0.99/0.01)/(pft->temp_co2.high-pft->temp_photos.high);

    fscanpftlimit(isout,file,&pft->temp,pft->name,"temp");
    fscanpftreal(isout,file,&pft->soc_k,pft->name,"soc_k");
    fscanpftreal(isout,file,&pft->fuelbulkdensity,pft->name,"fuel bulk density");
    fscanpftemissionfactor(isout,file,&pft->emissionfactor,
                           pft->name,"emission factor");
    fscanpftreal(isout,file,&pft->aprec_min,pft->name,
                 "aprec_min");
    fscanpftreal(isout,file,&pft->flam,pft->name,"flam");
    fscanpftreal(isout,file,&pft->k_litter10.leaf,pft->name,
                 "k_litter10_leaf");
    fscanpftreal(isout,file,&pft->k_litter10.wood,pft->name,
                 "k_litter10_wood");
    fscanpftreal(isout,file,&pft->windspeed,pft->name,
                 "windspeed dampening");
    fscanpftreal(isout,file,&pft->roughness,pft->name,
                 "roughness length");
    pft->k_litter10.leaf/=NDAYYEAR;
    pft->k_litter10.wood/=NDAYYEAR;
    if(pft->type<0 || pft->type>=ntypes)
    {
      if(isout)
        fprintf(stderr,"ERROR116: Invalid PFT class=%d of PFT '%s'.\n",
                pft->type,pft->name);
      return NULL;
    }
    npft[pft->type]++;
    /* set default PFT-specific functions */
    pft->init=noinit;
    pft->fire=nofire;
    pft->mix_veg=nomix_veg;
    pft->adjust=noadjust;
    pft->establishment=noestablishment;
    pft->wdf=nowdf;
    pft->turnover_monthly=noturnover_monthly;
    /* Now scan PFT-specific parameters and set specific functions */
    if(scanfcn[pft->type](file,pft,isout))
      return NULL;
  }
  return npft;
} /* of 'fscanpftpar' */
