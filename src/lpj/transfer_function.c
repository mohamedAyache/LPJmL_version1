/***************************************************************************/
/**                                                                       **/
/**    t  r  a  n  s  f  e  r  _  f  u  n  c  t  i  o  n  .  c            **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function calculates coefficients for the transfer function        **/
/**     used in drain.c to calculate the release from each grid cell      **/
/**                                                                       **/
/**     written by Jens Heinke                                            **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-14 09:09:50 +0200 (mar., 14 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define TFACTORS_MAX (int)(3/TSTEP)  /* maximum number of transfer coefficients */
#define ISTEPS 100                   /* number of integration steps per TSTEP */

Real *transfer_function(Real rlength, /* river length in m */
                        int *size     /* number of transfer coefficients */
                       )              /* returns array of coefficents of transfer function */
{
  Real *transf; /* pointer to coefficients of transfer function */
  Real *data;
  int i,m,k;
  Real sum,ga,dt,n;
  Real u1,u2;

  data=newvec(Real,TFACTORS_MAX);
  if(data==NULL)
    return NULL;
  n=rlength>SEGLEN ? rlength/SEGLEN : 1.0;
  ga=tgamma(n);      /* value of the gamma function */
  dt=TSTEP/ISTEPS;   /* length of integration time step in d */

  sum=0.0;
  *size=0;
  u1=0.0;
  for(i=0;i<TFACTORS_MAX;i++)
  {
    data[i]=0.0;
    for(m=1;m<=ISTEPS;m++)
    {
      u2=1/ga/TAU*pow((i*TSTEP+m*dt)/TAU,(n-1))*exp(-(i*TSTEP+m*dt)/TAU);
      data[i]+=(u1+u2)/2*dt;
      u1=u2;
    }
    if(data[i]<0 || data[i]>1) /* is coefficient positive and less than one? */
    {
      fprintf(stderr,"ERROR145: Invalid coefficient in transfer function: transf[%d] = %g\n",
              i,data[i]);
      free(data);
      return NULL;
    }
    sum+=data[i];
    (*size)++;
    if((sum>0.99) || ((sum>epsilon) && (data[i]<epsilon)))
      break;
  }

  transf=newvec(Real,*size);
  if(transf==NULL)
  {
    free(data);
    return NULL;
  }
  /* normalize and store coefficients */
  for(k=0;k<(*size);k++)
    transf[k]=data[k]/sum;
  free(data);
  return transf;
} /* of 'transfer_function' */
