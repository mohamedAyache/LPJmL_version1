/***************************************************************************/
/**                                                                       **/
/**                   s  t  r  i  p  s  u  f  f  i  x  .  c               **/
/**                                                                       **/
/**     Function strips suffix from filename                              **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 24.03.2011                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"

char *stripsuffix(const char *name /* filename with suffix */
                 )                 /* returns pointer to filename */
{
  int i;
  char *new;
#ifdef SAFE
  if(name==NULL)
    return NULL;
#endif
  for(i=strlen(name)-1;i>=0;i--)
    if(name[i]=='.')
      break;
  if(i<0) /* no suffix found */
    i=strlen(name);
  new=malloc(i+1);
  if(new==NULL)
    return NULL;
  strncpy(new,name,i);
  new[i]='\0';
  return new;
} /* of 'stripsuffix' */
