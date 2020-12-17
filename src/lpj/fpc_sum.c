/***************************************************************************/
/**                                                                       **/
/**                       f  p  c  _  s  u  m  .  c                       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function calculates sum of foliar projective covers (FPC) of      **/
/**     established PFTs                                                  **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 24.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real fpc_sum(Real fpc_type[], /* on return, FPC sum of each PFT class */
             int ntype,       /* number of different PFT classes */
             const Pftlist *pftlist /* List of established PFTs */
            ) /* returns total FPC of established PFTs */
{
  const Pft *pft;
  int p,i;
  Real fpc;
  for(i=0;i<ntype;i++)
    fpc_type[i]=0; 
  fpc=0;
  foreachpft(pft,p,pftlist)
  {
    fpc+=pft->fpc;   
    fpc_type[pft->par->type]+=pft->fpc; 
  }
  return fpc;
} /* of 'fpc_sum' */
