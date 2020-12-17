/***************************************************************************/
/**                                                                       **/
/**        f  r  e  a  d  _  a  g  r  i  c  u  l  t  u  r  e  .  c        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads irrigation data of stand                           **/
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

Bool fread_agriculture(FILE *file,   /* pointer to binary file */
                       Stand *stand, /* stand pointer */
                       Bool swap     /* byte order has to be changed */
                      )              /* returns TRUE on error */
{
  Byte b;
  Irrigation *irrigation;
  irrigation=new(Irrigation);
  stand->data=irrigation;
  if(irrigation==NULL)
    return TRUE;
  fread(&b,sizeof(b),1,file);
  irrigation->irrigation=b;
  freadint1(&irrigation->irrig_event,swap,file);
  freadint1(&irrigation->irrig_system,swap,file);
  freadreal1(&irrigation->ec,swap,file);
  freadreal1(&irrigation->conv_evap,swap,file);
  freadreal1(&irrigation->net_irrig_amount,swap,file);
  freadreal1(&irrigation->dist_irrig_amount,swap,file);
  freadreal1(&irrigation->irrig_amount,swap,file);
  freadreal1(&irrigation->irrig_stor,swap,file);
  freadint1(&stand->growing_time,swap,file);
  freadint1(&stand->growing_days,swap,file);
  freadint1(&stand->age,swap,file);
  return freadint1(&stand->pft_id,swap,file)!=1;
} /* of 'fread_agriculture' */
