/***************************************************************************/
/**                                                                       **/
/**              a  l  p  h  a  a  _  t  r  e  e  .  c                    **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Christoph Mueller                                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 12.06.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

Real alphaa_tree(const Pft *pft)
{
  return pft->par->alphaa;
} /* of 'alphaa_tree' */
