/***************************************************************************/
/**                                                                       **/
/**              i  n  i  t  s  o  i  l  t  e  m  p  .  c                 **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define nsoilmeanyears 30
#define initfrozen -0.5

Bool initsoiltemp(Climate* climate, Cell *grid,const Config *config)
{
  int year,s,l,day,dayofmonth;
  int month;
  int cell;
  Real whcs_all=0.0;
  Real temp,pet,prec,balance,dl,rad,delta,acos_dl;
  Stand* stand;
  for (year=config->firstyear; year < config->firstyear+nsoilmeanyears; ++year)
  {
    if(getclimate(climate,grid,year,config))
      return TRUE;
    day=0;
    foreachmonth(month)
    {
      for(cell=0;cell<config->ngridcell;cell++)
        if(!(grid+cell)->skip)
        {
          rad=deg2rad(grid[cell].coord.lat);
          delta=deg2rad(-23.4*cos(2*M_PI*(midday[month]+10.0)/NDAYYEAR));
          acos_dl=(-tan(delta)*tan(rad));
          if (acos_dl< -1.0)
            acos_dl=-1.0;
          else if (acos_dl>1.0)
            acos_dl=1.0;
          dl=2*acos(acos_dl)/0.2618;
          if(climate->file_temp.isdaily)
          {
            temp=0;
            foreachdayofmonth(dayofmonth,month)
              temp+=climate->data.temp[cell*NDAYYEAR+day+dayofmonth];
            temp*=ndaymonth1[month];
          }
          else
            temp=(getcelltemp(climate,cell))[month];
          if(climate->file_prec.isdaily)
          {
            prec=0;
            foreachdayofmonth(dayofmonth,month)
              prec+=climate->data.prec[cell*NDAYYEAR+day+dayofmonth];
          }
          else
            prec=(getcellprec(climate,cell))[month];
          pet=((temp>0) ? 924*dl*0.611*exp(17.3*temp/(temp+237.3))/(temp+273.2):0);

          balance=prec-pet;
          if(balance< epsilon) balance=0.0;
          foreachstand(stand,s,((grid+cell)->standlist))
          {
            whcs_all=0.0;
            foreachsoillayer(l) whcs_all+=stand->soil.par->whcs[l];
            foreachsoillayer(l)
            {
              stand->soil.temp[l]+=temp/NMONTH/nsoilmeanyears;
              stand->soil.w[l]+=balance/nsoilmeanyears/stand->soil.par->whcs[l]*stand->soil.par->whcs[l]/whcs_all;
            }
          }
        }
      day+=ndaymonth[month];
    } /* of foreachmonth */
  }
  for(cell=0;cell<config->ngridcell;cell++)
    if(!(grid+cell)->skip)
      foreachstand(stand,s,(grid+cell)->standlist)
        foreachsoillayer(l)
        {
          if(stand->soil.w[l]>1) stand->soil.w[l]=1.0;
          if(stand->soil.temp[l]<initfrozen)
          {
            stand->soil.ice_depth[l]=stand->soil.par->whcs[l]*stand->soil.w[l];
            stand->soil.w[l]=0;
            stand->soil.freeze_depth[l]=soildepth[l];
            stand->soil.ice_pwp[l]=1;
          }   
        }
  return FALSE;
} /* of 'initsoiltemp' */

/* CVS information:*/
#ifdef INCLUDE_RCSID             
static char const rcsid[] = 
    "@(#) $Id: initsoiltemp.c 153 2008-12-22 14:31:36Z heyder $"; /*survives gcc optimization*/ 
GENERATE_RCSID(rcsid2,
    "@(#) $Id: initsoiltemp.c 153 2008-12-22 14:31:36Z heyder $"); /*survives xlc optimization*/
#endif
