/***************************************************************************/
/**                                                                       **/
/**           f  w  r  i  t  e  i  n  t  _  s  o  c  k  e  t  .  c        **/
/**                                                                       **/
/**     FORTRAN interface to writeint_socket()                            **/
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

void writeint_socket_(Socket *socket,int *buffer,int *n,int *err)
{
  *err=writeint_socket(socket,buffer,*n);
} /* of 'writeint_socket' */
