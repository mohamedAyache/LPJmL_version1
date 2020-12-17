/***************************************************************************/
/**                                                                       **/
/**                 c  l  i  m  b  u  f  .  h                             **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  16.01.2013                                          **/
/**                                                                       **/
/***************************************************************************/

#ifndef CLIMBUF_H /* Already included? */
#define CLIMBUF_H

#include "date.h"

#define CLIMBUFSIZE 20 /* size of climate buffer */
#define NDAYS 31       /* maximum number of days in a month */
#define NDAYS_PREC 10

/* Definitions of datatypes */

typedef struct
{
  Real gdd5; /* number of days with temp > 5 deg C */
  Real temp[NDAYS];
  Real dval_prec[NDAYS+1]; /* daily precipitation values (mm) */
  Real temp_min; /* minimum annual temperature (deg C) */
  Real temp_max; /* maximum annual temperature (deg C) */
  Real atemp_mean;
  Real atemp; /* annual mean temperature, used to update atemp_mean20 */
  Real atemp_mean20; /* 20-year mean of annual mean temperature */
  Real atemp_mean20_fix; /* 20-year mean of annual mean temperature, fixed for phen_variety computation*/
  Real mtemp;
  Real mprec;
  Real mpet;
  Buffer min,max;
  Real mprec20[NMONTH]; /* 20-year average monthly precip */
  Real mpet20[NMONTH]; /* 20-year average monthly precip */
  Real mtemp20[NMONTH]; /* 20-year average monthly precip */
  Real mtemp_min20; /*20-year average of coldest month temperature*/
} Climbuf;

/* Definitions of macros */

#define initgdd5(climbuf) climbuf.gdd5=0
#define updategdd5(climbuf,temp) if(temp>5) (*climbuf).gdd5++

/* Declaration of functions */

extern Bool new_climbuf(Climbuf *);
extern void init_climbuf(Climbuf *);
extern void daily_climbuf(Climbuf *,Real);
extern void monthly_climbuf(Climbuf *,Real,Real,Real,int);
extern void annual_climbuf(Climbuf *);
extern Bool fwriteclimbuf(FILE *,const Climbuf *);
extern Bool freadclimbuf(FILE *,Climbuf *,Bool);
extern void freeclimbuf(Climbuf *);

#endif
