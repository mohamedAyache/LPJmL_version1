/***************************************************************************/
/**                                                                       **/
/**                 f  p  a  r  _  c  r  o  p  .  c                       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 23.10.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "crop.h"

Real fpar_crop(const Pft *pft)
{
    const Pftcrop *crop;
    crop=pft->data;
    if(pft->par->id==MAIZE)
        return min(1,max(0,0.2558*(crop->lai-crop->lai_nppdeficit)-0.0024));
    else
        return 1-exp(-param.k_beer*max(0,(crop->lai-crop->lai_nppdeficit)));
} /* of 'fpar_crop' */
