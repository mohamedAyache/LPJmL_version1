/***************************************************************************/
/**                                                                       **/
/**                 f  r  e  e  c  o  o  r  d  a  r  r  a  y  .  c        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function deallocates index array                                  **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 21.03.2011                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void freecoordarray(Coord_array *array)
{
  if(array!=NULL)
  {
    free(array->index);
    free(array);
  }
} /* of 'freecoordarray' */
