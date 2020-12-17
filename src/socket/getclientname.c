/***************************************************************************/
/**                                                                       **/
/**               g  e  t  c  l  i  e  n  t  n  a  m  e  .  c             **/
/**                                                                       **/
/**     Function gets client hostname from socket                         **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 01.12.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#ifndef _WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netdb.h>
#endif
#include "types.h"
#include "channel.h"

char *getclientname(const Socket *socket
                   )         /* returns client host name or NULL */
{
  struct hostent *host;
  struct in_addr addr;
  struct sockaddr_in sin;
#ifdef _WIN32
  int len;
#else
  socklen_t len;
#endif
  if (getpeername(socket->channel,(struct sockaddr *)&sin,&len)<0)
    return NULL;
  addr=sin.sin_addr;
  if ((host=gethostbyaddr((char *)&addr,
                           sizeof(addr),
                           AF_INET)) == NULL)
    return inet_ntoa(sin.sin_addr);
  return host->h_name;
} /* of 'getclientname' */
