/***************************************************************************/
/**                                                                       **/
/**             n  e  w  _  c  r  o  p  .  c                              **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     Function allocates and initializes crop-specific variables        **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 25.01.2006     by S.Jachner, M.Gumpenberger          **/
/**     Last change: 07.10.2009     by W. von Bloh                        **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "crop.h"

void new_crop(Pft *pft)
{
    Pftcrop *crop;
    Pftcroppar *par;
    crop=new(Pftcrop);
    check(crop);
    pft->data=crop;
    par=pft->par->data;

    pft->fpc=1;
    pft->nind=1;

    crop->growingdays=0;

    crop->vdsum=crop->husum=crop->fphu=0.0;
    crop->ind.so=0.0;
    pft->bm_inc=20.0;
    crop->laimax_adjusted=1; 
    pft->flaimax=0.000083; 
    crop->demandsum=0;
    crop->supplysum=0;

    crop->lai=crop->lai000=pft->flaimax*par->laimax;
    crop->lai_nppdeficit=0.0;
    pft->phen=crop->lai/par->laimax;
    crop->senescence=FALSE;

    allocation_daily_crop(pft,0,NO_WDF,NULL); 
} /* of 'new_crop' */

/*
   - new_crop() is called when a new crop is added to a stand (see newpft.c)
   - sets fpc 1 for crops
   - provides memory place for specific crop variables
   -> initialisation of specific crop variables (see crop.h)
   */
