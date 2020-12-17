/***************************************************************************/
/**                                                                       **/
/**             g  e  t  f  i  l  e  d  a  t  e  .  c                     **/
/**                                                                       **/
/**     Function gets modification time of file                           **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 01.09.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include "types.h"

time_t getfiledate(const char *name /* filename */
                  )                 /* returns modification time or -1
                                       on error */
{
  struct stat filestat;
  return (stat(name,&filestat)) ? -1 : filestat.st_mtime;
} /* of 'getfiledate' */
