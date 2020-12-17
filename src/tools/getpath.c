/***************************************************************************/
/**                                                                       **/
/**                   g  e  t  p  a  t  h  .  c                           **/
/**                                                                       **/
/**     Function extracts path from filename                              **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 02.12.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"

char *getpath(const char *name /* filename */
             )                 /* returns pointer to allocated path */
{
  int i;
  char *path;
  for(i=strlen(name)-1;i>=0;i--)
#ifdef _WIN32
    if(name[i]=='/' || name[i]=='\\' || name[i]==':')
    /* check for slash, backslash and ':' of drive */
#else
    if(name[i]=='/') /* check for slash only */
#endif
      break;
  if(i>=0)
  {
    path=malloc(i+2);
    if(path==NULL)
      return NULL; /* could not allocate memory, return NULL */
    strncpy(path,name,i+1);
    path[i+1]='\0';
  }
  else
    path=strdup(".");
  return path;
} /* of 'getpath' */
