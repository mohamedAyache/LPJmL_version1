/***************************************************************************/
/**                                                                       **/
/**              w  r  i  t  e  _  s  o  c  k  e  t  .  c                 **/
/**                                                                       **/
/**     Function writes bytes to socket                                   **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 13.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#ifndef _WIN32
#include <sys/types.h>
#include <sys/socket.h>
#endif
#include "types.h"
#include "channel.h"

Bool write_socket(Socket *socket,const void *buffer,int n)
{
  int i,j;
  i=0;
  do
  {
    j=send(socket->channel,(char *)buffer+i,n,0);
    if(j<0)
      return TRUE;
    i+=j;
    n-=j;
  }while(n);
  return FALSE;
} /* of 'write_socket' */
