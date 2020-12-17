/***************************************************************************/
/**                                                                       **/
/**                   f  s  c  a  n  r  e  a  l  .  c                     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads a real value from a text file                      **/
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
#include <math.h>
#include "types.h"

Bool fscanreal(FILE *file,       /* file pointer of a text file       */
               Real *val,        /* real value read from file         */
               const char *name, /* name of variable                  */
               Bool isout        /* enable error message (TRUE/FALSE) */
              )                  /* return TRUE on error              */
{
  double x;
  Bool rc;
  String line,token;
  char *ptr;
  rc=fscantoken(file,token);
  if(!rc)
  {
     x=strtod(token,&ptr);
     rc=*ptr!='\0';
  }
  if(rc || isnan(x))
  {
    if(isout)
    {
      fprintf(stderr,"ERROR101: Cannot read real '%s', line read:\n",name);
      if(fgets(line,STRING_LEN,file)!=NULL)
        line[strlen(line)-1]='\0';
      else
        line[0]='\0';
      fprintf(stderr,"          '%s%s'\n           ", token,line);
      frepeatch(stderr,'^',strlen(token));
      fputc('\n',stderr);
    }
    return TRUE; /* error occured at read */
  }
  *val=(Real)x;
  return FALSE;  /* no error */
} /* of 'fscanreal' */
