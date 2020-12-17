/***************************************************************************/
/**                                                                       **/
/**                 c  o  n  v  e  r  t  _  w  a  t  e  r  .  c           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 30.09.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void convert_water(Soil* soil, /* pointer to soil data */
                   int l,      /* soil layer */
                   Real* heat  /* heat (J/m2) */
                  )
{
  Real heatcap,
       freeze_heat,
       melt_heat;
  switch (getstate(soil->temp+l))
  {
    case AT_T_ZERO:/* most probable, continue below */
      break;
    case BELOW_T_ZERO: 
      if(allwater(soil,l)>epsilon)
      { /* e.g. rain on frozen ground */
        /*energy from freezing water heats soil, no change in heat*/
        heatcap=soilheatcap(soil,l);/* [J/m2/K] */
        /*  The heat (in J/m2) that will be released if  soil moisture is converted to ice */
        freeze_heat = allwater(soil,l)*1e-3*c_water2ice; /* [m]*[J/m3]=[J/m2] */
        /*freeze all available water or freeze water until T_zero is reached*/
        freeze_heat = min((T_zero-soil->temp[l])*heatcap,freeze_heat);
        if(freeze_heat>epsilon)
        {
          soil->temp[l] += freeze_heat/heatcap;
          freeze_heat *= (-1);
          moisture2soilice(soil,&freeze_heat,l);  
          soil->state[l]=(short)getstate(soil->temp+l);
        }        
      }
      else
      {
        if (fabs(soil->w[l])<epsilon)
          soil->w[l]=0.0;
        if (soil->w[l]<0)
          fail(PERM_ERR,FALSE,"soil->w[%d]=%.10f in convert_water\n",l,
               soil->w[l]);
      }
      break;
    case ABOVE_T_ZERO:
      if(allice(soil,l)>epsilon)
      {
        /*energy used for melting cools soil directly, no change in heatflux*/
        heatcap=soilheatcap(soil,l);/* [J/m2/K] */
        melt_heat=(allice(soil,l)*1e-3)*c_water2ice;/*[J/m2]*/
        /*thaw all available water or until T_zero is reached*/
        melt_heat=min((soil->temp[l]-T_zero)*heatcap,melt_heat);
        if(melt_heat>epsilon)
        {
          soil->temp[l] -= melt_heat/heatcap;
          soilice2moisture(soil,&melt_heat,l); 
          soil->state[l]=(short)getstate(soil->temp+l);
        }
      }     
      else if (soil->ice_depth[l]<0)
        fail(PERM_ERR,FALSE,"ice=%.2f\n",soil->ice_depth[l]);
  } /* of switch */
  /*now use the energy from heat for any additional water/ice conversions*/
  if(soil->state[l]==AT_T_ZERO)
  {
    if(*heat>epsilon && (allice(soil,l)>epsilon))/*melting*/
      soilice2moisture(soil,heat,l);
    else if(*heat<-epsilon && (allwater(soil,l)>epsilon))/*freezing*/
      moisture2soilice(soil,heat,l);
  }
} /* of 'convert_water' */

/* Version information:*/
#ifdef INCLUDE_RCSID             
static char const rcsid[] = 
    "@(#) $Id: convert_water.c 123 2008-12-16 14:13:33Z heyder $"; /*survives gcc optimization*/ 
GENERATE_RCSID(rcsid2,
    "@(#) $Id: convert_water.c 123 2008-12-16 14:13:33Z heyder $"); /*survives xlc optimization*/
#endif
