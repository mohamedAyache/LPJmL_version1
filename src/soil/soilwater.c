/***************************************************************************/
/**                                                                       **/
/**                s  o  i  l  w  a  t  e  r   .  c                       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function computes total water in soil                             **/
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

Real soilwater(const Soil *soil)
{
  Real totw;
  int l;
  totw=soil->snowpack;
  foreachsoillayer(l)
    totw+=soil->w[l]*soil->par->whcs[l]+soil->ice_depth[l]+soil->w_fw[l]+soil->ice_fw[l];
  totw+=soil->rw_buffer;
  return totw;
} /* of 'soilwater' */
