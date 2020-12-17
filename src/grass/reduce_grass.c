/***************************************************************************/
/**                                                                       **/
/**               r  e  d  u  c  e  _  g  r  a  s  s  .  c                **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 29.04.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"

void reduce_grass(Litter * UNUSED(litter),Pft *pft,Real factor) 
{
  pft->fpc/=factor;
  
} /* of 'reduce_grass' */

/***************************************************************************/
/**                                                                       **/
/** CALLED FROM:                                                          **/
/**   establishment.c (foreachpft)                                        **/
/** PURPOSE:                                                              **/
/**   reduce fpc by factor                                                **/
/**                                                                       **/
/***************************************************************************/
