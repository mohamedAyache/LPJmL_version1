/***************************************************************************/
/**                                                                       **/
/**              e  s  t  a  b  l  i  s  h  .  c                          **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 16.01.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool establish(Real gdd,              /* growing degree days */
               const Pftpar *pftpar,  /* PFT parameter */
               const Climbuf *climbuf /* climate buffer */
              )                       /* returns if established */ 
{
  Real temp_min20;
  if(isemptybuffer(climbuf->min)) /* check to avoid division by zero */
    return FALSE;
  temp_min20=getbufferavg(climbuf->min);
  return (temp_min20>=pftpar->temp.low) && 
         (temp_min20<=pftpar->temp.high) && 
         (gdd>=pftpar->gdd5min);
/*         (gdd>=pftpar->gdd5min) && (climbuf->temp_max<=pftpar->twmax);*/
} /* of 'establish' */
