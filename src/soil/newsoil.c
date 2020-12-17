/***************************************************************************/
/**                                                                       **/
/**                n  e  w  _  s  o  i  l  .  c                           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 23.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void newsoil(Soil *soil)
{
    int l;
    soil->litter.n=0;
    soil->litter.ag=NULL;
    soil->litter.bg=NULL;
    forrootsoillayer(l)
        soil->cpool[l].fast=soil->cpool[l].slow=soil->YEDOMA=0.0;

} /* of 'newsoil' */
