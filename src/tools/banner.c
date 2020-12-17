/***************************************************************************/
/**                                                                       **/
/**                      b  a  n  n  e  r  .  c                           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function prints copyright notice                                  **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 30.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include <string.h>
#include "types.h"

void banner(const char * const *s,int n,int size)
{
  int i,len;
  repeatch('*',size);
  putchar('\n');
  for(i=0;i<n;i++)
  {
    len=strlen(s[i]);
    fputs("****",stdout);
    repeatch(' ',(size-8-len)/2);
    fputs(s[i],stdout);
    repeatch(' ',(size-8-len)/2+(len+size) %2);
    puts("****");
  }
  repeatch('*',size);
  putchar('\n');
} /* of 'banner' */
