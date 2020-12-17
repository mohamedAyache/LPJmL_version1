/***************************************************************************/
/**                                                                       **/
/**           c  o  n  n  e  c  t   _  s  o  c  k  e  t  .  c             **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2013-10-15 10:24:05 +0200 (mar., 15 oct. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#ifdef _WIN32
#include "windows.h"
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/socketvar.h>
#include <arpa/inet.h>
#endif
#include <string.h>
#include <errno.h>
#include "types.h"
#include "channel.h"

Socket *connect_socket(const char *hostname, /* name of host to connect */
                       int port,             /* port number */
                       int wait              /* wait for connection (sec) */
                      )                      /* returns pointer to socket or NULL */
{
  Socket *sock;
  struct sockaddr_in name;
  struct hostent *entry;
  int iter;
  short token=1;
#ifdef _WIN32
  WORD version;
  SOCKET my_socket;
  WSADATA data;
  version=MAKEWORD(1,1);
  if(WSAStartup(version,&data))
  {
    fprintf(stderr,"ERROR301: Cannot start socket, rc=%d\n",WSAGetLastError());
    return NULL;
  }
#else
  int my_socket;
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
  entry=gethostbyname(hostname);
  if(entry==NULL)
  {
#ifdef _WIN32
    WSACleanup();
#endif
    fprintf(stderr,"ERROR303: Cannot get hostname: %s\n",strerror(errno));
    return NULL;
  }
  name.sin_family=AF_INET;
  memcpy(&name.sin_addr.s_addr,entry->h_addr,entry->h_length);
  name.sin_port=htons((unsigned short)port);
  iter=0;
  while(connect(my_socket,(struct sockaddr *)&name,sizeof(name))<0)
  {
    if(iter>=wait*20)
    {
#ifdef _WIN32
      closesocket(my_socket);
      WSACleanup();
#else
      close(my_socket);
#endif
      fputs("ERROR308: Timeout in connecting to socket.\n",stderr);
      return NULL;
    }
#ifdef _WIN32
    Sleep(50);
#else
    usleep(50000);
#endif
    iter++;
  }
  sock=(Socket *)malloc(sizeof(Socket));
  if(sock==NULL)
  {
    fputs("ERROR304: Cannot allocate memory for socket.\n",stderr);
    return NULL;
  }
  sock->channel=my_socket;
  write_socket(sock,(char *)&token,sizeof(token));
  read_socket(sock,(char *)&token,sizeof(token));
  sock->swap=(token!=1);
  return sock;
} /* of 'connect_socket' */
