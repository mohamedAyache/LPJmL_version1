/***************************************************************************/
/**                                                                       **/
/**                   f  r  e  e  i  n  p  u  t  .  c                     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function deallocates input data                                   **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 07.03.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void freeinput(Input input,Bool isroot)
{
  freeclimate(input.climate,isroot);
  if(input.wateruse!=NULL)
    freewateruse(input.wateruse,isroot);
  if(input.landuse!=NULL)
    freelanduse(input.landuse,isroot);
  if(input.popdens!=NULL)
    freepopdens(input.popdens,isroot);
} /* of 'freeinput' */
