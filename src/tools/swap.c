/***************************************************************************/
/**                                                                       **/
/**                     s  w  a  p  .  c                                  **/
/**                                                                       **/
/**     Converts big endian into little endian and vice versa.            **/
/**     Needed for reading data for architectures with different          **/
/**     endianness, e.g., IBM Power6 vs. Intel Xeon.                      **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  15.01.2013                                          **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "swap.h"

static void swap(char *a,char *b)
{
  char h;
  h=*a;
  *a=*b;
  *b=h;
} /* of 'swap' */

short int swapshort(short int x)
{
  swap((char *)&x,(char *)(&x)+1);
  return x;
} /* of 'swapshort' */

int swapint(int x)
{
  swap((char *)&x,(char *)(&x)+3);
  swap((char *)&x+1,(char *)(&x)+2);
  return x;
} /* of 'swapint' */

long long swaplong(long long x)
{
  swap((char *)&x,(char *)(&x)+7);
  swap((char *)&x+1,(char *)(&x)+6);
  swap((char *)&x+2,(char *)(&x)+5);
  swap((char *)&x+3,(char *)(&x)+4);
  return x;
} /* of 'swaplong' */

double swapdouble(Num num)
{
  double ret;
  Num x;
  x.hi=swapint(num.lo);
  x.lo=swapint(num.hi);
  memcpy(&ret,&x,sizeof(Num));
  return ret;
} /* of 'swapdouble' */

float swapfloat(int num)
{
  float ret;
  num=swapint(num);
  memcpy(&ret,&num,sizeof(int));
  return ret;
} /* of 'swapfloat' */

size_t freadfloat(float *data,size_t n,Bool swap,FILE *file)
{
  size_t i,rc;
  int *tmp;
  tmp=(int *)data;
  rc=fread(tmp,sizeof(int),n,file);
  if(swap)
    for(i=0;i<rc;i++)
      data[i]=swapfloat(tmp[i]);
  return rc;
} /* of 'freadfloat' */

size_t freaddouble(double *data,size_t n,Bool swap,FILE *file)
{
  size_t i,rc;
  Num *tmp;
  tmp=(Num *)data;
  rc=fread(tmp,sizeof(Num),n,file);
  if(swap)
    for(i=0;i<rc;i++)
      data[i]=swapdouble(tmp[i]);
  return rc;
} /* of 'freaddouble' */

size_t freadreal(Real *data,size_t n,Bool swap,FILE *file)
{
  if(sizeof(Real)==sizeof(double))
    return freaddouble((double *)data,n,swap,file);
  else
    return freadfloat((float *)data,n,swap,file);
} /* of 'freadreal' */

size_t freadlong(long long *data,size_t n,Bool swap,FILE *file)
{
  size_t i,rc;
  rc=fread(data,sizeof(long long),n,file);
  if(swap)
    for(i=0;i<rc;i++)
      data[i]=swaplong(data[i]);
  return rc;
} /* of 'freadlong' */

size_t freadint(int *data,size_t n,Bool swap,FILE *file)
{
  size_t i,rc;
  rc=fread(data,sizeof(int),n,file);
  if(swap)
    for(i=0;i<rc;i++)
      data[i]=swapint(data[i]);
  return rc;
} /* of 'freadint' */

size_t freadshort(short *data,size_t n,Bool swap,FILE *file)
{
  size_t i,rc;
  rc=fread(data,sizeof(short),n,file);
  if(swap)
    for(i=0;i<rc;i++)
      data[i]=swapshort(data[i]);
  return rc;
} /* of 'freadshort' */
