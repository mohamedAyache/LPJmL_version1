/***************************************************************************/
/**                                                                       **/
/**       f  r  e  a  d  r  e  s  d  a  t  a  .  c                        **/
/**                                                                       **/
/**     Function reads state variables for reservoirs                     **/
/**                                                                       **/
/**     written by Hester Biemans and Werner von Bloh                     **/
/**     Wageningen University and Research Centre                         **/
/**     and                                                               **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 14.10.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool freadresdata(FILE *file,Cell *cell,Bool swap)
{
  int j;
  cell->ml.resdata=new(Resdata);
  if(cell->ml.resdata==NULL)
    return TRUE;
  /*initialize other characteristics of this reservoir*/
  cell->ml.resdata->dfout_res=0.0;
  for(j=0;j<NIRRIGDAYS;j++)
    cell->ml.resdata->dfout_irrigation_daily[j]=0.0;
  cell->ml.resdata->dfout_irrigation=0.0;
  cell->ml.resdata->target_release_year=0.0;
  cell->ml.resdata->start_operational_year=0;
  cell->ml.resdata->c=1.0;
  cell->ml.resdata->mdemand=0.0;
  cell->ml.resdata->ddemand=0.0;
  cell->ml.resdata->demand_fraction=0.0;
  cell->ml.resdata->mean_volume=0.0;
  cell->ml.reservoirfrac=((Real)cell->ml.resdata->reservoir.area)*1000*1000/cell->coord.area; /*area in km2 to m2 */
  for(j=0;j<NMONTH;j++)
  {
    cell->ml.resdata->target_release_month[j]=0.0;
    cell->ml.resdata->mean_demand_month[j]=0.0;
    cell->ml.resdata->mean_level_month[j]=0.0;
    cell->ml.resdata->mean_inflow_month[j]=0.0;
  }
  freadreal1(&cell->ml.reservoirfrac,swap,file);
  freadreal1(&cell->ml.resdata->c_pool,swap,file);
  freadreal1(&cell->ml.resdata->mean_demand,swap,file);
  freadreal1(&cell->ml.resdata->mean_inflow,swap,file);
  freadreal1(&cell->ml.resdata->dmass,swap,file);
  freadreal1(&cell->ml.resdata->k_rls,swap,file);
  freadreal(cell->ml.resdata->demand_hist[0],NMONTH*HIST_YEARS,swap,file);
  freadreal(cell->ml.resdata->inflow_hist[0],NMONTH*HIST_YEARS,swap,file);
  return freadreal(cell->ml.resdata->level_hist[0],NMONTH*HIST_YEARS,swap,file)!=NMONTH*HIST_YEARS;
} /* of 'freadresdata' */
