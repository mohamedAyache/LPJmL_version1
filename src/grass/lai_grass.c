/***************************************************************************/
/**                                                                       **/
/**             l  a  i  _  g  r  a  s  s  .  c                           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 15.06.2004                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"

Real lai_grass(const Pft *pft)
{
  return ((Pftgrass *)pft->data)->ind.leaf*getpftpar(pft,sla);
} /* 'lai_grass' */
/*
- this function is called in fpc_grass() and returns the maximum lai of a grass (last year's LAI)
*/
Real actual_lai_grass(const Pft *pft)
{
  return ((Pftgrass *)pft->data)->ind.leaf*getpftpar(pft,sla)*pft->phen;
} /* 'actual_lai_grass' */
