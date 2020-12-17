/***************************************************************************/
/**                                                                       **/
/**                   f  s  c  a  n  f  l  o  a  t  .  c                  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads a float value from a text file                     **/
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
#include "types.h"

Bool fscanfloat(FILE *file,float *value,const char *name,Bool isout)
{
  String line,token;
  char *endptr;
  Bool rc;
  rc=fscantoken(file,token);
  if(!rc)
  {
    *value=(float)strtod(token,&endptr);
    rc=*endptr!='\0';
  }
  if(rc && isout)
  {
    fprintf(stderr,"ERROR101: Cannot read float '%s', line read:\n",name);
    if(fgets(line,STRING_LEN,file)!=NULL)
      line[strlen(line)-1]='\0';
    else
      line[0]='\0';
    fprintf(stderr,"          '%s%s'\n           ",token,line);
    frepeatch(stderr,'^',strlen(token));
    fputc('\n',stderr);
  }
  return rc;
} /* of 'fscanfloat' */
