/***************************************************************************/
/**                                                                       **/
/**       a  g  r  i  c  u  l  t  u  r  e    _  t  r  e  e  .  c          **/
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
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "natural.h"
#include "agriculture.h"
#include "tree.h"
#include "agriculture_tree.h"

Standtype agriculture_tree_stand={AGRICULTURE_TREE,"agriculture_tree",
                                  new_agriculture,
                                  free_agriculture,fwrite_agriculture,
                                  fread_agriculture,fprint_agriculture,
                                  daily_agriculture_tree,
                                  annual_agriculture_tree,NULL};
