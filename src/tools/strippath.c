/***************************************************************************/
/**                                                                       **/
/**                   s  t  r  i  p  p  a  t  h  .  c                     **/
/**                                                                       **/
/**     Function strips path from filename                                **/
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

const char *strippath(const char *name /* filename with path */
                     )                 /* returns pointer to filename */
{
  int i;
#ifdef SAFE
  if(name==NULL)
    return NULL;
#endif
  for(i=strlen(name)-1;i>=0;i--)
#ifdef _WIN32
    if(name[i]=='/' || name[i]=='\\' || name[i]==':')
    /* check for slash, backslash and ':' of drive */
#else
    if(name[i]=='/') /* check for slash only */
#endif
      break;
  return name+i+1;
} /* of 'strippath' */
