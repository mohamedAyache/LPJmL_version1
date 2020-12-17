/***************************************************************************/
/**                                                                       **/
/**     f  r  e  a  d   c  r  o  p  d  a  t  e  s  .  c                   **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 25.01.2006  by S.Jachner, M.Gumpenberger             **/
/**     Last change: $Date:: 2014-09-29 16:02:37 +0200 (lun., 29 sept. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Cropdates *freadcropdates(FILE *file, /* Pointer to binary file */
        int ncft,   /* Number of crop PFTs */
        Bool swap   /* byte order has to be changed */
        )            /* returns allocated cropdates */
{
    Cropdates *cropdates;
    cropdates=newvec(Cropdates,ncft);
    if(cropdates==NULL)
        return NULL;
    freadint((int *)cropdates,sizeof(Cropdates)/sizeof(int)*ncft,swap,file);
    /*for(i=0;i<ncft;i++)
      freadint(&cropdates[i].fall_sdate20,1,swap,file);
      for(i=0;i<ncft;i++)
      freadint(&cropdates[i].spring_sdate20,1,swap,file);
      for(i=0;i<ncft;i++)
      freadint(&cropdates[i].vern_date20,1,swap,file); */
    return cropdates;
} /* of 'freadcropdates' */
