/***************************************************************************/
/**                                                                       **/
/**             i  n  i  t  _  g  r  a  s  s  .  c                        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 16.08.2005                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"

void init_grass(Pft *pft)
{
  pft->bm_inc=0;
  pft->nind=1;
#ifdef DAILY_ESTABLISHMENT
  pft->established=FALSE;
#endif
} /* of 'init_grass' */
