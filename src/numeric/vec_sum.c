/***************************************************************************/
/**                                                                       **/
/**                           v  e  c  _  s  u  m  .  c                   **/
/**                                                                       **/
/**     Function computes sum of all vector elements                      **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 01.07.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "numeric.h"

Real vec_sum(const Real vec[],int size)
{
  int i;
  Real sum=0.0;
	
  for(i=0;i<size;i++) 
    sum+=vec[i];
   
  return sum;
} /* of 'vec_sum' */
