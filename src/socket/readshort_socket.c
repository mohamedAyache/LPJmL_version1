/***************************************************************************/
/**                                                                       **/
/**        r  e  a  d  s  h  o  r  t  _  s  o  c  k  e  t  .  c           **/
/**                                                                       **/
/**     Function reads short array from socket. Byte order is changed     **/
/**     if endianness is different from sender                            **/
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

Bool readshort_socket(Socket *socket,short *data,int n)
{
  int i;
  if(read_socket(socket,(char *)data,sizeof(short)*n))
    return TRUE;
  if(socket->swap)
    for(i=0;i<n;i++)
      data[i]=swapshort(data[i]); /* change byte order */
  return FALSE;
} /* of 'readshort_socket' */
