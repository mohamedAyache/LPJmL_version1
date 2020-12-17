/***************************************************************************/
/**                                                                       **/
/**     i  n  i  t  _ c  r  o  p  d  a  t  e  s  .  c                     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-09-29 16:02:37 +0200 (lun., 29 sept. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Cropdates *init_cropdates(const Pftpar par[],int ncft,Real lat)
{

    const Pftcroppar *croppar;
    Cropdates *cropdates;
    int cft;
    cropdates=newvec(Cropdates,ncft);
    if(cropdates==NULL)
        return NULL;
    for(cft=0;cft<ncft;cft++)
    {

        croppar=par[cft].data;

        cropdates[cft].last_update_fall=cropdates[cft].last_update_spring=cropdates[cft].last_update_vern=0;  
        cropdates[cft].fall_sdate20=cropdates[cft].spring_sdate20=cropdates[cft].vern_date20=0;
        cropdates[cft].fallow=cropdates[cft].fallow_irrig=0;

        if(croppar->calcmethod_sdate==NO_CALC_SDATE || croppar->calcmethod_sdate==MULTICROP)
        {
            cropdates[cft].last_update_fall=cropdates[cft].last_update_spring=NODATA;
            cropdates[cft].fall_sdate20=(lat>=0) ? croppar->initdate.sdatenh  : croppar->initdate.sdatesh;
        }
    }
    return cropdates;
} /* of 'init_cropdates' */

/*
   - called in newgrid()
   - initialisation of the 3 cropdates + related counters and fallow to zero
   - for group NO_CALC_SDATE & MULTICROP the counters last_update_fall &
   last_update_spring are set to -999 and the cropdate fall_sdate20 is set to the 
   initialised sowing date
   */
