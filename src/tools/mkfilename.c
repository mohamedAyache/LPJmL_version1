/***************************************************************************/
/**                                                                       **/
/**               m  k  f  i  l  e  n  a  m  e  .  c                      **/
/**                                                                       **/
/**     Function changes string 'filename.[aaaa-bbbb].suffix' into        **/
/**     'filename.[%d].suffix'                                            **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 04.10.2011                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"

char *mkfilename(const char *filename)
{
  int i,j;
  char *new;
  if(filename==NULL)
    return NULL;
  /* find the first occurrence of opening '[' */
  for(i=0;filename[i]!='[';i++)
    if(filename[i]=='\0') /* end of string reached? */
      return NULL;
  /* find the first occurrence of closing ']' */
  for(j=i;filename[j]!=']';j++)
    if(filename[j]=='\0') /* end of string reached? */
      return NULL;
  new=malloc(i+2+strlen(filename)-j);
  if(new==NULL)
    return NULL;
  strncpy(new,filename,i);
  new[i]='\0';
  strcat(new,"%d");
  strcat(new,filename+j+1);
  return new;
} /* of 'mkfilename' */
