/***************************************************************************/
/**                                                                       **/
/**                   n  u  m  e  r  i  c  .  h                           **/
/**                                                                       **/
/**     Header for numerical utility routines                             **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-11-06 11:17:06 +0100 (ven. 06 nov. 20#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef NUMERIC_H /* Already included? */
#define NUMERIC_H

#define beta 0.17

/* Definition of datatypes */

typedef Real (*Bisectfcn)(Real,void *);

/* Declaration of functions */

extern Real bisect(Real (*)(Real,void *),Real,Real,void *,Real,Real,int); /* find zero */
extern Real zbrent(Real (*)(Real,void *),Real,Real,Real,void *); /* find zero */
extern Real leftmostzero(Real (*)(Real,void *),Real,Real,void *,Real,Real,int); /* find leftmost zero */
extern void linreg(Real *,Real *,const Real[],int); /* linear regression */
extern void setseed(int); /* set seed of random number generator */
extern int getseed(void); /* get seed of random number generator */
extern Real randfrac(void); /* random number generator */
extern void petpar(Real *,Real *,Real *,Real,Real,Real,Real);
extern void petpar2(Real *,Real *,Real *,Real,Real,Real,Real,Real,Bool);
extern void petpar3(Real *,Real *,Real *,Real,Real,Real,Real);
extern Real bilinear(const Real [],int,int,Real,Real);
extern Real linear(const Real [],int,Real); /* linear interpolation */
extern Real vec_sum(const Real[],int); /* vector sum */
extern int ivec_sum(const int[],int); /* vector sum */
extern Real gamma(Real);

#ifndef USE_RAND48
/* if drand48(),srand48() functions are not defined, use randfrac,setseed instead */
#define drand48() randfrac()
#define srand48(seed) setseed(seed)
#endif
#ifndef USE_TGAMMA
#define tgamma(x) gamma(x)
#endif

#endif
