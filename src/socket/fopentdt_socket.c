/***************************************************************************/
/**                                                                       **/
/**               f  o  p  e  n  t  d  t  _  s  o  c  k  e  t  .  c       **/
/**                                                                       **/
/**     FORTRAN interface to opentdt_socket()                             **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 10.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "channel.h"

void opentdt_socket_(Socket *socket,
                     int *port, /* port of TCP/IP connection */
                     int *wait,
                     int *err   /* error code */
                    )
{
  Socket *s;
  s=opentdt_socket(*port,*wait);
  if(s==NULL)
    *err=1;
  else
  {
    *socket=*s;
    free(s);
    *err=0;
  }
} /* of 'opentdt_socket_' */
