/***************************************************************************/
/**                                                                       **/
/**                     i  n  i  t  g  d  d  .  c                         **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function initializes growing degree days array to zero            **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 09.11.2004                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void initgdd(Real gdd[],      /* Growing degree days array */
             int npft         /* Number of natural PFTs */
            )
{
  int p;
  /* Initialize gdd array to zero */
  for(p=0;p<npft;p++)
    gdd[p]=0;
} /* of 'initgdd' */
