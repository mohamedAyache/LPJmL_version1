/***************************************************************************/
/**                                                                       **/
/**     f  r  e  a  d  f  l  o  a  t  _  s  o  c  k  e  t  .  c           **/
/**                                                                       **/
/**     FORTRAN interface to readfloat_socket()                           **/
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
#include "swap.h"
#include "channel.h"

void readfloat_socket_(Socket *socket,float *data,int *n,int *err)
{
  *err=readfloat_socket(socket,data,*n);
} /* of 'readfloat_socket' */
