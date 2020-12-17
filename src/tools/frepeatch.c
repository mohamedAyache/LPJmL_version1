/***************************************************************************/
/**                                                                       **/
/**                  f   r  e  p  e  a  t  c  h  .  c                     **/
/**                                                                       **/
/**     Function prints character repeatedly                              **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 15.09.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include "types.h"

void frepeatch(FILE *file, /* pointer to ASCII file */
               char c,     /* character to repeat */
               int count   /* repeat count */
              )
{
  int i;
  for(i=0;i<count;i++)
    putc(c,file);
} /* of 'frepeatch' */
