/***************************************************************************/
/**                                                                       **/
/**        r  e  a  d  f  l  o  a  t  _  s  o  c  k  e  t  .  c           **/
/**                                                                       **/
/**     Function reads float array from socket. Byte order is changed     **/
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

Bool readfloat_socket(Socket *socket,float *data,int n)
{
  int *vec;
  int i;
  vec=(int *)data;
  if(read_socket(socket,(char *)vec,sizeof(float)*n))
    return TRUE;
  if(socket->swap)
    for(i=0;i<n;i++)
      vec[i]=swapint(vec[i]);
  return FALSE;
} /* of 'readfloat_socket' */
