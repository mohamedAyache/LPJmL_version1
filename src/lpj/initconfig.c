/***************************************************************************/
/**                                                                       **/
/**                   i  n  i  t  c  o  n  f  i  g  .  c                  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function initializes Config struct for sequential version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 02.12.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void initconfig(Config *config)
{
  /* setup for sequential version */
  config->rank=0;
  config->ntask=1;
} /* of 'initconfig' */
