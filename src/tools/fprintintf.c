/***************************************************************************/
/**                                                                       **/
/**                   f  p  r  i  n  t  i  n  t  f  .c                    **/
/**                                                                       **/
/**     Function prints integer in the format dd,dddd,ddd                 **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 21.11.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include "types.h"

void fprintintf(FILE *file,int num)
{
  int i,count,d;
  if(num<0)
  {
    fprintf(file,"-");
    num=-num;
  }
  d=1;
  count=0;
  for(i=num;i>=1000;i/=1000)
  {
    d*=1000;
    count++;
  }
  fprintf(file,"%d",i);
  for(i=0;i<count;i++)
  {
    num=num % d;
    d/=1000;
    fprintf(file,",%03d",num/d);
  }
} /* of fprintintf' */
