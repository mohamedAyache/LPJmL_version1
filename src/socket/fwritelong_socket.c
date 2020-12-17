/***************************************************************************/
/**                                                                       **/
/**           f  w  r  i  t  e  l  o  n  g  _  s  o  c  k  e  t  .  c     **/
/**                                                                       **/
/**     FORTRAN interface to writelong_socket()                           **/
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

void writelong_socket_(Socket *socket,long long *buffer,int *n,int *err)
{
  *err=writelong_socket(socket,buffer,*n);
} /* of 'writelong_socket' */
