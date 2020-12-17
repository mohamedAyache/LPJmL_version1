/***************************************************************************/
/**                                                                       **/
/**                    w  d  f  _  c  r  o  p  .  c                       **/
/**                                                                       **/
/**     Function calculates water deficit fraction                        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 08.10.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "crop.h"

Real wdf_crop(Pft *pft,    /* PFT variables */
        Real demand, /* water demand */
        Real supply  /* water supply */
        )             /* returns water deficit fraction (%) */
{
    Pftcrop *crop;
    crop=pft->data;
    crop->demandsum+=demand;
    crop->supplysum+=(supply>demand) ? demand : supply;
    return (crop->demandsum>0.0) ? 100.0*crop->supplysum/crop->demandsum: 100.0;
} /* of 'wdf_crop' */
