/***************************************************************************/
/**                                                                       **/
/**         u  p  d  a  t  e  _  c  r  o  p  d  a  t  e  s  .  c          **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 15.03.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void update_cropdates(Cropdates cropdates[],int ncft)
{
    int cft;
    for(cft=0;cft<ncft;cft++)
        if(cropdates[cft].last_update_fall>=0)
        {
            cropdates[cft].last_update_fall++;
            cropdates[cft].last_update_spring++;
            cropdates[cft].last_update_vern++;

            if(cropdates[cft].last_update_fall>10)
                cropdates[cft].fall_sdate20=0;
            if(cropdates[cft].last_update_spring>10)
                cropdates[cft].spring_sdate20=0;
            if(cropdates[cft].last_update_vern>10)
                cropdates[cft].vern_date20=0;
        }
} /* 'update_cropdates' */
