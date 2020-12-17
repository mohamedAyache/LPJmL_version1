/***************************************************************************/
/**                                                                       **/
/**                   f  s  c  a  n  i  n  t  .  c                        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads an int value from a text file                      **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 26.02.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"

Bool fscanint(FILE *file,int *value,const char *name,Bool isout)
{
  String line,token;
  char *ptr;
  Bool rc;
  rc=fscantoken(file,token);
  if(!rc)
  {
    *value=(int)strtol(token,&ptr,10);
    rc=*ptr!='\0';
  }
  if(rc && isout)
  {
    fprintf(stderr,"ERROR101: Cannot read int '%s', line read:\n",name);
    if(fgets(line,STRING_LEN,file)!=NULL)
      line[strlen(line)-1]='\0';
    else
      line[0]='\0';
    fprintf(stderr,"          '%s%s'\n           ",token,line);
    frepeatch(stderr,'^',strlen(token));
    fputc('\n',stderr);
  }
  return rc;
} /* of 'fscanint' */
