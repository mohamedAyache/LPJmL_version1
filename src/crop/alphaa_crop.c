/***************************************************************************/
/**                                                                       **/
/**                 a  l  p  h  a  a  _  c  r  o  p  .  c                 **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Christoph Mueller                                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 12.06.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "crop.h"

Real alphaa_crop(const Pft *pft)
{
    Real laimax;
    if(pft->stand->cell->ml.manage.laimax==NULL)                             
        laimax=0;                                                           
    else
        laimax=pft->stand->cell->ml.manage.laimax[pft->par->id];
    laimax= (laimax<=7) ? laimax : 7;
    /* learning from AgMIP, MAIZE reaches highest intensity level at LAImax=5*/
    if(pft->par->id==MAIZE)
        return min(1,pft->par->alphaa-(0.15*(5-laimax)));
    else
        return pft->par->alphaa-(0.1*(7-laimax));
} /* of 'alphaa_crop' */
