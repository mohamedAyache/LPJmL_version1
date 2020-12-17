/***************************************************************************/
/**                                                                       **/
/**   u  p  d  a  t  e  _  o  u  t  p  u  t  d  a  i  l  y  .  c          **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 25.09.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void update_outputdaily(Outputday *output,
                        const Real data[],
                        int total,
                        int day)
{
  int i;
  for(i=0;i<=D_PET-D_LAI;i++)
    if(output->data[i]!=NULL)
      output->data[i][output->count+day*total]=(float)data[i];
} /* of 'update_outputdaily' */
