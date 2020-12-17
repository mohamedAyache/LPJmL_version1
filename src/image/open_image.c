/***************************************************************************/
/**                                                                       **/
/**                  o  p  e  n  _  i  m  a  g  e  .  c                   **/
/**                                                                       **/
/**     extension of LPJ to couple LPJ online with IMAGE                  **/
/**     Opens connection to IMAGE model using TDT compliant socket        **/
/**                                                                       **/
/**     written by Christoph Mueller                                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 11.06.2009 by Werner von Bloh                        **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#ifdef IMAGE

Bool open_image(Config *config /* LPJ configuration */
               )               /* returns TRUE on error */
{
  if(config->rank==0)
  {
    /* Establish the TDT connection */
    printf("Connecting IMAGE model...  image_inport %d, wait %d host %s outport %d\n",config->image_inport,config->wait_image,config->image_host,config->image_outport);
    fflush(stdout);
    config->in=opentdt_socket(config->image_inport,config->wait_image);
    if(config->in==NULL)
      return TRUE;
#ifdef DEBUG_IMAGE
    printf("opened INTERFACE_to_LPJ\n");

    printf("opening LPJ_to_INTERFACE image_host %s outport %d\n",config->image_host,config->image_outport);
#endif
    config->out=connecttdt_socket(config->image_host,
                                       config->image_outport);
    if(config->out==NULL)
    {
      close_socket(config->in);
      config->in=NULL;
      return TRUE;
    }
#ifdef DEBUG_IMAGE
    printf("opened LPJ_to_INTERFACE\n");
    printf("All connections to IMAGE are established.\n");
    fflush(stdout);
#endif
  }
  return FALSE;
} /* of 'open_image' */

#endif
