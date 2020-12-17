/***************************************************************************/
/**                                                                       **/
/**                     e  q  u  i  l  s  o  m  . c                       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 20.10.2004                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

/*
 *  DESCRIPTION
 *
 *  after vegetation equilibrium reset decomposition parameter
 *
 *
 */

void equilveg(Cell *cell)
{
  int s,l;
  Stand *stand;
  
  foreachstand(stand,s,cell->standlist)
  {
    forrootsoillayer(l)
    {
       stand->soil.k_mean[l].fast=0.0;
       stand->soil.k_mean[l].slow=0.0;
/*     for(p=0;p<stand->soil.litter.n;p++)
       {
         for(f=0;f<NFUELCLASS;f++)
            stand->soil.litter.ag[p].trait.wood[f]=0.0;
         stand->soil.litter.bg[p]=stand->soil.litter.ag[p].trait.leaf=0;
      }*/
    }
    stand->soil.decomp_litter_mean=stand->soil.count=0;
  }
} /* of 'equilveg' */
