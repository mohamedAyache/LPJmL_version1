/***************************************************************************/
/**                                                                       **/
/**                     g  r  a  s  s  l  a  n  d  .  c                   **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Definition of grassland stand                                     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 08.10.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "agriculture.h"
#include "grassland.h"

Standtype grassland_stand={GRASSLAND,"grassland",new_agriculture,
                           free_agriculture,fwrite_agriculture,
                           fread_agriculture,fprint_agriculture,
                           daily_grassland,annual_grassland,NULL};
