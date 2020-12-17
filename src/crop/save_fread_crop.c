/***************************************************************************/
/**                                                                       **/
/**                f  r  e  a  d  _  c  r  o  p  .  c                     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     Function reads crop-specific variables from binary file           **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 14.10.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "crop.h"

Bool fread_crop(FILE *file, /* file pointer */
        Pft *pft,   /* PFT data to be read */
        Bool swap   /* if true data is in different byte order */
        )            /* returns TRUE on error */
{
    Pftcrop *crop;
    crop=new(Pftcrop);
    pft->data=crop;
    if(crop==NULL)
        return TRUE;
    freadint1(&crop->wtype,swap,file);
    freadint1(&crop->growingdays,swap,file);
    freadreal1(&crop->pvd,swap,file);
    freadreal1(&crop->phu,swap,file);
    freadreal1(&crop->basetemp,swap,file);
    freadint1(&crop->senescence,swap,file);
    freadint1(&crop->senescence0,swap,file);
    freadreal1(&crop->husum,swap,file);
    freadreal1(&crop->vdsum,swap,file);
    freadreal1(&crop->fphu,swap,file);
    freadreal((Real *)&crop->ind,sizeof(Cropphys2)/sizeof(Real),swap,file);
    freadreal1(&crop->lai,swap,file);
    freadreal1(&crop->lai000,swap,file);
    freadreal1(&crop->laimax_adjusted,swap,file);
    freadreal1(&crop->lai_nppdeficit,swap,file);
    freadreal1(&crop->demandsum,swap,file);
    return freadreal1(&crop->supplysum,swap,file)!=1;
} /* of 'fread_crop' */
