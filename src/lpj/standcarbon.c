/***************************************************************************/
/**                                                                       **/
/**                s  t  a  n  d  c  a  r  b  o  n  .  c                  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function computes total carbon in stand                           **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 27.02.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real standcarbon(const Stand *stand /* pointer to stand */
                )                   /* returns carbon sum (g/m2) */
{
  int p;
  const Pft *pft;
  Real totc;
  totc=soilcarbon(&stand->soil); /* get carbon in soil */
  foreachpft(pft,p,&stand->pftlist)
    totc+=vegc_sum(pft); /* sum up carbon in PFTs */
  return totc;
} /* of 'standcarbon' */
