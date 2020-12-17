/***************************************************************************/
/**                                                                       **/
/**          f  p  r  i  n  t  r  e  s  d  a  t  a  .  c                  **/
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

static void fprinthist(FILE *file,Real hist[HIST_YEARS][NMONTH])
{
  int i,j;
  fprintf(file,"Year");
  for(j=0;j<NMONTH;j++)
    fprintf(file," %-6s",months[j]);
  fprintf(file,"\n");
  for(i=0;i<HIST_YEARS;i++)
  {
    fprintf(file,"%4d",i);
    for(j=0;j<NMONTH;j++)
      fprintf(file," %6.1f",hist[i][j]);
    fprintf(file,"\n");
  }
} /* of 'fprinthist' */

void fprintresdata(FILE *file,const Cell *cell)
{
  fprintf(file,"Reservoir frac:\t%g\n"
               "C pool:\t%g\n"
               "meand demand:\t%g\n"
               "mean inflow:\t%g\n"
               "water mass:\t%g\n" 
               "k_rls:\t%g\n",
          cell->ml.reservoirfrac,cell->ml.resdata->c_pool,
          cell->ml.resdata->mean_demand,cell->ml.resdata->mean_inflow,
          cell->ml.resdata->dmass,cell->ml.resdata->k_rls);
  fprintf(file,"demand history:\n");
  fprinthist(file,cell->ml.resdata->demand_hist);
  fprintf(file,"inflow history:\n");
  fprinthist(file,cell->ml.resdata->inflow_hist);
  fprintf(file,"level history:\n");
  fprinthist(file,cell->ml.resdata->level_hist);
} /* of 'fprintresdata' */
