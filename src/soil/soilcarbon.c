/***************************************************************************/
/**                                                                       **/
/**                s  o  i  l  c  a  r  b  o  n  .  c                     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function computes total carbon in soil                            **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 23.02.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real soilcarbon(const Soil *soil)
{
  Real carbon=0;
  int l;
  forrootsoillayer(l)
    carbon+=soil->cpool[l].slow+soil->cpool[l].fast;
  carbon+=soil->YEDOMA;
  return carbon+littersum(&soil->litter);
} /* of 'soilcarbon' */

Real soilcarbon_slow(const Soil *soil)
{
  Real carbon=0;
  int l;
  forrootsoillayer(l)
     carbon+=soil->cpool[l].slow;
  return carbon;
} /* of 'soilcarbon_slow' */
