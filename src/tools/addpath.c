/***************************************************************************/
/**                                                                       **/
/**                        a  d  d  p  a  t  h  .  c                      **/
/**                                                                       **/
/**     Function add path to filename. Only done for filenames without    **/
/**     an absolute path.                                                 **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 16.09.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"

char *addpath(const char *name, /* filename */
              const char *path  /* path or NULL */
             )                  /* returns allocated string or NULL */
{
  char *s;
  if(path!=NULL && !isabspath(name))
  {
    s=malloc(strlen(path)+strlen(name)+2);
    if(s==NULL)
      return NULL;
    return strcat(strcat(strcpy(s,path),"/"),name);
  }
  else
    return strdup(name);
} /* of 'addpath' */
