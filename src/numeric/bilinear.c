/***************************************************************************/
/**                                                                       **/
/**                   b  i  l  i  n  e  a  r  .  c                        **/
/**                                                                       **/
/**     Bilinear interpolation function                                   **/
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

Real bilinear(const Real data[],int nx,int ny,Real x,Real y)
{
  Real f00,f01,f10,f11;
  int ix,iy;
  ix=(int)x;
  iy=(int)y;
  x-=floor(x);
  y-=floor(y);
  f00=data[iy*nx+ix];
  f01=(x==1) ? 0 : data[(iy+1)*nx+ix];
  f10=(y==1) ? 0 : data[iy*nx+ix+1];
  f11=(x==1 || y==1) ? 0: data[(iy+1)*nx+ix+1];
  return f00*(1-x)*(1-y)+f10*x*(1-y)+f01*(1-x)*y+f11*x*y;
} /* of 'bilinear' */ 
   
