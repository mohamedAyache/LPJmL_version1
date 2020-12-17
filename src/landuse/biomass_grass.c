/***************************************************************************/
/**                                                                       **/
/**                     b  i  o  m  a  s  s  _  g  r  a  s  s  . c        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Definition of biomass stand                                       **/
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
#include "biomass_grass.h"

Standtype biomass_grass_stand={BIOMASS_GRASS,"biomass_grass",new_agriculture,
                               free_agriculture,fwrite_agriculture,
                               fread_agriculture,fprint_agriculture,
                               daily_biomass_grass,annual_biomass_grass,NULL};
