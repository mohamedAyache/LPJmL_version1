/***************************************************************************/
/**                                                                       **/
/**                    n  o  w  d  f  .  c                                **/
/**                                                                       **/
/**     Default function for calculation of water deficit fraction        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 08.10.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real nowdf(Pft * UNUSED(pft),Real UNUSED(demand),Real UNUSED(supply))
{
  return NO_WDF;
} /* of 'nowdf' */
