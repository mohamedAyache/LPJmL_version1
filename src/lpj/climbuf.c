/***************************************************************************/
/**                                                                       **/
/**                 c  l  i  m   b  u  f  .  c                            **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 23.01.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define k (1.0/12.0)
#define kk 0.05

Bool new_climbuf(Climbuf *climbuf)
{
  int d,m;
  climbuf->max=newbuffer(CLIMBUFSIZE);
  if(climbuf->max==NULL)
  {
    climbuf->min=NULL;
    return TRUE;
  }
  climbuf->min=newbuffer(CLIMBUFSIZE);
  if(climbuf->min==NULL)
    return TRUE;
  climbuf->atemp_mean=0;
  climbuf->atemp=0;
  climbuf->atemp_mean20=-9999;
  climbuf->atemp_mean20_fix=0;
  climbuf->dval_prec[0]=0;
  climbuf->mprec=0;
  climbuf->mpet=0;
  for(d=0;d<NDAYS;d++)
    climbuf->temp[d]=0;
  for(m=0;m<NMONTH;m++)
    climbuf->mpet20[m]=climbuf->mprec20[m]=climbuf->mtemp20[m]=-9999;
  return FALSE;
} /* of 'new_climbuf' */

void init_climbuf(Climbuf *climbuf)
{
  climbuf->temp_min=HUGE_VAL;
  climbuf->temp_max=-HUGE_VAL;
  climbuf->gdd5=0;
  climbuf->mprec=0;
  climbuf->mpet=0;
} /* of 'init_climbuf' */

void daily_climbuf(Climbuf *climbuf,Real temp)
{
  int d;
  updategdd5(climbuf,temp);
  for(d=1;d<NDAYS;d++)
    climbuf->temp[d-1]=climbuf->temp[d];
  climbuf->temp[NDAYS-1]=temp;
} /* of  'daily_climbuf' */

void monthly_climbuf(Climbuf *climbuf,Real mtemp,Real mprec,Real mpet,int month)
{
  int d;
  Real temp_avg;
  temp_avg=0;
  for(d=0;d<NDAYS;d++)
    temp_avg+=climbuf->temp[d];
  temp_avg/=NDAYS;
  if(climbuf->temp_min>mtemp)
    climbuf->temp_min=mtemp;
  if(climbuf->temp_max<mtemp)
    climbuf->temp_max=mtemp;
  climbuf->atemp_mean=(1-k)*climbuf->atemp_mean+k*mtemp;
  climbuf->mprec20[month]= (climbuf->mprec20[month]<-9998) ? mprec : (1-kk)*climbuf->mprec20[month]+kk*mprec;
  climbuf->mpet20[month]= (climbuf->mpet20[month]<-9998) ? mpet : (1-kk)*climbuf->mpet20[month]+kk*mpet;
  climbuf->mtemp20[month]= (climbuf->mtemp20[month]<-9998) ? mtemp : (1-kk)*climbuf->mtemp20[month]+kk*mtemp;
  climbuf->atemp+=mtemp*k;
} /* of 'monthly_climbuf' */

void annual_climbuf(Climbuf *climbuf)
{
  updatebuffer(climbuf->min,climbuf->temp_min);
  updatebuffer(climbuf->max,climbuf->temp_max);
  climbuf->atemp_mean20 = (climbuf->atemp_mean20<-9998) ? climbuf->atemp : (1-kk)*climbuf->atemp_mean20+kk*climbuf->atemp;
  climbuf->atemp=0;
  climbuf->mtemp_min20 = getbufferavg(climbuf->min);
} /* of 'annual_climbuf' */

Bool fwriteclimbuf(FILE *file,const Climbuf *climbuf)
{
  fwrite(&climbuf->temp_max,sizeof(Real),1,file);
  fwrite(&climbuf->temp_min,sizeof(Real),1,file);
  fwrite(&climbuf->atemp_mean,sizeof(Real),1,file);
  fwrite(&climbuf->atemp_mean20,sizeof(Real),1,file);
  fwrite(&climbuf->atemp_mean20_fix,sizeof(Real),1,file);
  fwrite(&climbuf->gdd5,sizeof(Real),1,file);
  fwrite(climbuf->dval_prec,sizeof(Real),1,file);
  fwrite(climbuf->temp,sizeof(Real),NDAYS,file);
  fwrite(climbuf->mpet20,sizeof(Real),NMONTH,file);
  fwrite(climbuf->mprec20,sizeof(Real),NMONTH,file);
  fwrite(climbuf->mtemp20,sizeof(Real),NMONTH,file);
  fwritebuffer(file,climbuf->min);
  return fwritebuffer(file,climbuf->max);
} /* of 'fwriteclimbuf' */

Bool freadclimbuf(FILE *file,Climbuf *climbuf,Bool swap)
{
  freadreal1(&climbuf->temp_max,swap,file);
  freadreal1(&climbuf->temp_min,swap,file);
  freadreal1(&climbuf->atemp_mean,swap,file);
  freadreal1(&climbuf->atemp_mean20,swap,file);
  freadreal1(&climbuf->atemp_mean20_fix,swap,file);
  freadreal1(&climbuf->gdd5,swap,file);
  freadreal1(climbuf->dval_prec,swap,file);
  freadreal(climbuf->temp,NDAYS,swap,file);
  freadreal(climbuf->mpet20,NMONTH,swap,file);
  freadreal(climbuf->mprec20,NMONTH,swap,file);
  freadreal(climbuf->mtemp20,NMONTH,swap,file);
  climbuf->min=freadbuffer(file,swap);
  if(climbuf->min==NULL)
  {
    climbuf->max=NULL;
    return TRUE;
  }
  climbuf->max=freadbuffer(file,swap);
  climbuf->mtemp_min20 = getbufferavg(climbuf->min);
  climbuf->atemp=0;
  return (climbuf->max==NULL);
} /* of 'freadclimbuf' */

void freeclimbuf(Climbuf *climbuf)
{
  freebuffer(climbuf->max);
  freebuffer(climbuf->min);
} /* of 'freeclimbuf' */
