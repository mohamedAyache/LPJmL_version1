/***************************************************************************/
/**                                                                       **/
/**         n  o  e  s  t  a  b  l  i  s  h  m  e  n  t  .  c             **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     Default establishment function                                    **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 25.11.2005                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real noestablishment(Pft * UNUSED(pft),
                     Real UNUSED(fpc_total),
                     Real UNUSED(fpc_tree),
                     int UNUSED(n_est)
                    )
{
  return 0;
} /* of 'noestablishment' */
