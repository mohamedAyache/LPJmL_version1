/***************************************************************************/
/**                                                                       **/
/**       o  u  t  p  u  t  _  d  a  i  l  y  _  c  r  o  p  .  c         **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     See file AUTHORS for list of authors contributing to this code    **/
/**                                                                       **/
/**     Last change: 12.10.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "crop.h"

void output_daily_crop(Daily_outputs* output,const  Pft* pft,Real gpp,Real rd)
{
    const Pftcrop *crop;
    crop=pft->data;
    output->lai=crop->lai;
    output->laimaxad=crop->laimax_adjusted;
    output->lainppdeficit=crop->lai_nppdeficit;
    output->husum=crop->husum;
    output->vdsum=crop->vdsum;
    output->fphu=crop->fphu;
    output->phen=pft->phen;
    output->growingday=crop->growingdays;
    output->pvd=crop->pvd;
    output->phu=crop->phu;
    output->gpp=gpp;
    output->rd=rd;
} /* of 'output_daily_crop' */

