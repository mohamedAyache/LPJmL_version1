/***************************************************************************/
/**                                                                       **/
/**                 f  r  e  e  _  c  r  o  p  .  c                       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     Function deallocates crop-specific PFT data                       **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 25.11.2005                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "crop.h"

void free_crop(Pft *pft)
{
  free(pft->data);
} /* of 'free_crop' */
