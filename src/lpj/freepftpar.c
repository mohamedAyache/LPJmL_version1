/***************************************************************************/
/**                                                                       **/
/**               f  r  e  e  p  f  t  p  a  r  .  c                      **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 03.03.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void freepftpar(Pftpar pftpar[], /* PFT parameter array */
                int ntotpft         /* total number of PFTs */
               )
{
  int i;
  for(i=0;i<ntotpft;i++)
  {
    free(pftpar[i].name);
    free(pftpar[i].data);
  }
  free(pftpar);
} /* of 'freepftpar' */
