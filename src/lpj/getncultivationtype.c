/***************************************************************************/
/**                                                                       **/
/**     g  e  t  n  c  u  l  t  i  v  a  t  i  o  n  t  y  p  e  .  c     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function counts number of PFTs with given cultivation type        **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-23 11:11:13 +0200 (jeu., 23 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

int getncultivationtype(const Pftpar pftpar[], /* PFT parameter array */
                        int npft,              /* number of natural PFTs */
                        int cultivation_type
                       )                       /* returns number of biomass PFTs */
{
  int p,ntype;
  ntype=0;
  for(p=0;p<npft;p++)
    if(pftpar[p].cultivation_type==cultivation_type)
      ntype++;
  return ntype;
} /* of 'getncultivationtype' */
