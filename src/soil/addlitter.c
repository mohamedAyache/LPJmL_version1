/***************************************************************************/
/**                                                                       **/
/**                a  d  d  l  i  t  t  e  r  .  c                        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**     Function adds PFT-specific above ground litter pool               **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 15.03.2010                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

int addlitter(Litter *litter,const Pftpar *pft)
{
  int i;
  litter->ag=(Litteritem *)realloc(litter->ag,
                                   sizeof(Litteritem)*(litter->n+1));
  check(litter->ag);
  litter->bg=(Real *)realloc(litter->bg,
                                   sizeof(Real)*(litter->n+1));
  check(litter->bg);
  litter->ag[litter->n].pft=pft;
  litter->ag[litter->n].trait.leaf=0;
  litter->bg[litter->n]=0;
  for(i=0;i<NFUELCLASS;i++)
    litter->ag[litter->n].trait.wood[i]=0;
  litter->n++;
  return litter->n;
} /* of 'addlitter' */
