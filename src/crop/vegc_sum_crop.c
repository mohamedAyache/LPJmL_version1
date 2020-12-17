/***************************************************************************/
/**                                                                       **/
/**             v  e  g  c  _  s  u  m  _  c  r  o  p  .  c               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
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

Real vegc_sum_crop(const Pft *pft)
{
    const Pftcrop *crop;
    crop=pft->data;
    /*printf("vegc_sum_crop: %f nind: %f\n",phys_sum_crop(crop->ind)*pft->nind,pft->nind);*/
    return phys_sum_crop(crop->ind)*pft->nind;
} /* of 'vegc_sum_crop' */
