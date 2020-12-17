/***************************************************************************/
/**                                                                       **/
/**         o  p  e  n  t  d  t  _  s  o  c  k  e  t  .  c                **/
/**                                                                       **/
/**     Function opens socket and starts listening in a TDT compatible    **/
/**     way                                                               **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-15 12:26:32 +0200 (mer., 15 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#ifndef _WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#endif
#include "types.h"
#include "channel.h"

Socket *opentdt_socket(int port, /* port of TCP/IP connection */
                       int wait  /* maximum time for connection (sec)
                                    if zero unlimited */
                      )          /* returns open socket or NULL */
{
  Socket *sock;
  struct sockaddr_in name;
  struct sockaddr fsin;
  int known_int,array,opt=TRUE;
  char check='1';
  fd_set rfds;
  struct timeval tv;
  int rc;
#ifdef _WIN32
  int len;
  WORD version;
  WSADATA data;
  SOCKET my_socket;
  version=MAKEWORD(1,1);
  if(WSAStartup(version,&data))
  {
    fprintf(stderr,"ERROR301: Cannot start socket, rc=%d\n",WSAGetLastError());
    return NULL;
  }
#else
  int my_socket;
  socklen_t len;
#endif
  if(isinvalid_socket(my_socket=socket(AF_INET,SOCK_STREAM,0)))
  {
#ifdef _WIN32
    fprintf(stderr,"ERROR302: Cannot create socket, rc=%d\n",WSAGetLastError());
    WSACleanup();
#else
    fprintf(stderr,"ERROR302: Cannot create socket: %s\n",strerror(errno));
#endif
    return NULL;
  }
  name.sin_family=AF_INET ;
  name.sin_port=htons((unsigned short)port);
  name.sin_addr.s_addr=htonl(INADDR_ANY);

  setsockopt(my_socket,SOL_SOCKET,SO_REUSEADDR,(char *)&opt,sizeof(opt));
  if(bind(my_socket,(struct sockaddr *)&name,sizeof(name))<0)
  {
#ifdef _WIN32
    fprintf(stderr,"ERROR306: Cannot bind socket, rc=%d\n",WSAGetLastError());
    WSACleanup();
#else
    fprintf(stderr,"ERROR306: Cannot bind socket: %s\n",strerror(errno));
#endif
    return NULL;
  }
  listen(my_socket,5);
  if(wait)
  {
    FD_ZERO(&rfds);
    FD_SET(my_socket,&rfds);
    tv.tv_sec=wait;
    tv.tv_usec=0;
    rc=select(my_socket+1,&rfds,NULL,NULL,&tv);
    if(rc<0)
    {
#ifdef _WIN32
      fprintf(stderr,"ERROR307: Failure in select, rc=%d\n",WSAGetLastError());
      WSACleanup();
#else
      fprintf(stderr,"ERROR307: Failure in select: %s\n",strerror(errno));
#endif
      return NULL;
    }
    else if(rc==0)
    {
#ifdef _WIN32
      WSACleanup();
#endif
      fputs("ERROR308: Timeout in listening to socket.\n",stderr);
      return NULL;
    }
  }
  sock=(Socket *)malloc(sizeof(Socket));
  if(sock==NULL)
  {
    fputs("ERROR304: Cannot allocate memory for socket.\n",stderr);
    return NULL;
  }
  sock->channel=accept(my_socket,&fsin,&len);
  if(isinvalid_socket(sock->channel))
  {
#ifdef _WIN32
    fprintf(stderr,"ERROR309: Cannot accept socket, rc=%d\n",WSAGetLastError());
    WSACleanup();
#else
    fprintf(stderr,"ERROR309: Cannot accept socket: %s\n",strerror(errno));
#endif
    free(sock);
    return NULL;
  }
  write_socket(sock,&check,1);
  /* read int variable to determine endianess */
  read_socket(sock,&known_int,sizeof(int));
  sock->swap=(known_int!=42);
  readint_socket(sock,&array,1);
  array=1;
  writeint_socket(sock,&array,1);
  return sock;
} /* of 'opentdt_socket' */
