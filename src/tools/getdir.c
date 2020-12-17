/***************************************************************************/
/**                                                                       **/
/**                  g  e  t  d  i  r  .  c                               **/
/**                                                                       **/
/**     Function gets current working directory                           **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 07.09.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#define PATH_MAX STRING_LEN /* PATH_MAX is not defined on Windows */
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "types.h"

char *getdir(void)
{
  static char name[PATH_MAX];
  if(getcwd(name,PATH_MAX)==NULL)
    strcpy(name,"n/a");
  return name;
} /* of 'getdir' */
