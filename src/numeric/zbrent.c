/***************************************************************************/
/**                                                                       **/
/**                   z  b  r  e  n  t  .  c                              **/
/**                                                                       **/
/**     Finds a zero of a function using the Van-Wijngaarden-Dekker-      **/
/**     Brent method algorithm                                            **/
/**     Algorithm taken from Numerical Recipes in C, 2nd edition          **/
/**                                                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 21.02.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define ITMAX 100
#define EPS 3.e-8
#define SIGN(a,b) ((b) > 0.0 ? fabs(a) : -fabs(a))

Real zbrent(Real (*func)(Real,void *),Real x1,Real x2,Real tol,void *data)
{
  int iter;
  Real a=x1,b=x2,c=x2,d,e,min1,min2;
  Real fa=(*func)(a,data),fb=(*func)(b,data),fc,p,q,r,s,tol1,xm;
  if((fa>0.0 && fb>0.0) || (fa<0.0 && fb<0.0))
    return x1;
    /*fail("Root must be bracketed in zbrent"); */
  fc=fb;
  for(iter=1;iter<=ITMAX;iter++)
  {
    if((fb>0.0 && fc>0.0) || (fb<0.0 && fc<0.0))
    {
      c=a;
      fc=fa;
      e=d=b-a;
    }
    if(fabs(fc)<fabs(fb))
    {
      a=b;
      b=c;
      c=a;
      fa=fb;
      fb=fc;
      fc=fa;
    }
    tol1=2.0*EPS*fabs(b)+0.5*tol;
    xm=0.5*(c-b);
    if(fabs(xm)<=tol1 || fb==0.0)
      return b;
    if(fabs(e)>=tol1 && fabs(fa)>fabs(fb))
    {
      s=fb/fa;
      if(a==c)
      {
        p=2.0*xm*s;
        q=1.0-s;
      }
      else
      {
        q=fa/fc;
        r=fb/fc;
        p=s*(2.0*xm*q*(q-r)-(b-a)*(r-1.0));
        q=(q-1.0)*(r-1.0)*(s-1.0);
      }
      if(p>0.0)
        q= -q;
      p=fabs(p);
      min1=3.0*xm*q-fabs(tol1*q);
      min2=fabs(e*q);
      if(2.0*p<(min1<min2 ? min1 : min2))
      {
        e=d;
        d=p/q;
      }
      else
      {
        d=xm;
        e=d;
      }
    }
    else
    {
      d=xm;
      e=d;
    }
    a=b;
    fa=fb;
    if(fabs(d)>tol1)
      b+=d;
    else
      b+=SIGN(tol1,xm);
    fb=(*func)(b,data);
  }
  fail(26,TRUE,"Maximum number of iterations exceeded in zbrent");
  return 0.0;
} /* of 'zbrent' */
