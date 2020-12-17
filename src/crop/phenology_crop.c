/***************************************************************************/
/**                                                                       **/
/**            p  h  e  n  o  l  o  g  y  _  c  r  o  p  .  c             **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 03.05.2007  by S.Jachner, M.Gumpenberger             **/
/**     Last change: 22.08.2008  by S.Rost                                **/
/**     Last change: 23.10.2008  by J.Heinke                              **/
/**     Last change: 12.10.2009  by Werner von Bloh                       **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "crop.h"
#define VD_SLOPE 7 /* vernalization with reduced effects for +/- 7 deg outside min/max range*/

Bool phenology_crop(Pft *pft,       /* pointer to PFT variables */
        Real temp,      /* temperature (deg C) */
        Real daylength  /* length of day (h) */
        )                /* returns harvesting crop (TRUE/FALSE) */
{
    Pftcrop *crop;
    const Pftcroppar *par;
    Real hu,k,c;
    Real lai0,lai00;
    Real lai_inc=0;
    Real laimax;
    Bool harvesting;
    int hlimit;
    Real vd_inc,vrf,prf;

    crop=pft->data;
    par=pft->par->data;
    harvesting=FALSE;
    crop->growingdays++;

    hlimit=(crop->wtype && par->calcmethod_sdate==TEMP_WTYP_CALC_SDATE) ? par->hlimit+90 : par->hlimit; 

    crop->senescence0=crop->senescence;

    laimax=pft->stand->cell->ml.manage.laimax[pft->par->id];

    if(crop->husum<crop->phu)
    {
        hu=max(0,temp-crop->basetemp);
        /* Calculation of vernalization days */
        if (crop->vdsum<crop->pvd)
        {
            if (temp>par->trg.high)
                vd_inc=1-((temp-par->trg.high)/VD_SLOPE);
            else if (temp<par->trg.low)
                vd_inc=1-((par->trg.low-temp)/VD_SLOPE);
            else 
                vd_inc=1.0;
        }
        else
            vd_inc=0.0;
        crop->vdsum+=max(0,vd_inc);
        /* Calculation of vernalization reduction factor */
        if ((crop->vdsum<crop->pvd) && (crop->fphu<=0.2))
            vrf=(crop->vdsum<=10.0) ? 0.0 : (crop->vdsum-10.0) / (crop->pvd-10.0);
        else
            vrf = 1.0;

        /* Response to photoperiodic */
        if (crop->fphu <= par->fphusen)
            prf=(1-par->psens)*min(1,max(0,(daylength-par->pb)/(par->ps-par->pb)))+par->psens;
        else
            prf=1.0;

        /* Calculation of temperature sum (deg Cd) */
        crop->husum += hu * vrf * prf;

        /* fraction of growing season */
        crop->fphu=min(1.0,crop->husum/crop->phu);
        if(crop->fphu<par->fphusen)
        {
            /*k=(log(par->fphuc/par->flaimaxc-par->fphuc)-log(par->fphuk/par->flaimaxk-par->fphuk))
              /(par->fphuk-par->fphuc);
              c=log(par->fphuc/par->flaimaxc-par->fphuc)+k*par->fphuc;
              pft->flaimax=crop->fphu/(crop->fphu+exp(c-k*crop->fphu));*/
            c=par->fphuc/par->flaimaxc-par->fphuc;
            k=par->fphuk/par->flaimaxk-par->fphuk;
            pft->flaimax=crop->fphu/(crop->fphu+c*pow(c/k,(par->fphuc-crop->fphu)/(par->fphuk-par->fphuc)));
        }
        else
        {
            crop->senescence=TRUE;
            /*pft->flaimax=pow(1-crop->fphu,par->shapesenescencenorm)
              /pow(1-par->fphusen,par->shapesenescencenorm)
             *(1-par->flaimaxharvest)+par->flaimaxharvest;*/
            pft->flaimax=pow((1-crop->fphu)/(1-par->fphusen),par->shapesenescencenorm)
                *(1-par->flaimaxharvest)+par->flaimaxharvest;
        }
    }
    else
        harvesting=TRUE;

    if(crop->growingdays==hlimit)
        harvesting=TRUE;

    /* determination of lai */
    lai0=crop->lai;

    if(crop->senescence /* && !pft->stand->irrigation */)
    {
        if(!crop->senescence0)
            crop->laimax_adjusted=lai0;
        crop->lai=pft->flaimax*crop->laimax_adjusted;
    }
    else
        crop->lai=pft->flaimax*laimax;

    lai00=crop->lai000; 
    crop->lai000=crop->lai;

    if(!crop->senescence /* &&  !pft->stand->irrigation */)
    {
        lai_inc=(crop->lai-lai00)*pft->wscal;
        crop->lai=lai_inc+lai0;
    }

    pft->phen=crop->lai/laimax;

    return harvesting;
} /* of 'phenology_crop' */

/*
   - the function is called for each pft in update_daily():
   -> leaf_phenology(pft,temp,day,daylength,npft);
   - initialisation of local variables
   - daily computation of the phenological scalar between 0 at sowing and 1 at 
   maturity (harvest)
   -> calculation of hu (heat unit)
   -> calculation of vrf (vernalization reduction factor) dependent on pvd
   (vernalization days) to reduce hu (only for winter crops)
   -> calculation of prf (photoperiodism reduction factor) to reduce hu 
   (still inactive, yet)
   -> calculation of fphu (fraction of phenological heat unit)
   -> calculation of flaimax (fraction of maximum LAI) dependent of fphu
   -> two different shapes: - development and growth (increase)
   - senescence (decrease)
   -> harvest if phenological scalar reaches 1 or growingdays (number of days for
   the growing period; see hlimit in pft.par) are reached
   -> determination of lai (leaf area index) and pft->phen

   - returns TRUE on hdate, so that the landusetype will be set to KILL, the litter
   pools will be updated and the stand will be go to the set-aside stand 
   (see update_daily.c,setaside.c)
   */
