/***************************************************************************/
/**                                                                       **/
/**             g  e  t  f  i  l  e  s  i  z  e  .  c                     **/
/**                                                                       **/
/**     Function gets file size in bytes                                  **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 12.12.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "types.h"

long long getfilesize(const char *name /* filename */
                )                      /* returns file size in bytes or -1
                                          on error */
{
  struct stat filestat;
  return (stat(name,&filestat)) ? -1 : filestat.st_size;
} /* of 'getfilesize' */
