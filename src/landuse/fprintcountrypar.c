/***************************************************************************/
/**                                                                       **/
/**     f  p  r  i  n  t  c  o  u  n  t  r  y  p  a  r  .  c              **/
/**                                                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-06-24 13:11:51 +0200 (mer., 24 juin 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void fprintcountrypar(FILE *file,const Countrypar *countrypar,int ncountry,
                      int ncft)
{
  char *irrigsys[]={"SURF","SPRINK","DRIP"};
  int i,j;
  fputs("Country                                     ",file);
  if(countrypar[0].laimax_cft!=NULL)
    for(i=0;i<ncft;i++)
      fprintf(file," %2d",i+1);
  else
    fputs(" cereal maize",file);
  fputs(" Irrigsys\n",file);
  fputs("--------------------------------------------",file);
  if(countrypar[0].laimax_cft!=NULL)
    for(i=0;i<ncft;i++)
      fputs(" --",file);
  else
    fputs(" ------ -----",file);
  fputs(" --------\n",file);
  for(i=0;i<ncountry;i++)
  {
    fprintf(file,"%44s",countrypar[i].name);
    if(countrypar[i].laimax_cft!=NULL)
      for(j=0;j<ncft;j++)
        fprintf(file," %2.0g",countrypar[i].laimax_cft[j]);
    else
      fprintf(file," %6.1g %5.1g",countrypar[i].laimax_tempcer,
              countrypar[i].laimax_maize);
    fprintf(file," %s\n",irrigsys[countrypar[i].default_irrig_system-1]);
  }
  fputs("--------------------------------------------",file);
  if(countrypar[0].laimax_cft!=NULL)
    for(i=0;i<ncft;i++)
      fputs(" --",file);
  else
    fputs(" ------ -----",file);
  fputs(" --------\n",file);
} /* of 'fprintcountrypar' */
