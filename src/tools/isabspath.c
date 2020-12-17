/***************************************************************************/
/**                                                                       **/
/**                  i  s  a  b  s  p  a  t  h  .  c                      **/
/**                                                                       **/
/**     Function determines whether filename begins with absolute path,   **/
/**     i.e. on UNIX begins with '/'                                      **/
/**          on Windows begins with '/','\' or drive letter 'a:'          **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 19.10.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "types.h"

Bool isabspath(const char *name)
{
#ifdef _WIN32
  if(name[0]=='/' || name[0]=='\\')
    return TRUE;
  if(strlen(name)>1 && isalpha(name[0]) && name[1]==':')
    return TRUE;
  return FALSE;
#else
  return (name[0]=='/');
#endif
} /* of 'isabspath' */
