/***************************************************************************/
/**                                                                       **/
/**        f  c  o  n  n  e  c  t  _  s  o  c  k  e  t  .  c              **/
/**                                                                       **/
/**     FORTRAN interface to connect_socket                               **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 09.03.2010                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "channel.h"

void connect_socket_(Socket *socket,
                     const char *hostname, /* hostname to connect */
                     int *port,            /* port number */
                     int *wait,            /* wait for connection (sec) */
                     int *err,             /* error code */
                     int len               /* length of string hostname
                                              added by FORTRAN */
                    )
{
  Socket *sock;
  char *h;
  h=malloc(len+1);
  if(h==NULL)
  {
    *err=1;
    return;
  }
  h=memcpy(h,hostname,len);
  h[len]='\0';
  sock=connect_socket(h,*port,*wait);
  free(h);
  if(sock==NULL)
    *err=1;
  else
  {
    *socket=*sock;
    free(sock);
    *err=0;
  }
} /* of 'connect_socket_' */
