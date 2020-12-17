/***************************************************************************/
/**                                                                       **/
/**     s  u  m  _  i  r  r  i  g  _  d  e  m  a  n  d  .  c              **/
/**                                                                       **/
/**                                                                       **/
/**     Function sums irrigation demands from cells to irrigation demand  **/
/**                                                                       **/
/**     written by Hester Biemans and Werner von Bloh                     **/
/**     Wageningen University and research centre                         **/
/**     and                                                               **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 10.01.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void sum_irrig_demand(Cell grid[],         /* LPJ grid */
                      const Config *config /* LPJ configuration */
                     )
{
  int i,cell;
  Real *in,*out;
  for(cell=0;cell<config->ngridcell;cell++)
    if(grid[cell].ml.mdemand<0)
      grid[cell].ml.mdemand=0;
  grid-=config->startgrid-config->firstgrid;
  out=(Real *)pnet_output(config->irrig_res);
  in=(Real *)pnet_input(config->irrig_res);
  for(i=0;i<pnet_outlen(config->irrig_res);i++)
    out[i]=grid[pnet_outindex(config->irrig_res,i)].ml.mdemand*
           grid[pnet_outindex(config->irrig_res,i)].coord.area;
  pnet_exchg(config->irrig_res);
  for(cell=pnet_lo(config->irrig_res);cell<=pnet_hi(config->irrig_res);cell++)
    if(grid[cell].ml.dam)
    {
      grid[cell].ml.resdata->mdemand=0;
      for(i=0;i<pnet_inlen(config->irrig_res,cell);i++)
        grid[cell].ml.resdata->mdemand+=in[pnet_inindex(config->irrig_res,cell,i)]*
                                     grid[cell].ml.resdata->fraction[i];
    }
} /* of 'sum_irrig_demand' */
