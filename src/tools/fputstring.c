/***************************************************************************/
/**                                                                       **/
/**                   f  p  u  t  s  t  r  i  n  g  .  c                  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function prints string and breaks line into pieces if output      **/
/**     exceeds maximum line length.                                      **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 23.10.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"

int fputstring(FILE *file,       /* pointer to ASCII file */
               int len,          /* current length of line */
               const char *line, /* string added to line */
               int max_len       /* maximum length of line */
              )                  /* return updated length of line */
{
  int l;
  if(line==NULL)
    return len;
  l=strlen(line);
  if(len+l>max_len) /* line too long? */
  {
    /* yes, print newline */
    putc('\n',file);
    len=l;
  }
  else
    len+=l; /* no, update length of line */
  fputs(line,file);
  return len;
} /* of 'fputstring' */
