/***************************************************************************/
/**                                                                       **/
/**        r   e  a  d  d  o  u  b  l  e  _  s  o  c  k  e  t  .  c       **/
/**                                                                       **/
/**     Function reads double array from socket. Byte order is changed    **/
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

Bool readdouble_socket(Socket *socket,double *data,int n)
{
  Num *vec;
  int i;
  vec=(Num *)data;
  if(read_socket(socket,(char *)vec,sizeof(double)*n))
    return TRUE;
  if(socket->swap)
    for(i=0;i<n;i++)
      data[i]=swapdouble(vec[i]);
  return FALSE;
} /* of 'readdouble_socket' */
