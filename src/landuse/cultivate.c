/***************************************************************************/
/**                                                                       **/
/**            c  u  l  t  i  v  a  t  e  .  c                            **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
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
#include "agriculture.h"

Real cultivate(Cell *cell,           /* cell pointer */
               const Pftpar *pftpar, /* PFT parameter to be established */
               int vern_date20, 
               Real landfrac,        /* land fraction (0..1) */
               Bool irrigation,      /* irrigated (TRUE/FALSE) */
               int day,              /* day (1..365) */
               Bool wtype,           /* winter type (TRUE/FALSE) */
               Stand *setasidestand,
               Bool istimber,
               int irrig_scenario,   /* irrigation scenario */
               int ntotpft,          /* total number of PFTs */
               int cft               /* cft index for set_irrigsystem */
              )
{
  int pos; /*position of new stand in list*/
  Pft *pft;
  Stand *cropstand;
  Irrigation *data;
  if(landfrac>=setasidestand->frac-0.00001)
  {
    setasidestand->type->freestand(setasidestand);
    setasidestand->type=&agriculture_stand;
    new_agriculture(setasidestand);
    /* delete all PFTs */
    cutpfts(setasidestand);
    pos=addpft(setasidestand,pftpar);
    pft=getpft(&setasidestand->pftlist,pos-1);
    phen_variety(pft,vern_date20,cell->coord.lat,day,wtype);
    data=setasidestand->data;
    data->irrigation= irrig_scenario==ALL_IRRIGATION ? TRUE : irrigation;
    set_irrigsystem(setasidestand,cft,0,0,0,FALSE); /* calls set_irrigsystem() for landusetype AGRICULTURE only */
    return pft->bm_inc*setasidestand->frac;
  }
  else
  {
    pos=addstand(&agriculture_stand,cell);
    cropstand=getstand(cell->standlist,pos-1);
    data=cropstand->data;
    cropstand->frac=landfrac;
    data->irrigation= irrig_scenario==ALL_IRRIGATION ? TRUE : irrigation;
    
    reclaim_land(setasidestand,cropstand,cell,istimber,ntotpft);
    // barrier.n: update for soil nutrients
    nit_deforest_updatesoil(setasidestand, cropstand, landfrac);

    set_irrigsystem(cropstand,cft,0,0,0,FALSE);
    pos=addpft(cropstand,pftpar);
    pft=getpft(&cropstand->pftlist,pos-1);
    phen_variety(pft,vern_date20,cell->coord.lat,day,wtype);
    setasidestand->frac-=landfrac;
    return pft->bm_inc*cropstand->frac;
  }
} /* of 'cultivate' */

/*
- called in sowing()
- comparison of the land fraction (landfrac) of the considered cft with the 
  fraction of the set-aside stand (setasidestand->frac)
  -> is the land fraction of the cft greater or equal as the fraction of the
     set-aside stand:
  -> sets the landusetype of the set-aside stand to AGRICULTURE 
     (defined in stand.h)
  -> kills all pfts of the set-aside stand and updates the litter pools
  -> adds considered cft to the pftlist of the stand (see addpft() in 
     pftlist.h)
     (-> addpft() calls function newpft() (see newpft.c);
      -> newpft() calls specific functions (here new_crop.c, see below)) 
  -> creates a variable crop of type Pftcrop with the informations of the 
     crop-specific variables of the new cft (see getpft() in pftlist.h) 
     with the aim to change informations
  -> calls function phen_variety() (see below)
  -> sets wtype to TRUE or FALSE (this information comes from function sowing())
  -> sets irrigation to TRUE or FALSE (this information comes from function
     sowing())

  -> is the land fraction of the cft smaller as the fraction of the set-aside 
     stand
     -> adds a new stand to the standlist (see addstand() in standlist.c)
     -> addstand() returns the length of the standlist which is also the
        position of the new stand in the standlist
     -> creates a variable cropstand of type Stand with the informations of 
        the new stand (see getstand() in stand.h) with the aim to change 
        informations
     -> calls function reclaim_land()
     -> adds considered cft to the pftlist of the stand (see addpft() in 
        pftlist.h)
     -> creates a variable crop of type Pftcrop with the informations of the
        crop-specific variables of the new cft (see getpft() in pftlist.h) 
        with the aim to change informations
     -> calls function phen_variety() (see below)
     -> sets wtype to TRUE or FALSE (this information comes from function 
        sowing())
     -> sets the landusetype of the new cropstand to AGRICULTURE 
     -> sets irrigation to TRUE or FALSE (this information comes from function
        sowing())
     -> sets the frac of the new cropstand to the landfrac
     -> subtracts the frac of the set-aside stand with the landfrac
*/
