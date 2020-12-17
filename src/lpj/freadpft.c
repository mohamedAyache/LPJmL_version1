/***************************************************************************/
/**                                                                       **/
/**               f  r  e  a  d  p  f  t  .  c                            **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads PFT data from file                                 **/
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

Bool freadpft(FILE *file,            /* pointer to binary file */
              Stand *stand,          /* Stand pointer */
              Pft *pft,              /* PFT variables to read */
              const Pftpar pftpar[], /* PFT parameter array */
              int ntotpft,           /* total number of PFTs */
              Bool swap              /* Byte order has to be changed
                                        (TRUE/FALSE) */
             )                       /* returns TRUE on error */
{
  Byte id;
  if(fread(&id,sizeof(id),1,file)!=1)
    return TRUE;
  if(id>=ntotpft)
  {
    fprintf(stderr,"ERROR195: Invalid value %d for PFT index.\n",id); 
    return TRUE;
  }
  pft->par=pftpar+id;
  pft->stand=stand;
  freadreal1(&pft->wscal,swap,file);
  freadreal1(&pft->wscal_mean,swap,file);
  freadreal1(&pft->aphen,swap,file);
  freadreal1(&pft->phen,swap,file);
  freadreal1(&pft->flaimax,swap,file);
  /* read class-dependent PFT variables */
  if(pft->par->fread(file,pft,swap))
    return TRUE;
  freadreal1(&pft->bm_inc,swap,file);
  freadreal1(&pft->nind,swap,file);
  freadreal1(&pft->gdd,swap,file); 
  freadreal1(&pft->fpc,swap,file);
  if(fread(&id,sizeof(id),1,file)!=1){
      printf("shit id\n");
      return TRUE;
  }
  pft->litter=id;

  return FALSE;
} /* of 'freadpft' */
