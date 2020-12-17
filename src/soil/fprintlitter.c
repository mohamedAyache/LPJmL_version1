/***************************************************************************/
/**                                                                       **/
/**                f  p  r  i  n  t  l  i  t  t  e  r  .  c               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-14 09:09:50 +0200 (mar., 14 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void fprintlitter(FILE *file,const Litter *litter)
{
  int i,p;
  fputs("\n\tAvg. fbd:\t",file);
  for(i=0;i<NFUELCLASS+1;i++)
    fprintf(file,"%g ",litter->avg_fbd[i]);
  fputs("\n\tLitter carbon (gC/m2)\n"
        "\tPFT                                      below   leaf   ",file);
  for(i=0;i<NFUELCLASS;i++)
    fprintf(file," wood(%d)",i);
  fputs("\n\t---------------------------------------- ------- -------",file);
  for(i=0;i<NFUELCLASS;i++)
    fputs(" -------",file);
  fputc('\n',file);
  for(p=0;p<litter->n;p++)
  {
    fprintf(file,"\t%-40s %7.2f %7.2f",litter->ag[p].pft->name,
            litter->bg[p],litter->ag[p].trait.leaf);
    for(i=0;i<NFUELCLASS;i++)
      fprintf(file," %7.2f",litter->ag[p].trait.wood[i]);
    fputc('\n',file);
  }
  fputs("\t---------------------------------------- ------- -------",file);
  for(i=0;i<NFUELCLASS;i++)
    fputs(" -------",file);
} /* of 'fprintlitter' */
