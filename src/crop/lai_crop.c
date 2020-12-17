/***************************************************************************/
/**                                                                       **/
/**                  l  a  i  _  c  r  o  p  .  c                         **/
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

Real lai_crop(const Pft *pft)
{
    Pftcrop *crop;
    crop=pft->data;

    return max(0,(crop->lai-crop->lai_nppdeficit));
} /* of 'lai_crop' */
/*
   - this function is not called, pendant to lai_tree()/lai_grass()
   */

Real actual_lai_crop(const Pft *pft)
{
    Pftcrop *crop;
    crop=pft->data;

    return max(0,(crop->lai-crop->lai_nppdeficit));
} /* of 'actual_lai_crop' */

/*
   - this function is called in interception() and returns the actual lai of a cft 
   */
