/***************************************************************************/
/**                                                                       **/
/**                         a  v  g  t  e  m  p  .  c                     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function gets annual average temperature                          **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  06.01.2009                                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real avgtemp(const Climate *climate,int cell)
{
  int i,n;
  Real avg;
  n=(climate->file_temp.isdaily)  ? NDAYYEAR : NMONTH;
  avg=0;
  for(i=0;i<n;i++)
    avg+=climate->data.temp[cell*n+i];
  return avg/n;
} /* of 'avgtemp' */   
