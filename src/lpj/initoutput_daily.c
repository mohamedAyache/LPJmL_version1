/***************************************************************************/
/**                                                                       **/
/**        i  n  i  t  o  u  t  p  u  t  _  d  a  i  l  y  .  c           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function initializes monthly output data to zero                  **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-04-23 17:53:09 +0200 (mer., 23 avril #$ **/
/**     By         : $Author:: sibylls                         $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void initoutput_daily(Daily_outputs *output)
{
  output->cleaf=output->cpool=output->croot=output->cso=output->evap=
    output->fphu=output->froot=output->gpp=output->gresp=
    output->growingday=output->hi=output->himind=output->fhiopt=
    output->husum=output->irrig=output->lai=output->laimaxad=
    output->lainppdeficit=output->npp=output->perc=
    output->phen=output->phu=output->prec=output->pvd=output->rd=
    output->rpool=output->rroot=output->rso=output->trans=
    output->vdsum=output->w0=output->w1=output->wdf=output->wevap=
    output->wscal=output->temp=output->sun=output->par=output->pet=
    output->daylength=output->swe=output->discharge=output->runoff=
    output->rh=output->interc=0.0;
} /* of 'initoutput_daily' */
