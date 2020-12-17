/***************************************************************************/
/**                                                                       **/
/**           n  e  w  o  u  t  p  u  t  d  a  i  l  y  .  c              **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 22.02.2010                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Outputday *newoutputdaily(Outputfile *outputfile,
                          int count)
{
  int i,index;
  Outputday *output;
  output=new(Outputday);
  check(output);
  index=0;
  output->data=newvec(float *,D_PET-D_LAI+1);
  check(output->data);
  for(i=D_LAI;i<=D_PET;i++)
  {
    if(isopen(outputfile,i))
    {
      output->data[index]=newvec(float,count*NDAYYEAR);
      check(output->data[index]);
    }
    else
      output->data[index]=NULL;
    index++;
  }
  output->count=0;
  return output;
} /* of 'newoutputdaily' */
