/***************************************************************************/
/**                                                                       **/
/**          f  w  r  i  t  e  r  e  s  d  a  t  a  .  c                  **/
/**                                                                       **/
/**     Function writes state variables for reservoirs                    **/
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

Bool fwriteresdata(FILE *file,const Cell *cell)
{
  fwrite(&cell->ml.reservoirfrac,sizeof(Real),1,file);
  fwrite(&cell->ml.resdata->c_pool,sizeof(Real),1,file);
  fwrite(&cell->ml.resdata->mean_demand,sizeof(Real),1,file);
  fwrite(&cell->ml.resdata->mean_inflow,sizeof(Real),1,file);
  fwrite(&cell->ml.resdata->dmass,sizeof(Real),1,file);
  fwrite(&cell->ml.resdata->k_rls,sizeof(Real),1,file);
  fwrite(cell->ml.resdata->demand_hist,sizeof(Real),NMONTH*HIST_YEARS,file);
  fwrite(cell->ml.resdata->inflow_hist,sizeof(Real),NMONTH*HIST_YEARS,file);
  return fwrite(cell->ml.resdata->level_hist,sizeof(Real),NMONTH*HIST_YEARS,file)!=NMONTH*HIST_YEARS;
} /* of 'fwriteresdata' */
