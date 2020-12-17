/***************************************************************************/
/**                                                                       **/
/**               r  e  d  u  c  e  _  t  r  e  e  .  c                   **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 29.04.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

void reduce_tree(Litter *litter,Pft *pft,Real factor) 
{
  pft->nind/=factor;
  pft->fpc/=factor;
  litter_update_tree(litter,pft,pft->nind*factor-pft->nind); 
  
} /* of 'reduce_tree' */

/***************************************************************************/
/**                                                                       **/
/** CALLED FROM:                                                          **/
/**   establishment.c (foreachpft)                                        **/
/** PURPOSE:                                                              **/
/**   reduce nind and fpc by a factor                                     **/
/** DETAILED DESCRIPTION:                                                 **/
/**   litter_update_tree is used to transfer the "waste" carbon into the  **/
/**   litter pool in order to maintain carbon balance                     **/
/**                                                                       **/
/***************************************************************************/
