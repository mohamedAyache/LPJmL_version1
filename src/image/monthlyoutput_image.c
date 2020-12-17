/***************************************************************************/
/**                                                                       **/
/**     m  o  n  t  h  l  y  o  u  t  p  u  t  _  i  m  a  g  e  .  c     **/
/**                                                                       **/
/**     extension of LPJ to couple LPJ online with IMAGE                  **/
/**     Set output for Image climate data                                 **/
/**                                                                       **/
/**     written by Christoph Mueller                                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 21.09.2011 by Werner von Bloh                        **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#ifdef IMAGE

void monthlyoutput_image(Output *output,
                         const Climate *climate, /* climate data */
                         int cell,               /* index of cell */
                         int month               /* month (0..11) */
                        )
{
  output->mtemp_image=(getcelltemp(climate,cell))[month];
  output->mprec_image=(getcellprec(climate,cell))[month];
  if(climate->data.sun!=NULL)
    output->msun_image=(getcellsun(climate,cell))[month];
  if(climate->data.wet!=NULL)
    output->mwet_image=(getcellwet(climate,cell))[month];
} /* of 'monthlyoutput_image' */

#endif
