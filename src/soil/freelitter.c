/***************************************************************************/
/**                                                                       **/
/**                f  r  e  e  l  i  t  t  e  r  .  c                     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function deallocates memory for litter pools                      **/
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

void freelitter(Litter *litter)
{
  if(litter->n)
  {
    free(litter->ag);
    free(litter->bg); 
  }
  litter->n=0;
  litter->ag=NULL;
  litter->bg=NULL;
} /* of 'freelitter' */
