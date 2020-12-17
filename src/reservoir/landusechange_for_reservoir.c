/***************************************************************************/
/**                                                                       **/
/**     l a n d u s e c h a n g e _ f o r _ r e s e r v o i r . c         **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Hester Biemans                                         **/
/**     Wageningen University and Research Centre                         **/
/**                                                                       **/
/**     Last change: $Date:: 2016-02-11 11:10:20 +0100 (jeu., 11 févr.#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"
#include "grass.h"
#include "natural.h"
#include "agriculture.h"

static void deforest_for_reservoir(Cell *cell,
                                   Real difffrac,
                                   Bool istimber,
                                   int ntotpft /* total number of PFTs */
                                  )
{
  int s,pos;
  Stand *natstand,*cutstand;

  s=findlandusetype(cell->standlist,NATURAL);
  if(s!=NOT_FOUND)
  {
    pos=addstand(&natural_stand,cell)-1;
    natstand=getstand(cell->standlist,s);
    cutstand=getstand(cell->standlist,pos);
    cutstand->frac=difffrac;
    reclaim_land(natstand,cutstand,cell,istimber,ntotpft);
    if(difffrac+epsilon>=natstand->frac)
    {
      difffrac=natstand->frac;
      delstand(cell->standlist,s);
      pos=s;
    }
    else
      natstand->frac-=difffrac;


    /* all the water from the cutstand goes in the reservoir */
    cell->ml.resdata->dmass+=soilwater(&cutstand->soil)*cutstand->frac*cell->coord.area;

    /* all the carbon from the reservoir goes into a c_pool */
    cell->ml.resdata->c_pool+=soilcarbon(&cutstand->soil)*cutstand->frac;
    /* delete the cutstand */
    delstand(cell->standlist,pos);
  }
  else
    fail(NO_NATURAL_STAND_ERR,TRUE,"No natural stand for deforest_for_reservoir()");

} /* of 'deforest_for_reservoir' */

