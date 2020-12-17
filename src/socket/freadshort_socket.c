/***************************************************************************/
/**                                                                       **/
/**     f  r  e  a  d  s  h  o  r  t  _  s  o  c  k  e  t  .  c           **/
/**                                                                       **/
/**     FORTRAN interface to read_short_socket()                          **/
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

void readshort_socket_(Socket *socket,short *data,int *n,int *err)
{
  *err=readshort_socket(socket,data,*n);
} /* of 'readshort_socket' */
