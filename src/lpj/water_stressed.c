/***************************************************************************/
/**                                                                       **/
/**             w  a  t  e  r  _  s  t  r  e  s  s  e  d  .  c            **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-06-24 09:28:37 +0200 (mar., 24 juin 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define EPSILON 0.1  /* min precision of solution in bisection method */

typedef struct
{
  Real fac,co2,temp,apar,daylength,tstress;
  int path;
} Data;

static Real fcn(Real lambda,Data *data)
{
  Real agd,rd;

/*
 *              Call photosynthesis to determine alternative total
 *              daytime photosynthesis estimate (adt2) implied by
 *              Eqns 2 & 19, Haxeltine & Prentice 1996, and current
 *              guess for lambda (xmid)
 */
  
  return data->fac*(1-lambda)-photosynthesis(&agd,&rd,data->path,lambda,
                                             data->tstress,data->co2,
                                             data->temp,data->apar,
                                             data->daylength); 
/*
 *              Calculate total daytime photosynthesis implied by
 *              canopy conductance from water balance routine and
 *              current guess for lambda (xmid).  Units are mm/m2/day
 *              (mm come from gpd value, mm/day)
 *              Eqn 18, Haxeltine & Prentice 1996
 */

} /* of 'fcn' */

Real water_stressed(Pft *pft, /* pointer to PFT variabels */
                    Real aet_layer[LASTLAYER],       
                    Real gp_stand,
                    Real gp_stand_leafon, /* pot. canopy conduct. at full leaf cover */
                    Real gp_pft, /* potential canopy conductance */
                    Real *gc_pft,
                    Real *rd,
                    Real *wet,
                    Real pet,  /* potential evapotranspiration (mm) */
                    Real co2,  /* Atmospheric CO2 partial pressure (ppmv) */
                    Real temp, /* Temperature (deg C) */
                    Real par,  /* photosynthetic active radiation (J/m2/day) */
                    Real daylength, /* Daylength (h) */
                    Real *wdf,           /* water deficit fraction (0..100) */
                    Bool permafrost
                   ) /* returns gross primary productivity (gC/m2) */
{
  int l,i; 
  Real supply,supply_pft,demand,demand_pft,wr,lambda,gpd,agd,gc,aet,aet_cor;
  Data data;
  Real roots;
  Real rootdist_n[LASTLAYER];
  Real layer,root_u,root_nu;
  Real freeze_depth,thaw_depth;
  Bool isless=FALSE;
 
  wr=gpd=agd=*rd=layer=root_u=root_nu=aet_cor=0.0;
  forrootsoillayer(l)
    rootdist_n[l]=pft->par->rootdist[l];
  if(permafrost)
  {
    /*adjust root layer*/
    if(layerbound[BOTTOMLAYER]>pft->stand->soil.mean_maxthaw &&
       pft->stand->soil.mean_maxthaw>epsilon)
    {
      forrootsoillayer(l)
      {
        layer+=soildepth[l];
        root_u+=pft->par->rootdist[l];
        freeze_depth=layer-pft->stand->soil.mean_maxthaw;
        if (freeze_depth>0)
        {
          thaw_depth=soildepth[l]-freeze_depth;
          rootdist_n[l]=thaw_depth/soildepth[l]*pft->par->rootdist[l];
          root_nu=pft->par->rootdist[l]-rootdist_n[l];
          root_u-= root_nu;
          l++;
          break;
        }
      }
      for(i=l;i<BOTTOMLAYER;i++)
      {
        root_nu+=rootdist_n[i];
        rootdist_n[i]=0;
      }
      for(i=l-1;i>=0;--i)
        rootdist_n[i]=rootdist_n[i]/root_u*root_nu+rootdist_n[i];
    }
  }
  wr=roots=0;
  for(l=0;l<LASTLAYER;l++)
  {
    wr+=rootdist_n[l]*pft->stand->soil.w[l];
    roots+=rootdist_n[l];
    /*printf("wr=%lf rootdist=%lf w=%lf\n",wr,rootdist_n[l],pft->stand->soil.w[l]);*/
  }

  if(*wet>0.9999)
    *wet=0.9999;    
  
  if(pft->stand->type->landusetype==AGRICULTURE)
  {
    supply=pft->par->emax*wr*(1-exp(-1.0*pft->par->sla*((Pftcrop *)pft->data)->ind.root));
    if (pft->phen>0)
    {
      gp_stand=gp_stand/pft->phen*fpar(pft);
      gp_pft=gp_pft/pft->phen*fpar(pft);
    }
  }
  else
  {
    supply=pft->par->emax*wr*pft->phen;
  }

  supply_pft=supply*pft->fpc;
  demand=(gp_stand>0) ? (1.0-*wet)*pet*param.ALPHAM/(1+param.GM/gp_stand) : 0;
  demand_pft=(gp_pft>0) ? (1.0-*wet)*pet*param.ALPHAM/(1+param.GM/gp_pft) : 0;
  *wdf=wdf(pft,demand,supply); 

  if(pet>0 && gp_stand_leafon>0 && pft->fpc>0)
  {
    /*pft->wscal=(pft->par->emax*wr*pft->fpc)/(pet*ALPHAM/(1+GM/(gp_stand_leafon+pft->par->gmin*pft->fpc)));*/
    pft->wscal=(pft->par->emax*wr)/(pet*param.ALPHAM/(1+param.GM/gp_stand_leafon));
    if(pft->wscal>1)
      pft->wscal=1;
  }
  else 
    pft->wscal=1;
  
  pft->wscal_mean+=pft->wscal;
  
  if(supply_pft>=demand_pft)  
    *gc_pft=gp_pft;
  else if(pet>0)
  {
    *gc_pft=param.GM*supply_pft/((1.0-*wet)*pet*param.ALPHAM-supply_pft);
    if(*gc_pft<0)
      *gc_pft=0;
  }
  else
    *gc_pft=0;

  aet=(wr>0) ? min(supply,demand)/wr*pft->fpc : 0; 
  for (l=0;l<LASTLAYER;l++)
  {
    aet_layer[l]+=aet*rootdist_n[l]*pft->stand->soil.w[l];
    if (aet_layer[l]>pft->stand->soil.w[l]*pft->stand->soil.par->whcs[l])
    {
      aet_layer[l]=pft->stand->soil.w[l]*pft->stand->soil.par->whcs[l];
      aet_cor+=aet_layer[l];
      isless=TRUE;
    }
    else
      aet_cor+=aet_layer[l];
  }
  if (isless==TRUE && aet_cor<aet)
    supply=aet_cor*wr/pft->fpc;

  if(supply>=demand)  
    gc=gp_stand;
  else if(pet>0)
  {
    gc=param.GM*supply/((1.0-*wet)*pet*param.ALPHAM-supply);
    if(gc<0)
      gc=0;
  }
  else
    gc=0;
  /*gpd=hour2sec(daylength)*(gc-pft->par->gmin*pft->fpc*pft->phen);*/
 
  if(pft->stand->type->landusetype==AGRICULTURE)
    gpd=hour2sec(daylength)*(gc-pft->par->gmin*fpar(pft))*pft->fpc;
  else
    gpd=hour2sec(daylength)*(gc-pft->par->gmin*pft->phen)*pft->fpc;

  data.tstress=temp_stress(pft->par,temp,daylength);
  if(gpd>1e-5 && isphoto(data.tstress))
  {
    data.fac=gpd/1.6*ppm2bar(co2);
    data.path=pft->par->path;
    data.temp=temp;
    data.co2=ppm2Pa(co2);
    data.apar=par*alphaa(pft)*fpar(pft);
    data.daylength=daylength;
    lambda=bisect((Bisectfcn)fcn,0.02,LAMBDA_OPT+0.05,&data,0,EPSILON,10); 
/*     lambda=zbrent((Bisectfcn)fcn,0.02,LAMBDA_OPT+0.05,EPSILON,&data); */
    photosynthesis(&agd,rd,data.path,lambda,data.tstress,data.co2,
                   temp,data.apar,daylength);                   
    /* in rare occasions, agd(=GPP) can be negative, but shouldn't */
    agd=max(0,agd);
    *rd=*rd;    /* DON'T DELETE THIS LINE */
  }
  else
    return 0.0;
  return agd;
} /* of 'water_stressed' */
