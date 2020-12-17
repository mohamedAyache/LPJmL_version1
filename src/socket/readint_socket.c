/***************************************************************************/
/**                                                                       **/
/**        r  e  a  d  i  n  t  _  s  o  c  k  e  t  .  c                 **/
/**                                                                       **/
/**     Function reads int array from socket. Byte order is changed       **/
/**     if endianness is different from sender.                           **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 20.11.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "swap.h"
#include "channel.h"

Bool readint_socket(Socket *socket,int *data,int n)
{
  int i;
  if(read_socket(socket,(char *)data,sizeof(int)*n))
    return TRUE;
  if(socket->swap)
    for(i=0;i<n;i++)
      data[i]=swapint(data[i]);
  return FALSE;
} /* of 'readint_socket' */
