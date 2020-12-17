/***************************************************************************/
/**                                                                       **/
/**               f  i  n  d  l  a  n  d  u  s  e  t  y  p  e  .  c       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 09.02.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

int findlandusetype(const Standlist standlist, /* stand list */
                    Landusetype landusetype    /* land-use type */
                   ) /* returns index of stand found or NOT_FOUND */
{
  int s;
  const Stand *stand;
  foreachstand(stand,s,standlist)
    if(stand->type->landusetype==landusetype)
      return s;
  return NOT_FOUND;
} /* of 'findlandusetype' */
