/***************************************************************************/
/**                                                                       **/
/**     f  r  e  a  d  i  n  t  _  s  o  c  k  e  t  .  c                 **/
/**                                                                       **/
/**     FORTARN interface to readint_socket()                             **/
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

void readint_socket_(Socket *socket,int *data,int *n,int *err)
{
  *err=readint_socket(socket,data,*n);
} /* of 'readint_socket' */
