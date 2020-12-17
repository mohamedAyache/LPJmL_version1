/***************************************************************************/
/**                                                                       **/
/**                 f  p  a  r  _  t  r  e  e  .  c                       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 26.03.2006                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

Real fpar_tree(const Pft *pft)
{
  return pft->phen*pft->fpc;
} /* of 'fpar_tree' */
