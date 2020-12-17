/***************************************************************************/
/**                                                                       **/
/**       c  a  l  c  _   c  r  o  p  d  a  t  e  s  .  c                 **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 25.01.2006  by S.Jachner, M.Gumpenberger             **/
/**     Last change: 02.08.2010  by W. von Bloh                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void update_fallowdays(Cropdates *cropdates,   /* crop dates */
        Real lat,               /* Latitude (deg) */
        int day,                /* day (1..365) */
        int ncft               /* number of crop PFTs */
        )
{
    int cft;

    for(cft=0;cft<ncft;cft++)
    {

        cropdates[cft].fallow--;
        cropdates[cft].fallow_irrig--;

        if((lat>=0.0 && day==COLDEST_DAY_NHEMISPHERE) ||
                (lat<0.0 && day==COLDEST_DAY_SHEMISPHERE)) 
            cropdates[cft].fallow=cropdates[cft].fallow_irrig=0;
    }
} /* of 'update_fallowdays' */

/*local function update_fallowdays() is called in update_daily()*/
