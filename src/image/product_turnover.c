/***************************************************************************/
/**                                                                       **/
/**    p  r  o  d  u  c  t  _  t  u  r  n  o  v  e  r  .  c               **/
/**                                                                       **/
/**     extension of LPJ to compute turnover of product pools             **/
/**                                                                       **/
/**     written by Christoph Mueller                                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 07.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/


#include "lpj.h"

#ifdef IMAGE

Real product_turnover(Image_data *data)
{
  data->prod_turn_fast=(data->timber.fast)*PRODUCT_FAST;
  data->prod_turn_slow=(data->timber.slow)*PRODUCT_SLOW;
  
  data->timber.fast*=(1.0-PRODUCT_FAST);
  data->timber.slow*=(1.0-PRODUCT_SLOW);
  return data->prod_turn_fast+data->prod_turn_slow;
} /* of 'product_turnover' */

#endif
