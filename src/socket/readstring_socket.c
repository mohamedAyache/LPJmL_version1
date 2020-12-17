/***************************************************************************/
/**                                                                       **/
/**        r  e  a  d  s  t  r  i  n  g  _  s  o  c  k  e  t  .  c        **/
/**                                                                       **/
/**     Function reads string from socket                                 **/
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
#include "types.h"
#include "channel.h"

char *readstring_socket(Socket *socket)
{
  unsigned int n;
  char *s;
  if(readint_socket(socket,&n,1))
    return NULL;
  s=malloc(n+1);
  if(s==NULL)
    return s;
  if(n)
    read_socket(socket,s,n);
  s[n]='\0';
  return s;
} /* of 'readstring_socket' */
