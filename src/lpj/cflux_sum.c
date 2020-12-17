/***************************************************************************/
/**                                                                       **/
/**                    c  f  l  u  x  _  s  u  m  .  c                    **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function calculates total carbon flux.                            **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 14.03.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real cflux_sum(const Cell grid[],   /* array of grid cells */
               const Config *config /* LPJ configuration */
              )                     /* returns total carbon flux (gC) */
{
  int cell;
  Real sum,sum_total;
  sum=sum_total=0;
  for(cell=0;cell<config->ngridcell;cell++)
    sum+=(grid[cell].balance.nep-grid[cell].output.firec-grid[cell].output.flux_firewood+grid[cell].output.flux_estab
          -grid[cell].output.flux_harvest-grid[cell].balance.biomass_yield)*grid[cell].coord.area;
#ifdef USE_MPI
  /*
   * Use global reduce operation in order to sum carbon fluxes of all
   * tasks
   */
  MPI_Reduce(&sum,&sum_total,1,
             (sizeof(Real)==sizeof(double)) ? MPI_DOUBLE : MPI_FLOAT,
             MPI_SUM,0,config->comm);
  return sum_total;
#else
  return sum;
#endif
} /* of 'cflux_sum' */
