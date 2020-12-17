/***************************************************************************/
/**                                                                       **/
/**           s o i l i c e 2 m o i s t u r e . c                         **/
/**                                                                       **/
/**     thawing of soil ice.                                              **/
/**                                                                       **/
/**     written by Roy van den Houdt, Bart Strengers                      **/
/**     MNP/KMD                                                           **/
/**     PO Box 303                                                        **/
/**     3720 AH Bilthoven/The Netherlands                                 **/
/**                                                                       **/
/**     Last change: 30.09.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

/* assuming positive heat */

void soilice2moisture(Soil *soil, /* pointer to soil data */
                      Real *heat, /* heat (J/m2) */
                      int l       /* soil layer */
                     )
{
  Real melt_heat, melt_heat_ice, melt_heat_pwp, melt_heat_fw; /* The energy avaible to melt ice (J/m2)  */
  Real ice_water_old, soil_water_old,frac_melt;
  
  ice_water_old=allice(soil,l);
  soil_water_old=allwater(soil,l);
  /*ice_water_old=soil->ice_depth[l];
  soil_water_old=soil->w[l]*soil->par->whcs[l];*/
  
  melt_heat_ice = soil->ice_depth[l]*1e-3*c_water2ice;
  melt_heat_fw = soil->ice_fw[l]*1e-3*c_water2ice;
  melt_heat_pwp = soil->par->wpwps[l]*soil->ice_pwp[l]*1e-3*c_water2ice;
  if(melt_heat_ice+melt_heat_pwp+melt_heat_fw > (*heat))
  {
    melt_heat=*heat;
    /*melt_heat_ice = min(melt_heat, melt_heat_ice);
    melt_heat_pwp = melt_heat - melt_heat_ice;*/    
    /* melting: first ice_fw, then ice and ice_pwp*/
    melt_heat_fw=min(melt_heat,melt_heat_fw);
    /* fraction melting of ice and ice_pwp*/
    frac_melt=min(1,(melt_heat-melt_heat_fw)/(melt_heat_ice+melt_heat_pwp));
    melt_heat_ice = frac_melt*melt_heat_ice;
    melt_heat_pwp = frac_melt*melt_heat_pwp;
     
  }
  else
    melt_heat=melt_heat_ice+melt_heat_pwp+melt_heat_fw;
  *heat-=melt_heat;
  soil->ice_depth[l]-=melt_heat_ice/c_water2ice*1000;/*[mm]*/
  soil->w[l]+=(melt_heat_ice/c_water2ice)/soil->par->whcs[l]*1000; /*fraction of whcs*/
  if (fabs(soil->ice_depth[l])<epsilon)
    soil->ice_depth[l]=0;
  /* conversion of water below permanent wilting point*/
  soil->ice_pwp[l]-=melt_heat_pwp/c_water2ice*1000/soil->par->wpwps[l];
  if(fabs(soil->ice_pwp[l])<epsilon)
    soil->ice_pwp[l]=0.0;
  /* conversion of free water */
  soil->ice_fw[l]-=melt_heat_fw/c_water2ice*1000;/*[mm]*/
  soil->w_fw[l]+=melt_heat_fw/c_water2ice*1000; /*mm*/
  if (fabs(soil->ice_fw[l])<epsilon)
    soil->ice_fw[l]=0;
#ifdef SAFE
  if(soil->ice_depth[l]<0 || soil->ice_depth[l]>(soil->par->whcs[l]+epsilon))
    fprintf(stderr,"soil->ice_depth[%d]=%.10f in soilice2moisture\n",l,soil->ice_depth[l]);
  if(soil->ice_pwp[l]<0)
    fprintf(stderr,"soil->ice_pwp[l]=%.10f in soilice2moisture\n",soil->ice_pwp[l]);
  /*if(ice_water_old<=0)
     fprintf(stderr,"ice_water_old = 0 in soilice2moisture\n");*/
  if(soil->w[l]<0)
    fprintf(stderr,"w[l]=%.10f soil->ice_depth[l]=%.10f in soilice2moisture\n",soil->w[l],soil->ice_depth[l]);
#endif
  if(ice_water_old > epsilon)
    soil->freeze_depth[l]-=soil->freeze_depth[l]*(allwater(soil,l)-soil_water_old)/ice_water_old;
  /*soil->freeze_depth[l]-=soil->freeze_depth[l]*(soil->w[l]*soil->par->whcs[l]-soil_water_old)/ice_water_old;*/
  if (fabs(soil->freeze_depth[l])<epsilon)
    soil->freeze_depth[l]=0;
} /* of 'soilice2moisture' */

/* CVS information:*/
#ifdef INCLUDE_RCSID             
static char const rcsid[] = 
    "@(#) $Id: soilice2moisture.c 279 2009-04-08 13:27:12Z heyder $"; /*survives gcc optimization*/ 
GENERATE_RCSID(rcsid2,
    "@(#) $Id: soilice2moisture.c 279 2009-04-08 13:27:12Z heyder $"); /*survives xlc optimization*/
#endif
