/***************************************************************************/
/**                                                                       **/
/**               f  r  e  e  o  u  t  p  u  t  d  a  i  l  y  .  c       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 26.09.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void freeoutputdaily(Outputday *output)
{
  int i;
  for(i=0;i<=D_PET-D_LAI;i++)
    free(output->data[i]);
  free(output->data);
  free(output);
} /* of 'freeoutputdaily' */
