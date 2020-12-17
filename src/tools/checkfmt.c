/***************************************************************************/
/**                                                                       **/
/**                  c  h  e  c  k  f  m  t  .  c                         **/
/**                                                                       **/
/**     Functions checks whether format string contains one %arg format   **/
/**     specifier                                                         **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 18.06.2012                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "types.h"

Bool checkfmt(const char *fmt, /* format string */
              char arg         /* conversion character */
             )                 /* returns TRUE on error */
{
  Bool found;
  if(fmt==NULL)
    return TRUE;
  found=FALSE;
  while(*fmt!='\0')
   if(*fmt=='%') 
   {
     fmt++;
     if(*fmt==arg)
     {
       if(found)
         return TRUE;
       found=TRUE;
       fmt++;
     }
     else if(*fmt=='%')
       fmt++;
     else
       return TRUE;
    }
    else
     fmt++;
  return !found;
} /* of 'checkfmt' */
