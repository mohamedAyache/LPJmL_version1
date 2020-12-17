/***************************************************************************/
/**                                                                       **/
/**                 f  p  r  i  n  t  s  t  a  n  d  .  c                 **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function prints stand variables in text file                      **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-15 12:26:32 +0200 (mer., 15 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void fprintstand(FILE *file,        /* Pointer to text file */
                 const Stand *stand /* Stand pointer */
                )
{
  int l;
  fprintf(file,"Standfrac:\t%g\n",stand->frac);
  fprintf(file,"Standtype:\t%s\n",stand->type->name);
  fprintf(file,"Standage: \t%d (yr)\n",stand->age);
  /* print stand-specific data */
  stand->type->fprint(file,stand);
  fprintsoil(file,&stand->soil);
  fputs("Frac_g:\t\t",file);
  for(l=0;l<NSOILLAYER;l++)
    fprintf(file,"%.2f ",stand->frac_g[l]);
  fputc('\n',file);
  fprintpftlist(file,&stand->pftlist);
} /* of 'fprintstand' */
