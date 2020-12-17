/***************************************************************************/
/**                                                                       **/
/**                       c  u  t  p  f  t  s  .  c                       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function deletes all PFTs and transfers carbon to litter          **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 05.02.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void cutpfts(Stand *stand /* Pointer to stand */
            )
{
  int p;
  Pft *pft;
  foreachpft(pft,p,&stand->pftlist)
    litter_update(&stand->soil.litter,pft,pft->nind);
  freepftlist(&stand->pftlist);
} /* of 'cutpfts' */
