/***************************************************************************/
/**                                                                       **/
/**                 g  e  t  s  t  a  t  e  .  c                          **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 22.09.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Soilstate getstate(Real *temp)
{
  if (*temp-T_zero>epsilon)
    return ABOVE_T_ZERO;
  else if (*temp-T_zero<-epsilon)
    return BELOW_T_ZERO;
  else
  {
    *temp=T_zero;
    return AT_T_ZERO;
  }
} /* of 'getstate' */

/* CVS information:*/
#ifdef INCLUDE_RCSID             
static char const rcsid[] = 
    "@(#) $Id: get_state.c 120 2008-12-15 14:27:13Z heyder $"; /*survives gcc optimization*/ 
GENERATE_RCSID(rcsid2,
    "@(#) $Id: get_state.c 120 2008-12-15 14:27:13Z heyder $"); /*survives xlc optimization*/
#endif
