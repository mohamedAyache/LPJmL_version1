/***************************************************************************/
/**                                                                       **/
/**         a  g  r  i  c  u  l  t  u  r  e  _  g  r  a  s  s  . c        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Definition of agriculture grass  stand                            **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "agriculture.h"
#include "grassland.h"
#include "agriculture_grass.h"

Standtype agriculture_grass_stand={AGRICULTURE_GRASS,"agriculture_grass",
                                   new_agriculture,
                                   free_agriculture,fwrite_agriculture,
                                   fread_agriculture,fprint_agriculture,
                                   daily_agriculture_grass,annual_agriculture_grass,NULL};
