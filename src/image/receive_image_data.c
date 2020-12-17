/***************************************************************************/
/**                                                                       **/
/**     r  e  c  e  i  v  e  _  i  m  a  g  e  _  d  a  t  a  .  c        **/
/**                                                                       **/
/**     extension of LPJ to couple LPJ online with IMAGE                  **/
/**                                                                       **/
/**     written by Christoph Mueller                                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 10.01.2013 by Werner von Bloh                        **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#ifdef IMAGE

Bool receive_image_data(Cell *cell,          /* LPJ grid */
                        int npft,            /* number of natural PFTs */
                        int ncft,            /* number of crop PFTS */
                        const Config *config /* LPJ configuration */
                       )                     /* returns TRUE on error */
{
  if(config->withlanduse!=NO_LANDUSE)
  {
    if(receive_image_luc(cell,npft,ncft,config))
    {
      fprintf(stderr,"ERROR172: Simulation stopped for lack of LUC data from IMAGE\n");
      fflush(stderr);
      return TRUE;
    }
  }
  if(receive_image_productpools(cell,config))
  {
    fprintf(stderr,"ERROR173: Simulation stopped for lack of product pool data from IMAGE\n");
    fflush(stderr);
    return TRUE;
  }
  return FALSE;
} /* of 'receive_image_data' */

#endif
