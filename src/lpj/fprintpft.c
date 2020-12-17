/***************************************************************************/
/**                                                                       **/
/**                 f  p  r  i  n  t  p  f  t  .  c                       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function prints PFT variables                                     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-09-29 16:02:37 +0200 (lun., 29 sept. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void fprintpft(FILE *file,    /* pointer to text file */
               const Pft *pft /* pointer to PFT variables */
              )
{
  fprintf(file,"PFT:\t\t%s\n",pft->par->name);
  fprintf(file,"fpc:\t\t%g\n",pft->fpc);
  fprintf(file,"nind:\t\t%g (1/m2)\n",pft->nind);
  fprintf(file,"wscal:\t\t%g\n",pft->wscal_mean);
  fprintf(file,"aphen:\t\t%g\n",pft->aphen);
  fprintf(file,"bminc:\t\t%g (gC/m2)\n",pft->bm_inc);
  fprintf(file,"gdd:\t\t%g\n",pft->gdd); 
  fprintf(file,"FLAImax:\t%g\n",pft->flaimax);
  /* Print type specific variables */
  pft->par->fprint(file,pft);
} /* of 'fprintpft' */
