/***************************************************************************/
/**                                                                       **/
/**    c h e c k _ s t a n d _ f r a c s _ f o r _ r e s e r v o i r . c  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by H. Biemans                                             **/
/**     Wageningen University and Research Centre                         **/
/**     to update the stand fracs when the reservoir is built.            **/
/**                                                                       **/
/**     Last change: 13.10.2009 by Werner von Bloh                        **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define accuracy 1e-4

Bool check_stand_fracs_for_reservoir(const Cell *cell,
                                     Real *difffrac)
{
  int s;
  Real frac_sum;
  const Stand *stand;

  frac_sum=cell->lakefrac+cell->ml.reservoirfrac; 
  foreachstand(stand,s,cell->standlist)
  {
    if(stand->frac>0) 
      frac_sum+=stand->frac;
    else 
      fail(NEGATIVE_STAND_FRAC_ERR,TRUE,"negative STAND_FRAC :%g %g %g",
           stand->frac,cell->lakefrac,cell->ml.reservoirfrac);
  }
  *difffrac=frac_sum-1;

  return (fabs(frac_sum-1)>accuracy);
      
} /* of 'check_stand_fracs_for_reservoir' */
