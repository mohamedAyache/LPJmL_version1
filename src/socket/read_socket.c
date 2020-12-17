/***************************************************************************/
/**                                                                       **/
/**                 r  e  a  d  _  s  o  c  k  e  t  .  c                 **/
/**                                                                       **/
/**     Function reads bytes from socket                                  **/
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

Bool read_socket(Socket *socket,void *data,int n)
{
  int i,j;
  i=n;
  do
  {
    j=recv(socket->channel,(char *)data+n-i,i,0);
    if(j<0)
      return TRUE;
    i-=j;
  }while(i);
  return FALSE;
} /* of 'read_socket' */
