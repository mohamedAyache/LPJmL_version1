/***************************************************************************/
/**                                                                       **/
/**                   h  a  s  s  u  f  f  i  x  .  c                     **/
/**                                                                       **/
/**     Function checks whether filename has specified suffix             **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 24.03.2011                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"

Bool hassuffix(const char *name,  /* filename with suffix */
               const char *suffix /* file suffix including '.' */
              ) 
{
#ifdef SAFE
  if(name==NULL || suffix==NULL)
    return FALSE;
#endif
  return (strlen(name)>=strlen(suffix) &&
          !strcmp(name+strlen(name)-strlen(suffix),suffix));
} /* of 'hassuffix' */
