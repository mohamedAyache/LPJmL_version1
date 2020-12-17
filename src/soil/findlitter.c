/***************************************************************************/
/**                                                                       **/
/**                f  i  n  d  l  i  t  t  e  r  .  c                     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 23.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

int findlitter(const Litter *litter,const Pftpar *pft)
{
  int l;
  for(l=0;l<litter->n;l++)
    if(litter->ag[l].pft==pft)
      return l;
  return NOT_FOUND;
} /* of 'findlitter' */
