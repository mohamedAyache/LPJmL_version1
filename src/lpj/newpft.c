/***************************************************************************/
/**                                                                       **/
/**                       n  e  w  p  f  t  .  c                          **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function allocates and initializes PFT variables                  **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 07.03.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void newpft(Pft *pft,            /* PFT variables */
            Stand *stand,        /* Stand pointer */
            const Pftpar *pftpar /* Parameter of PFT */
           )
{
  int index;
  pft->par=pftpar;
  /* find litter pool for PFT */
  index=findlitter(&stand->soil.litter,pftpar);
  if(index==NOT_FOUND)
  {
    /* not found, add new litter pool for PFT */
    pft->litter=addlitter(&stand->soil.litter,pftpar)-1;
  }
  else
    /* save index of litter pool */
    pft->litter=index;
  /* Initialize variables to zero */
  pft->stand=stand;
  pft->fpc=pft->nind=pft->wscal=pft->aphen=pft->bm_inc=pft->wscal_mean=
           pft->gdd=pft->phen=0.0; 
#ifdef DAILY_ESTABLISHMENT
  pft->established=FALSE;
#endif
  pft->par->newpft(pft); /* type-specific allocation of memory */
} /* of 'newpft' */
