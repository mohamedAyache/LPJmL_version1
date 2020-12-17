/***************************************************************************/
/**                                                                       **/
/**           f  p  r  i  n  t  p  a  r  _  g  r  a  s  s  .  c           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function prints PFT parameter                                     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 30.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"

void fprintpar_grass(FILE *file,const Pftpar *par)
{
  const Pftgrasspar *pargrass;
  pargrass=par->data;
  fprintf(file,"turnover:\t%g %g (yr)\n"
               "C:N ratio:\t%g %g\n"
               "reprod cost:\t%g\n",
          pargrass->turnover.leaf,pargrass->turnover.root,
          par->respcoeff*param.k/pargrass->cn_ratio.leaf,
          par->respcoeff*param.k/pargrass->cn_ratio.root,
          pargrass->reprod_cost);
} /* of 'fprintpar_grass' */
