/***************************************************************************/
/**                                                                       **/
/**                    b  i  o  m  a  s  s  _  t  r  e  e  .  c           **/
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
#include "natural.h"
#include "agriculture.h"
#include "tree.h"
#include "biomass_tree.h"

Standtype biomass_tree_stand={BIOMASS_TREE,"biomass_tree",new_agriculture,
                              free_agriculture,fwrite_agriculture,
                              fread_agriculture,fprint_agriculture,
                              daily_biomass_tree,annual_biomass_tree,NULL};
