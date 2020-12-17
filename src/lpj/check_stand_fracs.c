/***************************************************************************/
/**                                                                       **/
/**     c  h  e  c  k  _  s  t  a  n  d  _  f  r  a  c  s  .  c           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function checks stand fractions on consisteny. Sum of stand       **/
/**     must be <=1 and stand fractions must be non-negative              **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 01.09.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define accuracy 1e-4

void check_stand_fracs(const Standlist standlist, /* Stand list */
                       Real lakefrac              /* lake fraction (0..1) */
                      )
{
  int s;
  Real frac_sum;
  const Stand *stand;

  frac_sum=lakefrac; 
  foreachstand(stand,s,standlist)
  {
    if(stand->frac>0) 
      frac_sum+=stand->frac;
    else 
      fail(NEGATIVE_STAND_FRAC_ERR,TRUE,"negative STAND_FRAC :%g %g, cell (%g/%g)",
      stand->frac,lakefrac,stand->cell->coord.lon,stand->cell->coord.lat);
  }
  
  if(fabs(frac_sum-1)>accuracy)
    fail(STAND_FRAC_SUM_ERR,TRUE,"STAND_FRAC_SUM-error %g lakefrac: %g, cell (%g/%g)",
         fabs(frac_sum-1),lakefrac,stand->cell->coord.lon,stand->cell->coord.lat);
} /* of 'check_stand_fracs' */
