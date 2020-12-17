/***************************************************************************/
/**                                                                       **/
/**                     n  e  w  _  i  m  a  g  e  .  c                   **/
/**                                                                       **/
/**     extension of LPJ to couple LPJ online with IMAGE                  **/
/**     Allocates and initializes memory for IMAGE data                   **/
/**                                                                       **/
/**     written by Christoph Mueller                                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 09.01.2013 by Werner von Bloh                        **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#ifdef IMAGE

Image_data *new_image(const Product *productpool)
{
  Image_data *image_data;
  image_data=new(Image_data);
  check(image_data);
  /* data received from IMAGE */
  image_data->fburnt=image_data->timber.fast=
  image_data->timber.slow=image_data->timber_frac=
  image_data->timber_f.fast=image_data->timber_f.slow=
  image_data->timber_turnover=image_data->deforest_emissions=
  image_data->store_bmtree_yield=image_data->biomass_yield_annual=0.0
;
  image_data->timber.fast=productpool->fast;
  image_data->timber.slow=productpool->slow;
  /* data sent to image */
  image_data->anpp=image_data->arh=0.0;
  return image_data;
} /* of 'new_image' */

#endif
