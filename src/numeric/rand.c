/***************************************************************************/
/**                                                                       **/
/**                         r  a  n  d  .  c                              **/
/**                                                                       **/
/**     Random number generator                                           **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 27.05.2011                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include "types.h"
#include "numeric.h"

#define modulus 2147483647
#define multiplier 16807 
#define q 127773
#define r 2836
#define MASK 123459876

static int seed=12345678; /* seed for random number generator (see randfrac) */

void setseed(int init) 
{
  seed=init;
} /* of 'setseed' */

int getseed(void)
{
  return seed;
} /* of 'getseed' */

Real randfrac(void) 
{
 /*
  * DESCRIPTION
  * Returns a random floating-point number in the range 0-1.
  * Uses and updates the global variable 'seed' which may be initialised to any
  * positive integral value (the same initial value will result in the same 
  * sequence of returned values on subsequent calls to randfloat every time
  * the program is run)
  *
  * Reference: Park & Miller 1988 CACM 31: 1192
  * based on Numerical Recipes ran0() 
  *
  */
  int k;
  Real ans;
  seed^=MASK; /* XORing to avoid seed being 0 */
  k=seed/q;
  seed=multiplier*(seed-k*q)-r*k;
  if (seed<0) 
    seed+=modulus;
  ans=(Real)seed/(Real)modulus;
  seed^=MASK; /* XORing to avoid seed being 0 */
  return ans;
} /* of 'randfrac' */
