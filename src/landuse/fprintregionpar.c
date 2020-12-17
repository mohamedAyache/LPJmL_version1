/***************************************************************************/
/**                                                                       **/
/**        f  p  r  i  n  t  r  e  g  i  o  n  p  a  r  .  c              **/
/**                                                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 03.03.2011, Werner von Bloh                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void fprintregionpar(FILE *file,const Regionpar regionpar[],int nregions)
{
  int i;
  fputs("Region                                       bifratio fuelratio woodconsum\n"
        "-------------------------------------------- -------- --------- ----------\n",file);
  for(i=0;i<nregions;i++)
   fprintf(file,"%44s %8.5f %9.5f %10.5f\n",regionpar[i].name,
           regionpar[i].bifratio,regionpar[i].fuelratio,regionpar[i].woodconsum);
  fputs("-------------------------------------------- -------- --------- ----------\n",file);
} /* of 'fprintregionpar' */
