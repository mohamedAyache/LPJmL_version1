/***************************************************************************/
/**                                                                       **/
/**           m o i s t u r e 2 s o i l i c e . c                         **/
/**                                                                       **/
/**     freezing of soil moisture.                                        **/
/**                                                                       **/
/**     written by Roy van den Houdt, Bart Strengers                      **/
/**     MNP/KMD                                                           **/
/**     PO Box 303                                                        **/
/**     3720 AH Bilthoven/The Netherlands                                 **/
/**                                                                       **/
/**     Last change: $Date:: 2014-11-06 10:13:14 +0100 (jeu., 06 nov. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

/*assuming negative heat*/
void moisture2soilice(Soil *soil, /* pointer to soil data */
                      Real *heat, /* heat (J/m2) */
                      int l       /* soil layer */
                     )
{
  Real freeze_heat,freeze_heat_w, freeze_heat_pwp, freeze_heat_fw; /* The energy (J/m2) that will be added to heat during freezing */
  Real ice_water_old, soil_water_old,frac_freeze;

  ice_water_old=allice(soil,l);
  soil_water_old=allwater(soil,l);
/*  The heat (in J/m2) that will be released if  soil moisture is converted to ice */
  freeze_heat_w = soil->w[l]*soil->par->whcs[l]*1e-3*c_water2ice; /* [m]*[J/m3]=[J/m2] */
  freeze_heat_pwp = soil->par->wpwps[l]*(1-soil->ice_pwp[l])*1e-3*c_water2ice;
  freeze_heat_fw = soil->w_fw[l]*1e-3*c_water2ice;
  if(freeze_heat_w+freeze_heat_pwp+freeze_heat_fw > -(*heat))
  {
    freeze_heat=-(*heat);
    /* freezing: first w and pwp, then w_fw*/
    /* fraction freezing of w and pwp*/
    if((freeze_heat_w+freeze_heat_pwp)>0)
      frac_freeze=min(1,freeze_heat/(freeze_heat_w+freeze_heat_pwp));
    else
      frac_freeze=1;
    freeze_heat_w = frac_freeze*freeze_heat_w;
    freeze_heat_pwp = frac_freeze*freeze_heat_pwp;
    /*freeze_heat_w = min(freeze_heat, freeze_heat_w);
    freeze_heat_pwp = freeze_heat - freeze_heat_w;*/
    if(fabs(frac_freeze-1)<epsilon)
        freeze_heat_fw=freeze_heat-freeze_heat_w-freeze_heat_pwp;
    else
         freeze_heat_fw=0;  
  }
  else
    freeze_heat=freeze_heat_w+freeze_heat_pwp+freeze_heat_fw;
  *heat+=freeze_heat;
  if(fabs(*heat)<epsilon)
    *heat=0.0;
  /* amount of ice formed in current timestep (mm) */
  soil->ice_depth[l]+=freeze_heat_w/c_water2ice*1000; /*[mm]*/
  soil->w[l]-=freeze_heat_w/c_water2ice*1000/soil->par->whcs[l];
  if (fabs(soil->w[l])<epsilon)
    soil->w[l]=0;
  /* conversion of water below permanent wilting point*/
  soil->ice_pwp[l]+=freeze_heat_pwp/c_water2ice*1000/soil->par->wpwps[l];
  /* conversion of free water*/
  soil->ice_fw[l]+=freeze_heat_fw/c_water2ice*1000; /*[mm]*/
  soil->w_fw[l]-=freeze_heat_fw/c_water2ice*1000;
  if (fabs(soil->w_fw[l])<epsilon)
    soil->w_fw[l]=0;
  if(soil_water_old>epsilon)
    soil->freeze_depth[l]+=(soildepth[l]-soil->freeze_depth[l])*
                           (allice(soil,l)-ice_water_old)/
                           soil_water_old;
  /*soil->freeze_depth[l]+=(soildepth[l]-soil->freeze_depth[l])*
                         (soil->ice_depth[l]-ice_water_old)/
                         soil_water_old;*/
#ifdef SAFE
  if(soil->w[l]<0)
    fail(PERM_ERR,FALSE,"moisture2soilice --- soil.w[%d] =%.10f",l,soil->w[l]);
  if(soil->ice_depth[l]<0 || soil->ice_depth[l]>soil->par->whcs[l]+epsilon)
    fprintf(stderr,"soil->ice_depth[%d]=%.10f in moisture2soilice\n",l,soil->ice_depth[l]);
#endif
} /* of 'moisture2soilice' */

/* CVS information:*/
#ifdef INCLUDE_RCSID             
static char const rcsid[] = 
    "@(#) $Id: moisture2soilice.c 279 2009-04-08 13:27:12Z heyder $"; /*survives gcc optimization*/ 
GENERATE_RCSID(rcsid2,
    "@(#) $Id: moisture2soilice.c 279 2009-04-08 13:27:12Z heyder $"); /*survives xlc optimization*/
#endif
