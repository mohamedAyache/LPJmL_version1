/***************************************************************************/
/**                                                                       **/
/**     i  n  i  t  c  l  i  m  a  t  e  _  m  o  n  t  h  l  y  .  c     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  14.09.2008                                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void initclimate_monthly(const Climate *climate, /* Pointer to climate data */
                         Climbuf *climbuf,
                         int cell,               /* cell index */
                         int month               /* month (0..11) */
                        )
{
  if(!climate->file_prec.isdaily && israndomprec(climate))
    prdaily(climbuf->dval_prec,ndaymonth[month],
            (getcellprec(climate,cell))[month],
            (getcellwet(climate,cell))[month]);
  climbuf->mtemp=climbuf->mprec=0;
} /* of 'initclimate_monthly' */
