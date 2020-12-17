/***************************************************************************/
/**                                                                       **/
/**     f  r  e  a  d  l  o  n  g  _  s  o  c  k  e  t  .  c              **/
/**                                                                       **/
/**     FORTARN interface to readlong_socket()                            **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 09.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "channel.h"

void readlong_socket_(Socket *socket,long long *data,int *n,int *err)
{
  *err=readlong_socket(socket,data,*n);
} /* of 'readlong_socket' */
