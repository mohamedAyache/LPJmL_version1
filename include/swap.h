/***************************************************************************/
/**                                                                       **/
/**                     s  w  a  p  .  h                                  **/
/**                                                                       **/
/**     Converts big endian into little endian and vice versa             **/
/**     Needed for reading data under IA32                                **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  15.01.2013                                          **/
/**                                                                       **/
/***************************************************************************/

#ifndef SWAP_H /* Already included? */
#define SWAP_H

/* Definition of datatypes */

typedef struct
{
  int lo,hi;
} Num;

/* Declaration of functions */

extern short int swapshort(short int);
extern int swapint(int);
extern long long swaplong(long long);
extern double swapdouble(Num);
extern float swapfloat(int);
extern size_t freadint(int *,size_t,Bool,FILE *);
extern size_t freadlong(long long *,size_t,Bool,FILE *);
extern size_t freadreal(Real *,size_t,Bool,FILE *);
extern size_t freadfloat(float *,size_t,Bool,FILE *);
extern size_t freaddouble(double *,size_t,Bool,FILE *);
extern size_t freadshort(short *,size_t,Bool,FILE *);
extern Bool bigendian(void);

/* Definitions of macros */

#define freadint1(data,swap,file) freadint(data,1,swap,file)
#define freadreal1(data,swap,file) freadreal(data,1,swap,file)
#define freadshort1(data,swap,file) freadshort(data,1,swap,file)
#define freadlong1(data,swap,file) freadlong(data,1,swap,file)


#endif
