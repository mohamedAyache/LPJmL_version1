/***************************************************************************/
/**                                                                       **/
/**      i  n  i  t  r  e  s  d  a  t  a  .  c                            **/
/**                                                                       **/
/**     written by Hester Biemans and Werner von Bloh                     **/
/**     Wageningen University and Research Centre                         **/
/**     and                                                               **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 24.02.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void initresdata(Cell *cell)
{
  int j,k;
  /*initialize other characteristics of this reservoir*/
  cell->ml.resdata->dmass=0.0;
  cell->ml.resdata->dfout_res=0.0;
  for(j=0;j<NIRRIGDAYS;j++)
    cell->ml.resdata->dfout_irrigation_daily[j]=0.0;
  cell->ml.resdata->dfout_irrigation=0.0;
  cell->ml.resdata->mean_inflow=0.1; /*not zero, because in res outflow capacity is divided by this value */
  cell->ml.resdata->target_release_year=0.0;
  cell->ml.resdata->start_operational_year=0;
  cell->ml.resdata->k_rls=0.0;
  cell->ml.resdata->c=1.0;
  cell->ml.resdata->mdemand=0.0;
  cell->ml.resdata->ddemand=0.0;
  cell->ml.resdata->demand_fraction=0.0;
  cell->ml.resdata->mean_demand=0.0;
  cell->ml.resdata->mean_volume=0.0;
  cell->ml.reservoirfrac=((Real)cell->ml.resdata->reservoir.area)*1000*1000/cell->coord.area; /*area in km2 to m2 */
  cell->ml.resdata->c_pool=0.0; /*initialize c pool with 0. */
  if(cell->ml.reservoirfrac>1-cell->lakefrac)
    cell->ml.reservoirfrac=(1.0-cell->lakefrac);
  for(j=0;j<NMONTH;j++)
  {
    cell->ml.resdata->target_release_month[j]=0.0;
    cell->ml.resdata->mean_demand_month[j]=0.0;
    cell->ml.resdata->mean_level_month[j]=0.0;
    cell->ml.resdata->mean_inflow_month[j]=0.0;
    for(k=0;k<HIST_YEARS;k++)
    {
      cell->ml.resdata->demand_hist[k][j]=0.0;
      cell->ml.resdata->inflow_hist[k][j]=-1.0;
      cell->ml.resdata->level_hist[k][j]=-1.0;
    }
  }
} /* of 'initresdata' */
