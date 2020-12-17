/***************************************************************************/
/**                                                                       **/
/**                  c  l  o  s  e  _  i  m  a  g  e  .  c                **/
/**                                                                       **/
/**     extension of LPJ to couple LPJ online with IMAGE                  **/
/**     Closes connection to IMAGE model                                  **/
/**                                                                       **/
/**     written by Christoph Mueller                                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 14.01.2009 by Werner von Bloh                        **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#ifdef IMAGE

void close_image(const Config *config)
{
  if(config->rank==0)
  {
    if(config->in!=NULL)
      close_socket(config->in);
    if(config->out!=NULL)
    close_socket(config->out);
  }
} /* of 'close_image' */

#endif
