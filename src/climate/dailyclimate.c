/***************************************************************************/
/**                                                                       **/
/**                d  a  i  l  y  c  l  i  m  a  t  e  .  c               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function dailyclimate gets daily value for temperature,           **/
/**     precipitation, cloudiness and wet days.                           **/
/**     The pointer to the climate data has to be initialized by the      **/
/**     function getclimate for the specified year.                       **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  19.02.2013                                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void dailyclimate(Dailyclimate *daily,
                  const Climate *climate, /* climate data pointer */
                  Climbuf *climbuf,
                  int cell,               /* cell index */
                  int day,                /* day (1..365) */
                  int month,              /* month (0..11) */
                  int dayofmonth)
{
  Real tamp;
  if(climate->file_temp.fmt!=FMS)
  {
    if(climate->file_temp.isdaily)
    {
      daily->temp=climate->data.temp[cell*NDAYYEAR+day-1];
      climbuf->mtemp+=daily->temp;
    }
    else
      daily->temp=interpolate(getcelltemp(climate,cell),month,dayofmonth);
  }
  else
    climbuf->mtemp+=daily->temp;
  if(climate->data.sun!=NULL)
    daily->sun=(climate->file_cloud.isdaily) ? climate->data.sun[cell*NDAYYEAR+day-1]
                      : interpolate(getcellsun(climate,cell),month,dayofmonth);
  if(climate->data.lwnet!=NULL)
    daily->lwnet=(climate->file_lwnet.isdaily) ? climate->data.lwnet[cell*NDAYYEAR+day-1]
                      : interpolate(getcelllwnet(climate,cell),month,dayofmonth);
  if(climate->data.swdown!=NULL)
    daily->swdown=(climate->file_swdown.isdaily) ? climate->data.swdown[cell*NDAYYEAR+day-1]
                      : interpolate(getcellswdown(climate,cell),month,dayofmonth);
  if(climate->data.wind!=NULL)
  {
    if(climate->file_wind.isdaily)
      daily->windspeed=climate->data.wind[cell*NDAYYEAR+day-1];
    else
      daily->windspeed=interpolate(getcellwind(climate,cell),month,dayofmonth);
  }
  if(climate->data.tamp!=NULL)
  {
#ifdef TMAX_SPITFIRE
    daily->tmin=(climate->file_tamp.isdaily) ?  climate->data.tamp[cell*NDAYYEAR+day-1] : interpolate(getcelltamp(climate,cell),month,dayofmonth);
    daily->tmax=(climate->file_tmax.isdaily) ?  climate->data.tmax[cell*NDAYYEAR+day-1] : interpolate(getcelltmax(climate,cell),month,dayofmonth);
#else
    tamp=(climate->file_tamp.isdaily) ?  climate->data.tamp[cell*NDAYYEAR+day-1] : interpolate(getcelltamp(climate,cell),month,dayofmonth);
    daily->tmin=daily->temp-tamp*0.5;
    daily->tmax=daily->temp+tamp*0.5;
#endif
  }
  if(climate->data.lightning!=NULL)
  {
    if(climate->file_lightning.isdaily)
      daily->lightning=climate->data.lightning[cell*NDAYYEAR+day-1];
    else
      daily->lightning=interpolate(getcelllightning(climate,cell),month,dayofmonth);
  }
  if(climate->file_prec.fmt!=FMS)
  {
    if(climate->file_prec.isdaily)
    {
      daily->prec=climate->data.prec[cell*NDAYYEAR+day-1];
      climbuf->mprec+=daily->prec;
    }
    else
    {
      daily->prec=(israndomprec(climate)) ? climbuf->dval_prec[dayofmonth+1] :
          interpolate(getcellprec(climate,cell),month,dayofmonth)*
                        ndaymonth1[month];
      daily->prec=(daily->prec>0.000001) ? daily->prec : 0.0;
    }
  }
  else
    climbuf->mprec+=daily->prec;
} /* of 'dailyclimate' */
