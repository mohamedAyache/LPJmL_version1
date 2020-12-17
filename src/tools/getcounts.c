/***************************************************************************/
/**                                                                       **/
/**                  g  e  t  c  o  u  n  t  s  .  c                      **/
/**                                                                       **/
/**     Function computes counts and offsets used by MPI_Gatherv/         **/
/**     MPI_Scatterv                                                      **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 05.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include "types.h"

void getcounts(int counts[],int offsets[],int size,int n,int tasks)
{
  int i;
  for(i=0;i<tasks;i++)
  {
    counts[i]=size/tasks;
    if(i<size % tasks)
      counts[i]++;
    counts[i]*=n;
  }
  offsets[0]=0;
  for(i=1;i<tasks;i++)
    offsets[i]=offsets[i-1]+counts[i-1];
} /* of 'getcounts' */
