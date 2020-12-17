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
/**     Last change: 25.01.2006  by S.Jachner, M.Gumpenberger             **/
/**     Last change: $Date:: 2015-07-23 11:11:13 +0200 (jeu., 23 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "crop.h"
#include "agriculture.h"

Real sowing_season(Cell *cell,            /* pointer to cell */
        int day,               /* day (1..365) */
        int npft,              /* number of natural PFTs  */
        int ncft,              /* number of crop PFTs */
        Real dprec,            /* today's precipitation (mm) */
        const Config *config   /* LPJ settings */
        )                       /* returns establish flux (gC/m2) */
{
    Bool alloc_today_rf=FALSE, alloc_today_ir=FALSE,istimber;
    int cft,m,mm,dayofmonth,month,s,s2;
    Real flux_estab=0;
    const Pftcroppar *croppar;
    Stand *setasidestand;

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
            //printf("===================== before cell->ml.sowing_date[cft]=%d\n", cell->ml.sowing_date[cft]);
            croppar=config->pftpar[npft+cft].data;
            cvrtdaymonth(&dayofmonth,&month,day);

            if (cell->ml.seasonality_type==NO_SEASONALITY) /*no seasonality*/
            {
                if (month==cell->ml.sowing_month[cft] && dayofmonth==1)
                {
                    /*rainfed CFTs*/
                    s=findlandusetype(cell->standlist,SETASIDE_RF);

                    if(s!=NOT_FOUND)
                    {
                        setasidestand=getstand(cell->standlist,s);
                        if(cell->ml.cropdates[cft].fallow<=0 &&
                                check_lu(cell->standlist,cell->ml.landfrac[0].crop[cft],npft+cft,FALSE))
                        {
                            if(!alloc_today_rf)
                            {
                                allocation_today(setasidestand, config->ntypes);
                                alloc_today_rf=TRUE;
                            }
                            flux_estab+=cultivate(cell,config->pftpar+npft+cft,
                                    cell->ml.cropdates[cft].vern_date20,
                                    cell->ml.landfrac[0].crop[cft],FALSE,day,FALSE,
                                    setasidestand,istimber,config->irrig_scenario,npft+ncft,cft);
                            cell->output.sdate[cft]=day;
                            cell->ml.sowing_date[cft] = day;
                            if(config->sdate_option==FIXED_SDATE){
                                cell->ml.sdate_fixed[cft]=day;
                                cell->ml.sowing_date[cft] = day;
                            }
                            /*if(setasidestand->type->landusetype!=SETASIDE)
                              break;*/
                        }
                    } /*of rainfed CFTs*/
                    /*irrigated CFTs*/
                    s=findlandusetype(cell->standlist,SETASIDE_IR);

                    if(s!=NOT_FOUND)
                    {
                        setasidestand=getstand(cell->standlist,s);if(cell->ml.cropdates[cft].fallow_irrig<=0 &&
                                check_lu(cell->standlist,cell->ml.landfrac[1].crop[cft],npft+cft,TRUE))
                        {
                            if(!alloc_today_ir)
                            {
                                allocation_today(setasidestand,config->ntypes);
                                alloc_today_ir=TRUE;
                            }
                            flux_estab+=cultivate(cell,config->pftpar+npft+cft,
                                    cell->ml.cropdates[cft].vern_date20,
                                    cell->ml.landfrac[1].crop[cft],TRUE,day,FALSE,
                                    setasidestand,istimber,config->irrig_scenario,npft+ncft,cft);
                            cell->output.sdate[cft+ncft]=day;
                            cell->ml.sowing_date[cft+ncft] = day;
                            if(config->sdate_option==FIXED_SDATE){
                                cell->ml.sdate_fixed[cft+ncft]=day;
                                cell->ml.sowing_date[cft+ncft] = day;
                            }
                            /*if(setasidestand->type->landusetype!=SETASIDE)
                              break;*/
                        }
                    } /*of irrigated CFTs*/
                }
            } /*of no seasonality*/

            if (cell->ml.seasonality_type==PREC || cell->ml.seasonality_type==PRECTEMP) /*precipitation-dependent rules*/
            {
                s=findlandusetype(cell->standlist,SETASIDE_RF);

                if(s!=NOT_FOUND)
                {
                    setasidestand=getstand(cell->standlist,s);
                    if (month==cell->ml.sowing_month[cft] && (dprec > MIN_PREC || dayofmonth==ndaymonth[month-1])) /*no irrigation, first wet day*/
                    {
                        if(cell->ml.cropdates[cft].fallow<=0 &&
                                check_lu(cell->standlist,cell->ml.landfrac[0].crop[cft],npft+cft,FALSE))
                        {
                            if(!alloc_today_rf)
                            {
                                allocation_today(setasidestand,config->ntypes);
                                alloc_today_rf=TRUE;
                            }
                            flux_estab+=cultivate(cell,config->pftpar+npft+cft,
                                    cell->ml.cropdates[cft].vern_date20,
                                    cell->ml.landfrac[0].crop[cft],FALSE,day,FALSE,
                                    setasidestand,istimber,config->irrig_scenario,npft+ncft,cft);
                            cell->output.sdate[cft]=day;
                            cell->ml.sowing_date[cft] = day;
                            if(config->sdate_option==FIXED_SDATE){
                                cell->ml.sdate_fixed[cft]=day;
                                cell->ml.sowing_date[cft] = day;
                            }
                            /*if(setasidestand->type->landusetype!=SETASIDE)
                              break;*/
                        }
                    }
                }
                s=findlandusetype(cell->standlist,SETASIDE_IR);

                if(s!=NOT_FOUND)
                {
                    setasidestand=getstand(cell->standlist,s);
                    if (month==cell->ml.sowing_month[cft+ncft] && (dprec > MIN_PREC || dayofmonth==ndaymonth[month-1])) /*irrigation, first wet day*/ 
                    {
                        if(cell->ml.cropdates[cft].fallow_irrig<=0 &&
                                check_lu(cell->standlist,cell->ml.landfrac[1].crop[cft],npft+cft,TRUE))
                        {
                            if(!alloc_today_ir)
                            {
                                allocation_today(setasidestand,config->ntypes);
                                alloc_today_ir=TRUE;
                            }
                            flux_estab+=cultivate(cell,config->pftpar+npft+cft,
                                    cell->ml.cropdates[cft].vern_date20,
                                    cell->ml.landfrac[1].crop[cft],TRUE,day,FALSE,
                                    setasidestand,istimber,config->irrig_scenario,npft+ncft,cft);
                            cell->output.sdate[cft+ncft]=day;
                            cell->ml.sowing_date[cft+ncft] = day;
                            if(config->sdate_option==FIXED_SDATE){
                                cell->ml.sdate_fixed[cft+ncft]=day;
                                cell->ml.sowing_date[cft+ncft] = day;
                            }
                            /*if(setasidestand->type->landusetype!=SETASIDE)
                              break;*/
                        }
                    }
                }
            } /*of precipitation seasonality*/

            if (cell->ml.seasonality_type==TEMP || cell->ml.seasonality_type==TEMPPREC) /*temperature-dependent rule*/
            {
                s=findlandusetype(cell->standlist,SETASIDE_RF);

                if(s!=NOT_FOUND)
                {
                    setasidestand=getstand(cell->standlist,s);
                    if (month==cell->ml.sowing_month[cft]) /*no irrigation*/
                    {
                        m = month-1; /*m runs from 0 to 11*/
                        mm = (m-1 < 0) ? NMONTH-1 : m-1; /*mm is the month before*/
                        if (cell->climbuf.mtemp20[mm] > cell->climbuf.mtemp20[m] && croppar->calcmethod_sdate==TEMP_WTYP_CALC_SDATE)
                        {
                            /*calculate day when temperature exceeds or falls below a crop-specific temperature threshold - from former function calc_cropdates*/
                            if(((cell->climbuf.temp[NDAYS-1]<croppar->temp_fall)
                                        &&(cell->climbuf.temp[NDAYS-2]>=croppar->temp_fall || dayofmonth==1)) || dayofmonth==ndaymonth[m]) /*sow winter variety*/
                            {
                                if(cell->ml.cropdates[cft].fallow<=0 &&
                                        check_lu(cell->standlist,cell->ml.landfrac[0].crop[cft],npft+cft,FALSE))
                                {
                                    if(!alloc_today_rf)
                                    {
                                        allocation_today(setasidestand, config->ntypes);
                                        alloc_today_rf=TRUE;
                                    }
                                    flux_estab+=cultivate(cell,config->pftpar+npft+cft,
                                            cell->ml.cropdates[cft].vern_date20,
                                            cell->ml.landfrac[0].crop[cft],FALSE,day,TRUE,
                                            setasidestand,istimber,config->irrig_scenario,npft+ncft,cft);
                                    cell->output.sdate[cft]=day;
                                    cell->ml.sowing_date[cft] = day;
                                    if(config->sdate_option==FIXED_SDATE){
                                        cell->ml.sdate_fixed[cft]=day;
                                        cell->ml.sowing_date[cft] = day;
                                    }
                                    /*if(setasidestand->type->landusetype!=SETASIDE)
                                      break;*/
                                }
                            }
                        }
                        else if (((cell->climbuf.temp[NDAYS-1]>croppar->temp_spring)
                                    &&(cell->climbuf.temp[NDAYS-2]<=croppar->temp_spring || dayofmonth==1)) || dayofmonth==ndaymonth[m]) /*sow summer variety */
                        {
                            if(cell->ml.cropdates[cft].fallow<=0 &&
                                    check_lu(cell->standlist,cell->ml.landfrac[0].crop[cft],npft+cft,FALSE))
                            {
                                if(!alloc_today_rf)
                                {
                                    allocation_today(setasidestand, config->ntypes);
                                    alloc_today_rf=TRUE;
                                }
                                flux_estab+=cultivate(cell,config->pftpar+npft+cft,
                                        cell->ml.cropdates[cft].vern_date20,
                                        cell->ml.landfrac[0].crop[cft],FALSE,day,FALSE,
                                        setasidestand,istimber,config->irrig_scenario,npft+ncft,cft);
                                cell->output.sdate[cft]=day;
                                cell->ml.sowing_date[cft] = day;
                                if(config->sdate_option==FIXED_SDATE){
                                    cell->ml.sdate_fixed[cft]=day;
                                    cell->ml.sowing_date[cft] = day;
                                }
                                /*if(setasidestand->type->landusetype!=SETASIDE)
                                  break;*/
                            }
                        } /*of cultivating summer variety*/
                    } /*of if month==ml.sowing_month[cft]*/
                }
                s=findlandusetype(cell->standlist,SETASIDE_IR);

                if(s!=NOT_FOUND)
                {
                    setasidestand=getstand(cell->standlist,s);

                    if (month==cell->ml.sowing_month[cft+ncft]) /*irrigation*/
                    {
                        m = month-1; /*m runs from 0 to 11*/
                        mm = (m-1 < 0) ? NMONTH-1 : m-1; /*mm is the month before*/
                        if (cell->climbuf.mtemp20[mm] > cell->climbuf.mtemp20[m] && croppar->calcmethod_sdate==TEMP_WTYP_CALC_SDATE)
                        {
                            /*calculate day when temperature exceeds or falls below a crop-specific temperature threshold - from former function calc_cropdates*/
                            if(((cell->climbuf.temp[NDAYS-1]<croppar->temp_fall)
                                        &&(cell->climbuf.temp[NDAYS-2]>=croppar->temp_fall || dayofmonth==1)) || dayofmonth==ndaymonth[m]) /*sow winter variety*/
                            {
                                if(cell->ml.cropdates[cft].fallow_irrig<=0 &&
                                        check_lu(cell->standlist,cell->ml.landfrac[1].crop[cft],npft+cft,TRUE))
                                {
                                    if(!alloc_today_ir)
                                    {
                                        allocation_today(setasidestand, config->ntypes);
                                        alloc_today_ir=TRUE;
                                    }
                                    flux_estab+=cultivate(cell,config->pftpar+npft+cft,
                                            cell->ml.cropdates[cft].vern_date20,
                                            cell->ml.landfrac[1].crop[cft],TRUE,day,TRUE,
                                            setasidestand,istimber,config->irrig_scenario,npft+ncft,cft);
                                    cell->output.sdate[cft+ncft]=day;
                                    cell->ml.sowing_date[cft+ncft] = day;
                                    if(config->sdate_option==FIXED_SDATE){
                                        cell->ml.sdate_fixed[cft+ncft]=day;
                                        cell->ml.sowing_date[cft+ncft] = day;
                                    }
                                    /*if(setasidestand->type->landusetype!=SETASIDE)
                                      break;*/
                                }
                            }
                        }
                        else if (((cell->climbuf.temp[NDAYS-1]>croppar->temp_spring)
                                    &&(cell->climbuf.temp[NDAYS-2]<=croppar->temp_spring || dayofmonth==1)) || dayofmonth==ndaymonth[m]) /*sow summer variety */
                        {
                            if(cell->ml.cropdates[cft].fallow_irrig<=0 &&
                                    check_lu(cell->standlist,cell->ml.landfrac[1].crop[cft],npft+cft,TRUE))
                            {
                                if(!alloc_today_ir)
                                {
                                    allocation_today(setasidestand, config->ntypes);
                                    alloc_today_ir=TRUE;
                                }
                                flux_estab+=cultivate(cell,config->pftpar+npft+cft,
                                        cell->ml.cropdates[cft].vern_date20,
                                        cell->ml.landfrac[1].crop[cft],TRUE,day,FALSE,
                                        setasidestand,istimber,config->irrig_scenario,npft+ncft,cft);
                                cell->output.sdate[cft+ncft]=day;
                                cell->ml.sowing_date[cft+ncft] = day;
                                if(config->sdate_option==FIXED_SDATE){
                                    cell->ml.sdate_fixed[cft+ncft]=day;
                                    cell->ml.sowing_date[cft+ncft] = day;
                                }
                                /*if(setasidestand->type->landusetype!=SETASIDE)
                                  break;*/
                            }
                        } /*of cultivating summer variety*/
                    } /*of if month==ml.sowing_month[cft+ncft]*/
                }
            } /*of temperature seasonality or both seasonality*/
            
            //printf("===================== after cell->ml.sowing_date[cft]=%d\n", cell->ml.sowing_date[cft]);

        }  /*for(cft=...) */
    }
    

    return flux_estab;
} /* of 'sowing_season' */
/*
   for each cell and day, called in update_daily:
   {
   month=monthofday;

   for(cft=0;cft<ncft;cft++)
   {
   if (seasonality_type[cell]==no seasonality)
   {
   if (month=sowing month[cft] && dayofmonth==1)
   {
   cultivate(irrigation=FALSE,wtype=FALSE)
   output->sdate[cft]=day
   cultivate(irrigation=TRUE, wtype=FALSE)
   output->sdate[cft+ncft]=day
   }
   }
   if (seasonality_type[cell]==precipitation)
   {
   if ((month==ml.sowing_month[cft] && precipitation > MIN_PREC) || dayofmonth==31)
   {
   cultivate(irrigation=FALSE, wtype=FALSE)
   output->sdate[cft]=day
   }
   if (month==ml.sowing_month[cft+ncft])
   {
   cultivate(irrigation=TRUE, wtype=FALSE)
   output->sdate[cft+ncft]=day
   }
   }
   if (seasonality_type[cell]==temperature or both)
   {
   if (month==ml.sowing_month[cft])
   {
   if (climbuf->mtemp20[lastmonth]>climbuf->mtemp20[thismonth] && calc_method = TEMP_WTYP_CALC_SDATE) => we are in fall
   {
   if(((cell->climbuf.temp[NDAYS-1]<croppar->temp_fall)
   && (cell->climbuf.temp[NDAYS-2]>=croppar->temp_fall || day==firstdayofmonth)) || day==lastdayofmonth)
   cultivate(irrigation=FALSE, wtype=TRUE)
   output->sdate[cft]=day
   else if (((cell->climbuf.temp[NDAYS-1]>croppar->temp_spring)
   && (cell->climbuf.temp[NDAYS-2]<=croppar->temp_spring || dayofmonth==1)) || dayofmonth==ndaymonth[m])
   cultivate(irrigation=FALSE, wtype=FALSE)
   output->sdate[cft]=day
   }
   }

   if (month==cell->ml.sowing_month[cft+ncft])
   {
   if (climbuf->mtemp20[lastmonth]>climbuf->mtemp20[thismonth] && calc_method = TEMP_WTYP_CALC_SDATE) => we are in fall
   {
   if(((cell->climbuf.temp[NDAYS-1]<croppar->temp_fall)
   && (cell->climbuf.temp[NDAYS-2]>=croppar->temp_fall || day==firstdayofmonth)) || day==lastdayofmonth)
   cultivate(irrigation=TRUE, wtype=TRUE)
   output->sdate[cft]=day
   else if (((cell->climbuf.temp[NDAYS-1]>croppar->temp_spring) => we are in spring
   && (cell->climbuf.temp[NDAYS-2]<=croppar->temp_spring || dayofmonth==1)) || dayofmonth==ndaymonth[m])
   cultivate(irrigation=TRUE, wtype=FALSE)
   output->sdate[cft]=day
   }
   }
   }
   }
   */
