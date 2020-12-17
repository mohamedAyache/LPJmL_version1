/***************************************************************************/
/**                                                                       **/
/**                         l  i  n  e  a  r  .  c                        **/
/**                                                                       **/
/**     linear interpolation function                                     **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 07.03.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include <math.h>
#include "types.h"
#include "numeric.h"

Real linear(const Real data[],int nx,Real x)
{
  Real f0,f1;
  int ix;
  ix=(int)x;
  x-=floor(x);
  f0=data[ix];
  f1=(x==1) ? 0 : data[ix+1];
  return f0*(1-x)+f1*x;
} /* of 'linear' */ 