static Real from_setaside_for_reservoir(Cell *cell,
                                        Real difffrac,
                                        const Pftpar pftpar[],
                                        Bool istimber,
                                        Bool intercrop,
                                        int npft,
                                        int ncft)
{
  int s,s2,pos,p;
  Stand *setasidestand,*setasidestand_ir,*cutstand, *stand;
  Real factor;
  Pft *pft;
  Irrigation *data;

  /* first check if there is any SETASIDE in the cell */
  s=findlandusetype(cell->standlist,SETASIDE_RF);
  s2=findlandusetype(cell->standlist,SETASIDE_IR);
  /* if there IS already setaside in the the cell: */
  if(s!=NOT_FOUND)
  {
    setasidestand=getstand(cell->standlist,s);
    factor=0.0; /* initialize factor with 0, assumed that there is enough setaside stand in the cell */
    /* check if there is ENOUGH setaside stand to put the reservoir */
    if(difffrac>setasidestand->frac+epsilon) /* IF not: reduce cropland with factor and add to
                                        setasidestand */
    {
      if(s2!=NOT_FOUND) /* if irrigated setaside is available, merge as much as needed */
      {
        setasidestand_ir=getstand(cell->standlist,s2);
        if(difffrac+epsilon>=setasidestand->frac+setasidestand_ir->frac)
        {
          /* remove all vegetation on irrigated setaside */
          foreachpft(pft,p,&setasidestand_ir->pftlist){
            litter_update(&setasidestand_ir->soil.litter,pft,pft->nind);
            delpft(&setasidestand_ir->pftlist,p);
            p--; /* adjust loop variable */
          }
          mixsetaside(setasidestand,setasidestand_ir,intercrop);
          delstand(cell->standlist,s2);
        }
        else{
          pos=addstand(&natural_stand,cell)-1;
          cutstand=getstand(cell->standlist,pos);
          cutstand->frac=difffrac-setasidestand->frac;
          reclaim_land(setasidestand_ir,cutstand,cell,istimber,npft+ncft);
          setasidestand_ir->frac-=difffrac-setasidestand->frac;
          mixsetaside(setasidestand,cutstand,intercrop);
          delstand(cell->standlist,pos);
        }
      }
      if(difffrac>setasidestand->frac+epsilon)
      {
        factor=(difffrac-setasidestand->frac)/(cell->ml.cropfrac_rf+cell->ml.cropfrac_ir-setasidestand->frac);
        fprintf(stderr,"ERROR213: not enough setasidestand in (%g,%g) to put the reservoir, reduce cropland by %g\n",cell->coord.lon,cell->coord.lat,factor);
      if(factor>1.0+epsilon)
        fprintf(stderr,"ERROR187: factor=%g >1 in cell lon %.2f lat %.2f\n",
                factor,cell->coord.lon, cell->coord.lat);
      foreachstand(stand,s,cell->standlist)
        if(stand->type->landusetype!=NATURAL &&
           stand->type->landusetype!=SETASIDE_RF &&
           stand->type->landusetype!=SETASIDE_IR)
        {
          data=stand->data;
          pos=addstand(&natural_stand,cell)-1;
          cutstand=getstand(cell->standlist,pos);
          cutstand->frac=factor*stand->frac;
          reclaim_land(stand,cutstand,cell,istimber,npft+ncft);
          stand->frac-=cutstand->frac;

          cell->discharge.dmass_lake+=(data->irrig_stor+data->irrig_amount)*cell->coord.area*cutstand->frac;
          cell->balance.awater_flux-=(data->irrig_stor+data->irrig_amount)*cutstand->frac;
          
          if(setaside(cell,getstand(cell->standlist,pos),pftpar,intercrop,npft,FALSE))
            delstand(cell->standlist,pos);
        }
      }
    }
  }
  else /* if there IS no setaside in the the cell: */
  {
    factor=difffrac/(cell->ml.cropfrac_rf+cell->ml.cropfrac_ir);
    fprintf(stderr,"ERROR213: no setasidestand in cell (%g,%g) to put the reservoir, reduce cropland by %g\n",cell->coord.lon,cell->coord.lat,factor);
    foreachstand(stand,s,cell->standlist)
      if(stand->type->landusetype!=NATURAL) /*&&
         stand->type->landusetype!=SETASIDE)*/ /* existence of SETASIDES has been ruled out */
      {
        data=stand->data;
        pos=addstand(&natural_stand,cell)-1;
        cutstand=getstand(cell->standlist,pos);
        cutstand->frac=factor*stand->frac;
        reclaim_land(stand,cutstand,cell,istimber,npft+ncft);
        stand->frac-=cutstand->frac;
        
        cell->discharge.dmass_lake+=(data->irrig_stor+data->irrig_amount)*cell->coord.area*cutstand->frac;
        cell->balance.awater_flux-=(data->irrig_stor+data->irrig_amount)*cutstand->frac;
        
        if(setaside(cell,getstand(cell->standlist,pos),pftpar,intercrop,npft,FALSE))
          delstand(cell->standlist,pos);
      }

  }

  /* if previous step correct, there is now enough setaside
   * created to put the be replaced by the reservoir
   */
  s=findlandusetype(cell->standlist,SETASIDE_RF);
  setasidestand=getstand(cell->standlist,s);

  if(difffrac<=setasidestand->frac+epsilon)
  {
    pos=addstand(&natural_stand,cell)-1;
    cutstand=getstand(cell->standlist,pos);
    cutstand->frac=difffrac;
    reclaim_land(setasidestand,cutstand,cell,istimber,npft+ncft);
    setasidestand->frac-=difffrac;

    /* all the water from the cutstand goes in the reservoir */
    cell->ml.resdata->dmass+=soilwater(&cutstand->soil)*cutstand->frac*cell->coord.area;

    /* all the carbon from the reservoir goes into a c_pool */
    cell->ml.resdata->c_pool+=soilcarbon(&cutstand->soil)*cutstand->frac;

    /* delete the cutstand, and the setasidestand if empty */
    delstand(cell->standlist,pos);
    if(setasidestand->frac<epsilon)
      delstand(cell->standlist,s);

    if(factor>0.9999) /* If the total gridcell is occupied by the reservoir,
                         remove all stands */
      while(!isempty(cell->standlist))
        delstand(cell->standlist,0);

  }
  else
    fail(NO_SETASIDE_ERR,TRUE,
         "Not enough setaside stand created to put the reservoir in cell %.2f %.2f: diff=%g",
         cell->coord.lon,cell->coord.lat,difffrac-setasidestand->frac);

  return difffrac;
} /* of 'from setaside for reservoir' */

