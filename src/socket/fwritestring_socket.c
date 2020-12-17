/***************************************************************************/
/**                                                                       **/
/**    w  r  i  t  e  s  t  r  i  n  g  _  s  o  c  k  e  t  .  c         **/
/**                                                                       **/
/**     Function writes string to socket                                  **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 12.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "channel.h"

void writestring_socket_(Socket *socket,const char *s,int *err,int len)
{
  if(writeint_socket(socket,&len,1))
    *err=1;
  else if(len)
    *err=write_socket(socket,s,len);
  else
    *err=0;
} /* of 'writestring_socket' */
