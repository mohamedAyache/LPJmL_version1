/***************************************************************************/
/**                                                                       **/
/**     o  u  t  f  l  o  w  _  r  e  s  e  r  v  o  i  r  .  c           **/
/**                                                                       **/
/**     written by Hester Biemans                                         **/
/**     Wageningen University and research centre                         **/
/**                                                                       **/
/**     Last change: 26.02.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real outflow_reservoir(Resdata *resdata,int month)
{
  Real dfresout=0;
  
  if(resdata->c>=0.5)
    dfresout=max(0,resdata->k_rls*resdata->target_release_month[month]);
  else if (resdata->c>=0 && resdata->c<0.5)
    dfresout=max(0,pow(resdata->c/0.5,2)*resdata->k_rls*resdata->target_release_month[month]+(1-pow(resdata->c/0.5,2))*resdata->mean_inflow_month[month]/ndaymonth[month]); /* Hier nog naar kijken!!!! */
  else 
    fail(OUTFLOW_RESERVOIR_ERR,TRUE,"outflow_reservoir ERROR: c<0!");

  if(dfresout>resdata->dmass) /* hb added 11-08-08 to avoid negative dmass_reservoir */
    dfresout=resdata->dmass;

  resdata->dmass-=dfresout;
  
  return dfresout;
} /* of 'outflow_reservoir' */
