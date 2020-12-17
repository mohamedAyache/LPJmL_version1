/***************************************************************************/
/**                                                                       **/
/**            a  n  n  u  a  l  _  t  r  e  e  .  c                      **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 04.08.2010                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

Bool annual_tree(Stand *stand, /* pointer to stand */
                 Pft *pft,     /* pointer to PFT */
                 Real *fpc_inc /* FPC increment */
                )              /* returns TRUE on death */
{
  Real turnover_ind;
  Bool isdead;
  
 
  turnover_ind=turnover_tree(&stand->soil.litter,pft);
  isdead=allocation_tree(&stand->soil.litter,pft,fpc_inc);
  if(!isdead)
  {
    isdead=mortality_tree(&stand->soil.litter,pft,turnover_ind,
                          stand->cell->climbuf.temp_max);
    if(!isdead){ /* still not dead? */
      isdead=!survive(pft->par,&stand->cell->climbuf);
    }
  }
  return isdead;
} /* of 'annual_tree' */
