/***************************************************************************/
/**                                                                       **/
/**                      g  e  t  m p r e c  .  c                         **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function getmprec gets monthly value of precipitation             **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  14.10.2008                                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real getmprec(const Climate *climate, /* pointer to climate data */
              const Climbuf *climbuf,
              int cell,                /* cell index */
              int month                /* month (0..11) */
             )             /* returns monthly averaged precipitation (mm) */
{
  return (climate->file_prec.isdaily) ? climbuf->mprec : (getcellprec(climate,cell))[month];
} /* of 'getmprec' */
