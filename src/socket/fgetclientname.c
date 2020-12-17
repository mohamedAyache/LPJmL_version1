/***************************************************************************/
/**                                                                       **/
/**            f  g  e  t  c  l  i  e  n  t  n  a  m  e  .  c             **/
/**                                                                       **/
/**     Function gets client hostname from socket                         **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 12.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "channel.h"

void getclientname_(const Socket *socket,
                    char *name,
                    int *err,
                    int len
                   )
{
  char *s;
  s=getclientname(socket);
  if(s==NULL)
    *err=1;
  else
  {
    strncpy(name,s,len);
    *err=0;
  }
} /* of 'getclientname' */
