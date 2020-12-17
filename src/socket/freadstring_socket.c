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
/**     Last change: 14.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "channel.h"

void readstring_socket_(Socket *socket,char *s,int *err,int len)
{
  int n;
  if(readint_socket(socket,&n,1))
    *err=1;
  else if(n>len)
    *err=1;
  else if(n)
    *err=read_socket(socket,s,n);
  else
    *err=0;
} /* of 'readstring_socket' */
