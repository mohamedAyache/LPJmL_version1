/***************************************************************************/
/**                                                                       **/
/**                   s  o  w  i  n  g  .  c                              **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-23 11:11:13 +0200 (jeu., 23 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "crop.h"
#include "agriculture.h"

Real sowing_prescribe(Cell *cell,
                      int day,
                      int npft,
                      int ncft,
                      const Config *config
                     )
{
  Real flux_estab=0;
  Stand *setasidestand;
  Bool alloc_today_rf=FALSE, alloc_today_ir=FALSE,istimber;
  const Pftcroppar *croppar;
  int cft,s,s2;
  int earliest_sdate;
  Bool wtype=FALSE;

#ifdef IMAGE
  istimber=(config->start_imagecoupling!=INT_MAX);
#else
  istimber=FALSE;
#endif
  s=findlandusetype(cell->standlist,SETASIDE_RF);
  s2=findlandusetype(cell->standlist,SETASIDE_IR);
  if(s!=NOT_FOUND || s2!=NOT_FOUND)
  {

    for(cft=0;cft<ncft;cft++)
    {
      croppar=config->pftpar[npft+cft].data;
      earliest_sdate=(cell->coord.lat>=0) ? croppar->initdate.sdatenh : croppar->initdate.sdatesh;

      /*rainfed crops*/
      s=findlandusetype(cell->standlist,SETASIDE_RF);
      if(s!=NOT_FOUND)
      {
        setasidestand=getstand(cell->standlist,s);

        if(day==cell->ml.sdate_fixed[cft])
        {
          wtype = (croppar->calcmethod_sdate==TEMP_WTYP_CALC_SDATE && day>earliest_sdate) ? TRUE : FALSE;
          if(check_lu(cell->standlist,cell->ml.landfrac[0].crop[cft],npft+cft,FALSE))
          {
            if(!alloc_today_rf)
            {
              allocation_today(setasidestand, config->ntypes);
              alloc_today_rf=TRUE;
            }
            flux_estab+=cultivate(cell,config->pftpar+npft+cft,
              cell->ml.cropdates[cft].vern_date20,
              cell->ml.landfrac[0].crop[cft],FALSE,day,wtype,
              setasidestand,istimber,config->irrig_scenario,npft+ncft,cft);
            cell->output.sdate[cft]=day;
            /*if(setasidestand->type->landusetype!=SETASIDE)
              break;*/
          }
        }/*of rainfed CFTs*/
      }
      s=findlandusetype(cell->standlist,SETASIDE_IR);
      if(s!=NOT_FOUND)
      {
        setasidestand=getstand(cell->standlist,s);


        /*irrigated crops*/
        if(day==cell->ml.sdate_fixed[cft+ncft])
        {
          wtype = (croppar->calcmethod_sdate==TEMP_WTYP_CALC_SDATE && day>earliest_sdate) ? TRUE : FALSE;
          if(check_lu(cell->standlist,cell->ml.landfrac[1].crop[cft],npft+cft,TRUE))
          {
            if(!alloc_today_ir)
            {
              allocation_today(setasidestand, config->ntypes);
              alloc_today_ir=TRUE;
            }
            flux_estab+=cultivate(cell,config->pftpar+npft+cft,
              cell->ml.cropdates[cft].vern_date20,
              cell->ml.landfrac[1].crop[cft],TRUE,day,wtype,
              setasidestand,istimber,config->irrig_scenario,npft+ncft,cft);
            /*if(setasidestand->type->landusetype!=SETASIDE)
              break;*/
          }
        }/*of irrigated CFTs*/
      }
    }
  }
  return flux_estab;
}
