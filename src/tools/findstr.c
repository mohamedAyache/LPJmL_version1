/***************************************************************************/
/**                                                                       **/
/**                      f  i  n  d  s  t  r  .  c                        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function finds string in array of strings                         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 11.04.2012                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include <string.h>
#include "types.h"

int findstr(const char *s,const char *const *array,int size)
{
  int i;
  if(s!=NULL && array!=NULL)
  {
    for(i=0;i<size;i++)
      if(!strcmp(s,array[i]))
        return i;
  }
  return NOT_FOUND;
} /* of 'findstr' */
