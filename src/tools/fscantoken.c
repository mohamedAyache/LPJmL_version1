/***************************************************************************/
/**                                                                       **/
/**                   f  s  c  a  n  t  o  k  e  n  .  c                  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads a token from a text file                           **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 27.02.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "types.h"

Bool fscantoken(FILE *file, /* file pointer of a text file         */
                String s    /* pointer to a char array of dimension
                               STRING_LEN+1                        */
               )            /* return TRUE on error                */
{
  int c;
  int len;
  /* searching for first occurence of non-whitespace character  */
  while((c=fgetc(file))!=EOF)
    if(!isspace(c))
      break;
  if(c==EOF) /* reached end of file? */
  {
    s[0]='\0';
    return TRUE;
  }
  s[0]=(char)c;
  len=1;
  while((c=fgetc(file))!=EOF)
  {
    if(isspace(c)) /* white space detected? */
    {
      ungetc(c,file);
      break;
    }
    else if(len==STRING_LEN)  /* string too long? */
    {
      s[len]='\0';  /* terminate string */
      return TRUE;
    }
    else
      s[len++]=(char)c; /* add character to string */
  }
  s[len]='\0';  /* terminate string */
  return FALSE;
} /* of 'fscantoken' */
