/***************************************************************************/
/**                                                                       **/
/**               f  r  e  e  m  a  n  a  g  e  .  c                      **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Landuse Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-24 11:22:32 +0200 (ven., 24 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void freemanage(Manage *manage,int npft)
{
  if(manage->par!=NULL)
  {
    if(manage->par->laimax_cft==NULL)
      free(manage->laimax+npft);
  }
} /* of 'freemanage' */
