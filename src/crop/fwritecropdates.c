/***************************************************************************/
/**                                                                       **/
/**        f  w  r  i  t  e  c  r  o  p  d  a  t  e  s  .  c              **/
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

Bool fwritecropdates(FILE *file,const Cropdates *cropdates,int ncft)
{
    return fwrite(cropdates,sizeof(Cropdates),ncft,file)!=ncft;
    /*for(i=0;i<ncft;i++)
      fwrite(&cropdates[i].fall_sdate20,sizeof(int),1,file);
      for(i=0;i<ncft;i++)
      fwrite(&cropdates[i].spring_sdate20,sizeof(int),1,file);
      for(i=0;i<ncft;i++)
      fwrite(&cropdates[i].vern_date20,sizeof(int),1,file); */
} /* of 'fwritecropdates' */
