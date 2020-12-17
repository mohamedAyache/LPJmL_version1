/***************************************************************************/
/**                                                                       **/
/**                          n  e  p  _  s  u  m  .  c                    **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 17.07.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real nep_sum(const Cell grid[],const Config *config)
{
  int cell;
  Real sum,sum_total;
  sum=sum_total=0;
  for(cell=0;cell<config->ngridcell;cell++)
    sum+=grid[cell].balance.nep*grid[cell].coord.area;
#ifdef USE_MPI
  MPI_Reduce(&sum,&sum_total,1,
             (sizeof(Real)==sizeof(double)) ? MPI_DOUBLE : MPI_FLOAT,
             MPI_SUM,0,config->comm);
  return sum_total;
#else
  return sum;
#endif
} /* of 'nep_sum' */
