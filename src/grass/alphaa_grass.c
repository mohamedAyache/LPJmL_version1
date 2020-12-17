/***************************************************************************/
/**                                                                       **/
/**              a  l  p  h  a  a  _  g  r  a  s  s  .  c                 **/
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
#include "grass.h"

Real alphaa_grass(const Pft *pft)
{
  return pft->par->alphaa;
} /* of 'alphaa_grass' */
