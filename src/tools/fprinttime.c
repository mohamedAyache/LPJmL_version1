/***************************************************************************/
/**                                                                       **/
/**                   f  p  r  i  n  t  t  i  m  e  .  c                  **/
/**                                                                       **/
/**     Function prints seconds in format d+hh:mm:ss                      **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-08-20 08:58:44 +0200 (mer., 20 août #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "types.h"

void fprinttime(FILE *file,int s)
{
  int h,m,d;
  d=s/(3600*24);
  if(d>0)
  {
    fprintf(file,"%d+",d);
    s-=d*3600*24;
  }
  h=s/3600;
  s-=h*3600;
  m=s/60;
  fprintf(file,"%02d:%02d:%02d",h,m,s-m*60);
} /* of 'fprinttime' */
