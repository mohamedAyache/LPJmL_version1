/***************************************************************************/
/**                                                                       **/
/**      f  w  r  i  t  e  _  a  g  r  i  c  u  l  t  u  r  e  .  c       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function writes irrigation data of stand                          **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "agriculture.h"

Bool fwrite_agriculture(FILE *file,        /* pointer to binary file */
                        const Stand *stand /* stand pointer */
                       )                   /* returns TRUE on error */
{
  Byte b;
  Irrigation *irrigation;
  irrigation=stand->data;
  b=(Byte)irrigation->irrigation;
  fwrite(&b,sizeof(b),1,file);
  fwrite1(&irrigation->irrig_event,sizeof(int),file);
  fwrite1(&irrigation->irrig_system,sizeof(int),file);
  fwrite1(&irrigation->ec,sizeof(Real),file);
  fwrite1(&irrigation->conv_evap,sizeof(Real),file);
  fwrite1(&irrigation->net_irrig_amount,sizeof(Real),file);
  fwrite1(&irrigation->dist_irrig_amount,sizeof(Real),file);
  fwrite1(&irrigation->irrig_amount,sizeof(Real),file);
  fwrite1(&irrigation->irrig_stor,sizeof(Real),file);
  fwrite1(&stand->growing_time,sizeof(int),file);
  fwrite1(&stand->growing_days,sizeof(int),file);
  fwrite1(&stand->age,sizeof(int),file);
  fwrite1(&stand->pft_id,sizeof(int),file);
  return FALSE;
} /* of 'fwrite_agriculture' */
