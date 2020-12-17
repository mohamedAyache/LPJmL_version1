/***************************************************************************/
/**                                                                       **/
/**                     n  a  t  u  r  a  l  .  c                         **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Definition of natural stand                                       **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 11.06.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "natural.h"

Standtype natural_stand={NATURAL,"natural",new_natural,free_natural,
                         fwrite_natural,fread_natural,fprint_natural,
                         daily_natural,annual_natural,dailyfire};
