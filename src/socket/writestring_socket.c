/***************************************************************************/
/**                                                                       **/
/**    w  r  i  t  e  s  t  r  i  n  g  _  s  o  c  k  e  t  .  c         **/
/**                                                                       **/
/**     Function writes string to socket                                  **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 12.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "channel.h"

Bool writestring_socket(Socket *socket,const char *s)
{
  unsigned int n;
  n=strlen(s);
  if(writeint_socket(socket,&n,1))
    return TRUE;
  if(n)
    return write_socket(socket,s,n);
  return FALSE;
} /* of 'writestring_socket' */
