/***************************************************************************/
/**                                                                       **/
/**                  f  s  c  a  n  s  t  r  i  n  g  .  c                **/
/**                                                                       **/
/**     Functions reads string of maximum length STRING_LEN. String is    **/
/**     read up to the first occurence of a whitespace character or is    **/
/**     delimited by  '"'.                                                **/
/**     Returns FALSE on success                                          **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 27.02.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include "types.h"

Bool fscanstring(FILE *file, /* file pointer of a text file         */
                 String s    /* pointer to a char array of dimension
                                STRING_LEN+1                        */
                )            /* return TRUE on error                */
{
  int c;
  int len;
  /* searching for first occurence of non-whitespace character  */
  while((c=fgetc(file))!=EOF)
    if(!isspace(c))
    {
      /* non-whitespace character found */
      if(c=='\"') /* opening '"' found? */
      {
        len=0;
        while((c=fgetc(file))!=EOF)
        {
          if(c=='\"') /* closing '"' found? */
          {
            s[len]='\0';  /* yes, return with success */
            return FALSE;
          }
          else if(len==STRING_LEN)  /* string too long? */
            break;
          else if(c=='\\') /* backslash found? */
          {
            if((c=fgetc(file))==EOF) /* yes, read next character */
            {
              s[len]='\0';
              return TRUE;
            }
            else
              switch(c)
              {
                case '"': case '\\':
                  s[len++]=(char)c;
                  break;
                case 'n':
                  s[len++]='\n';
                  break;
                case 't':
                  s[len++]='\t';
                  break;
                default:
                  s[len]='\0';
                  return TRUE;
              }
          }
          else
            s[len++]=(char)c;
        }
      }
      else
      {
        s[0]=(char)c;
        len=1;
        while((c=fgetc(file))!=EOF)
        {
          if(isspace(c))
          {
            s[len]='\0';  /* yes, return with success */
            return FALSE;
          }
          else if(len==STRING_LEN)  /* string too long? */
          {
            s[len]='\0';  /* terminate string */
            return TRUE;
          }
          else
            s[len++]=(char)c;
        }
        s[len]='\0';
        return FALSE;
      }
      s[len]='\0';  /* terminate string */
      return TRUE;
    }
  *s='\0';  /* no, exits with error */
  return TRUE;
} /* of 'fscanstring' */
