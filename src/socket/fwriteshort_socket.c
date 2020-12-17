/***************************************************************************/
/**                                                                       **/
/**       f  w  r  i  t  e  s  h  o  r  t  _  s  o  c  k  e  t  .  c      **/
/**                                                                       **/
/**     FORTRAN interface to writeshort_socket()                          **/
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

void writeshort_socket_(Socket *socket,short *buffer,int *n,int *err)
{
  *err=writeshort_socket(socket,buffer,*n);
} /* of 'writeshort_socket' */
