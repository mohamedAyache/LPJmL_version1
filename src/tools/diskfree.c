/***************************************************************************/
/**                                                                       **/
/**                   d  i  s  k  f  r  e  e  .  c                        **/
/**                                                                       **/
/**     Function gets free space on file system in bytes                  **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 14.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#ifdef _WIN32
#include "windows.h"
#else
#include <sys/statvfs.h>
#endif
#include "types.h"

long long diskfree(const char *disk)
{
#ifdef _WIN32
  unsigned long long freebytes;
  if(GetDiskFreeSpaceEx(disk,(PULARGE_INTEGER)&freebytes,NULL,NULL))
    return (long long)freebytes;
  else
    return -1;
#else
  struct statvfs fiData;
  if((statvfs(disk,&fiData))<0)
    return -1;
  else
    return fiData.f_bsize*fiData.f_bfree;
#endif
} /* of 'diskfree' */