void landusechange_for_reservoir(Cell *cell,            /* pointer to cell */
                                 const Pftpar pftpar[], /* PFT parameter array */
                                 int npft,              /* number of natural PFTs */
                                 Bool istimber,
                                 Bool intercrop,
                                 int ncft)
/* needs to be called before establishment, to ensure that regrowth is possible in the
   following year */
{
  Real difffrac;
  Stand *stand;
  Real sum[2];
  Real minnatfrac_res;
  int s;
  Real totw_before,totw_after,totc_before,totc_after,balanceW,balanceC; /* to check the water and c balance in the cells */
  Irrigation *data;

  minnatfrac_res=0.0002;

  /* First check is a new dam causes fraction >1, natural stand is lowered with
   * dam area
   * if this is not possible the natural stand is removed and the setaside stands are reduced
   * if this is also not possible reduce the crops evenly untill there is space for the reservoir
   */
  if(check_stand_fracs_for_reservoir(cell,&difffrac))
  {
    //fprintf(stderr,"reservoir caused frac >1 in cell (%g/%g). diffrac = %.5f, reservoirfrac = %f\n",cell->coord.lon,cell->coord.lat,difffrac,cell->reservoirfrac);
    /* total water and carbon calculation before the correction of fractions
     * for reservoir water
     */
    totc_before=totc_after=0.0;
    totw_before=cell->balance.awater_flux;
    foreachstand(stand,s,cell->standlist)
      totw_before+=soilwater(&stand->soil)*stand->frac;
    totw_before+=(cell->discharge.dmass_lake)/cell->coord.area;
    totw_before+=cell->ml.resdata->dmass/cell->coord.area;
    totw_before+=cell->ml.resdata->dfout_irrigation/cell->coord.area;

    /* carbon */
    foreachstand(stand,s,cell->standlist)
      totc_before+=standcarbon(stand)*stand->frac;
    totc_before+=cell->ml.resdata->c_pool;


    /* cut cut entire natural stand if lakefraction+reservoir fraction equals 1 */
    if(difffrac>=1-cell->lakefrac-minnatfrac_res)
      minnatfrac_res=0.0;

    /* first choice: deforest natural land */
    if(difffrac>epsilon && (1-cell->lakefrac-cell->ml.cropfrac_rf-cell->ml.cropfrac_ir-minnatfrac_res)>=difffrac)
    {  /* deforestation to built the reservoir */
       s=findlandusetype(cell->standlist,NATURAL);
       if(s!=NOT_FOUND) deforest_for_reservoir(cell,difffrac,istimber,npft+ncft);
    }
    /* if this is not possible: deforest all the natural land and then reduce crops  */
    if(difffrac>epsilon && 1-cell->lakefrac-cell->ml.cropfrac_rf-cell->ml.cropfrac_ir-minnatfrac_res<difffrac)
    {
      s=findlandusetype(cell->standlist,NATURAL);
      if(s!=NOT_FOUND) /* check if there is still natural land in the gridcell */
        deforest(cell,difffrac,pftpar,intercrop,npft,FALSE,istimber,FALSE,ncft,minnatfrac_res); /* 1 deforest */
      s=findlandusetype(cell->standlist,NATURAL); /* 2 check if everyting is deforested */
      if(s!=NOT_FOUND)
      {
        stand=getstand(cell->standlist,s);
        if(stand->frac>minnatfrac_res+epsilon)
        {
          printf("defor res 1 wrong loop %d %g %g %g %g %g %g\n in cell lon %.2f lat %.2f\n",
                 s,difffrac,
                 1-cell->lakefrac-cell->ml.cropfrac_rf-cell->ml.cropfrac_ir,
                 cell->lakefrac,cell->ml.cropfrac_rf, cell->ml.cropfrac_ir,
                 cell->ml.reservoirfrac, cell->coord.lon, cell->coord.lat);
          fflush(stdout);
/*        fail(FOREST_LEFT_ERR,TRUE,
               "wrong loop, there is still natural land to deforest left"); */
      
          deforest(cell,difffrac,pftpar,intercrop,npft,FALSE,istimber,FALSE,ncft,minnatfrac_res); /* 1 deforest */
          s=findlandusetype(cell->standlist,NATURAL); /* 2 check if everyting is deforested */
          if(s!=NOT_FOUND)
          {
            printf("defor res 2 wrong loop %d %g %g %g %g %g %g\n in cell lon %.2f lat %.2f\n",
                   s,difffrac,
                   1-cell->lakefrac-cell->ml.cropfrac_rf-cell->ml.cropfrac_ir,
                   cell->lakefrac,cell->ml.cropfrac_rf, cell->ml.cropfrac_ir,
                   cell->ml.reservoirfrac, cell->coord.lon, cell->coord.lat);
            fflush(stdout);
            fail(FOREST_LEFT_ERR,TRUE,
                 "wrong loop, there is still natural land to deforest left");
          }
        }
      }

      /* update the cropfactor to include the 'new' setaside */
      sum[0]=sum[1]=0.0;
      foreachstand(stand,s,cell->standlist)
        if(stand->type->landusetype!=NATURAL){
          data=stand->data;
          if(data->irrigation)
            sum[1]+=stand->frac;
          else
            sum[0]+=stand->frac;
        }
      cell->ml.cropfrac_rf=sum[0];
      cell->ml.cropfrac_ir=sum[1];

      difffrac-=from_setaside_for_reservoir(cell,difffrac,pftpar,istimber,
                                            intercrop,npft,ncft);
      /*3 cut setaside stand to built the reservoir */

      /* update the cropfactor */
      sum[0]=sum[1]=0.0;
      foreachstand(stand,s,cell->standlist)
        if(stand->type->landusetype!=NATURAL){
          data=stand->data;
          if(data->irrigation)
            sum[1]+=stand->frac;
          else
            sum[0]+=stand->frac;
        }
      cell->ml.cropfrac_rf=sum[0];
      cell->ml.cropfrac_ir=sum[1];
    }
    /* total water and carbon calculation after the correction of fractions
     * for reservoir water
     */
    totw_after=cell->balance.awater_flux;
    foreachstand(stand,s,cell->standlist)
      totw_after+=soilwater(&stand->soil)*stand->frac;
    totw_after+=(cell->discharge.dmass_lake)/cell->coord.area;
    totw_after+=cell->ml.resdata->dmass/cell->coord.area;
    totw_after+=cell->ml.resdata->dfout_irrigation/cell->coord.area;

    /* carbon */
    foreachstand(stand,s,cell->standlist)
      totc_after+=standcarbon(stand)*stand->frac;
    totc_after+=cell->ml.resdata->c_pool;
    /* check if the same */
    balanceW=totw_before-totw_after;
    balanceC=totc_before-totc_after;

#ifndef IMAGE /*HB. Because the timber harvest is not accounted for in the carbon balance check*/
    if(fabs(balanceW)>0.01)
      fail(INVALID_WATER_BALANCE_ERR,TRUE,
           "balance error in the building of the reservoir, balanceW=%g",
           balanceW);
    if(fabs(balanceC)>2)
      fail(INVALID_CARBON_BALANCE_ERR,TRUE,
           "balance error in the building of the reservoir, balanceC=%g",
           balanceC);
#endif
    /* check if total fractions add up to 1 again */
    check_stand_fracs(cell->standlist,cell->lakefrac+cell->ml.reservoirfrac);

  }
  /* End of added by HB jan 09 */
} /* end of function */
