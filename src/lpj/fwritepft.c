/***************************************************************************/
/**                                                                       **/
/**                 f  w  r  i  t  e  p  f  t  .  c                       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/*      Function writes PFT variables into binary file                    **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 19.10.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"


Bool fwritepft(FILE *file,    /* File pointer to binary file */
               const Pft *pft /* PFT variables */
              )               /* returns TRUE on error */
{
  Byte b;
  b=(Byte)pft->par->id;
  fwrite1(&b,sizeof(b),file);
  fwrite1(&pft->wscal,sizeof(Real),file);
  fwrite1(&pft->wscal_mean,sizeof(Real),file);
  fwrite1(&pft->aphen,sizeof(Real),file);
  fwrite1(&pft->phen,sizeof(Real),file);
  fwrite1(&pft->flaimax,sizeof(Real),file);
  /* write type-dependent PFT variables */
  if(pft->par->fwrite(file,pft))
    return TRUE;
  fwrite1(&pft->bm_inc,sizeof(Real),file);
  fwrite1(&pft->nind,sizeof(Real),file);
  fwrite1(&pft->gdd,sizeof(Real),file); 
  fwrite1(&pft->fpc,sizeof(Real),file);
  b=(Byte)pft->litter;
  fwrite1(&b,sizeof(b),file);
  return FALSE;
} /* of 'fwritepft' */
