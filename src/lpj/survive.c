/***************************************************************************/
/**                                                                       **/
/**                         s  u  r  v  i  v  e  .  c                     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function determines wheher PFT is beyond its climatological       **/
/**     limits. Killed biomass is tranferred to litter                    **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  04.08.2010                                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool survive(const Pftpar *pftpar,const Climbuf *climbuf)
{
  Real temp_min20,temp_max20;
  
  temp_min20=getbufferavg(climbuf->min);
  temp_max20=getbufferavg(climbuf->max);
  return (temp_min20>=pftpar->temp.low) || 
         (temp_max20-temp_min20>=pftpar->min_temprange);

} /* of 'survive' */
