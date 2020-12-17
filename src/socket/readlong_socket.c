/***************************************************************************/
/**                                                                       **/
/**        r  e  a  d  l  o  n  g  _  s  o  c  k  e  t  .  c              **/
/**                                                                       **/
/**     Function reads long array from socket. Byte order is changed      **/
/**     if endianness is different from sender.                           **/
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
#include "swap.h"
#include "channel.h"

Bool readlong_socket(Socket *socket,long long *data,int n)
{
  int i;
  if(read_socket(socket,(char *)data,sizeof(long long)*n))
    return TRUE;
  if(socket->swap)
    for(i=0;i<n;i++)
      data[i]=swaplong(data[i]);
  return FALSE;
} /* of 'readlong_socket' */
