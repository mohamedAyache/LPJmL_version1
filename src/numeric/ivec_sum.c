/***************************************************************************/
/**                                                                       **/
/**                       i  v  e  c  _  s  u  m  .  c                    **/
/**                                                                       **/
/**     Function computes sum of all integer vector elements              **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 15.07.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "numeric.h"

int ivec_sum(const int vec[],int size)
{
  int i;
  int sum=0;
	
  for(i=0;i<size;i++) 
    sum+=vec[i];
   
  return sum;
} /* of 'ivec_sum' */
