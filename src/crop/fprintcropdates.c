/***************************************************************************/
/**                                                                       **/
/**     f  p  r  i  n  t  _ c  r  o  p  d  a  t  e  s  .  c               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 11.02.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void fprintcropdates(FILE *file,const Cropdates cropdates[],int ncft)
{
    int cft;
    fprintf(file,"Cropdates:\n");
    fprintf(file,"Crop fall    last        spring  last          vern   last        fallow fallow\n"
            "     sdate20 update_fall sdate20 update_spring date20 update_vern        irrig\n"
            "---- ------- ----------- ------- ------------- ------ ----------- ------ ------\n");
    for(cft=0;cft<ncft;cft++)
        fprintf(file,"%4d %7d %11d %7d %13d %6d %11d %6d %6d\n",
                cft,
                cropdates[cft].fall_sdate20,cropdates[cft].last_update_fall,
                cropdates[cft].spring_sdate20,cropdates[cft].last_update_spring,
                cropdates[cft].vern_date20,cropdates[cft].last_update_vern,
                cropdates[cft].fallow,cropdates[cft].fallow_irrig);
    fprintf(file,"---- ------- ----------- ------- ------------- ------ ----------- ------ ------\n");
} /* of 'fprintcropdates' */
