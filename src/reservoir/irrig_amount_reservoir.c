/***************************************************************************/
/**                                                                       **/
/**         i r r i g _ a m o u n t _ r e s e r v o i r . c               **/
/**                                                                       **/
/**     written by Hester Biemans and Werner von Bloh                     **/
/**     Wageningen University and Research Centre                         **/
/**     and                                                               **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 26.02.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void irrig_amount_reservoir(Cell grid[],const Config *config)
{
  Real *in,*out;
  Real sumout,sumin;
  int i,cell,k;

  sumout=sumin=0.0;
  grid-=config->startgrid;
  out=(Real *)pnet_output(config->irrig_res);
  in=(Real *)pnet_input(config->irrig_res);
  for(i=0;i<pnet_outlen(config->irrig_res);i++)
    out[i]=grid[pnet_outindex(config->irrig_res,i)].discharge.irrig_unmet;
  pnet_exchg(config->irrig_res);
  for(cell=pnet_lo(config->irrig_res);cell<=pnet_hi(config->irrig_res);cell++)
    if(grid[cell].ml.dam)
    {
      grid[cell].ml.resdata->ddemand=0;

      /* calculate ddemand by summing up requests */
      for(i=0;i<pnet_inlen(config->irrig_res,cell);i++)
      {
        grid[cell].ml.resdata->ddemand+=in[pnet_inindex(config->irrig_res,cell,i)]*grid[cell].ml.resdata->fraction[i];
        grid[cell].ml.resdata->mdemand+=in[pnet_inindex(config->irrig_res,cell,i)]*grid[cell].ml.resdata->fraction[i]; /*Hb 15-09-09 */
      }
      /* calculate fraction of ddemand that can be fulfilled */
      if(grid[cell].ml.resdata->ddemand>0)
      {
        grid[cell].ml.resdata->dfout_irrigation=0.0;
        for(k=0;k<NIRRIGDAYS;k++)
          grid[cell].ml.resdata->dfout_irrigation+=grid[cell].ml.resdata->dfout_irrigation_daily[k]; /*sum to get total available water for irrigation*/

        if(grid[cell].ml.resdata->dfout_irrigation<0 && grid[cell].ml.resdata->dfout_irrigation>=-0.0001)
          grid[cell].ml.resdata->dfout_irrigation=0.0;
        if(grid[cell].ml.resdata->dfout_irrigation<-0.0001)
          fail(OUTFLOW_RESERVOIR_ERR,TRUE,"dfout_irrigation<-0.0001");

        /*calculate how much of todays demand is available*/
        grid[cell].ml.resdata->demand_fraction=grid[cell].ml.resdata->dfout_irrigation/grid[cell].ml.resdata->ddemand;

        if(grid[cell].ml.resdata->demand_fraction>1)
          grid[cell].ml.resdata->demand_fraction=1;

        /* the demand is subtracted from storage for irrigation, the 'oldest' water is used first*/
        //add nutrient, en première approximation les nutrient pris dans un reservoir retourne dans le sol de la même cellule
        grid[cell].ml.resdata->dfout_irrigation_daily[0]-=grid[cell].ml.resdata->demand_fraction*grid[cell].ml.resdata->ddemand;

        for(k=0;k<NIRRIGDAYS-1;k++)
        {
          if(grid[cell].ml.resdata->dfout_irrigation_daily[k]<0)
          {
            grid[cell].ml.resdata->dfout_irrigation_daily[k+1]+=grid[cell].ml.resdata->dfout_irrigation_daily[k];
            grid[cell].ml.resdata->dfout_irrigation_daily[k]=0.0;
          }
        }
        if(grid[cell].ml.resdata->dfout_irrigation_daily[NIRRIGDAYS-1]<0 && grid[cell].ml.resdata->dfout_irrigation_daily[NIRRIGDAYS-1]>=-0.0005)
          grid[cell].ml.resdata->dfout_irrigation_daily[NIRRIGDAYS-1]=0.0;
        if(grid[cell].ml.resdata->dfout_irrigation_daily[NIRRIGDAYS-1]<-0.0005)
          fail(OUTFLOW_RESERVOIR_ERR,TRUE,"dfout_irrigation_daily[%d] <-0.0005: %.5f",NIRRIGDAYS-1,grid[cell].ml.resdata->dfout_irrigation_daily[NIRRIGDAYS-1]);

        grid[cell].discharge.mfout+=grid[cell].ml.resdata->demand_fraction*grid[cell].ml.resdata->ddemand;
        sumout+=grid[cell].ml.resdata->demand_fraction*grid[cell].ml.resdata->ddemand;

      }
      else
        grid[cell].ml.resdata->demand_fraction=0;
    }

  out=(Real *)pnet_output(config->irrig_res_back);
  in=(Real *)pnet_input(config->irrig_res_back);

  /* write demand_fraction to outbuffer */
  for(i=0;i<pnet_outlen(config->irrig_res_back);i++)
    out[i]=grid[pnet_outindex(config->irrig_res_back,i)].ml.resdata->demand_fraction;

  pnet_exchg(config->irrig_res_back);
  for(cell=pnet_lo(config->irrig_res_back);cell<=pnet_hi(config->irrig_res_back);cell++)
  {
    grid[cell].discharge.act_irrig_amount_from_reservoir=0;
    for(i=0;i<pnet_inlen(config->irrig_res_back,cell);i++)
    {
      grid[cell].discharge.act_irrig_amount_from_reservoir+=grid[cell].ml.fraction[i]*in[pnet_inindex(config->irrig_res_back,cell,i)]*grid[cell].discharge.irrig_unmet;
      sumin+=grid[cell].ml.fraction[i]*in[pnet_inindex(config->irrig_res_back,cell,i)]*grid[cell].discharge.irrig_unmet;
    }
   }
} /* of 'irrig_amount_reservoir' */
