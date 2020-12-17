/***************************************************************************/
/**                                                                       **/
/**               f  w  r  i  t  e  l  i  t  t  e  r  .  c                **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function writes litter variables into binary file                 **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 20.10.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool fwritelitter(FILE *file,const Litter *litter)
{
  Byte b;
  int l;
  fwrite(litter->avg_fbd,sizeof(Real),NFUELCLASS+1,file);
  b=(Byte)litter->n;
  fwrite1(&b,sizeof(b),file);
  for(l=0;l<litter->n;l++)
  {
    b=(Byte)litter->ag[l].pft->id;
    fwrite1(&b,sizeof(b),file);
    fwrite1(&litter->ag[l].trait,sizeof(Trait),file);
    fwrite1(&litter->bg[l],sizeof(Real),file);
  }
  return FALSE;
} /* of 'fwritelitter' */
