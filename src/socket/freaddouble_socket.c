/***************************************************************************/
/**                                                                       **/
/**     f  r   e  a  d  d  o  u  b  l  e  _  s  o  c  k  e  t  .  c       **/
/**                                                                       **/
/**     FORTRAN interface to freaddouble_socket()                         **/
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

void readdouble_socket_(Socket *socket,double *data,int *n,int *err)
{
  *err=readdouble_socket(socket,data,*n);
} /* of 'readdouble_socket' */
