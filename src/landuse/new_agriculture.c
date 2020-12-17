/***************************************************************************/
/**                                                                       **/
/**              n  e  w  _  a  g  r  i  c  u  l  t  u  r  e  .  c        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function allocates irrigation data of stand                       **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 08.10.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "agriculture.h"

void new_agriculture(Stand *stand)
{
  Irrigation *irrigation;
  irrigation=new(Irrigation);
  check(irrigation);
  stand->fire_sum=0.0;
  stand->growing_time=stand->growing_days=stand->age=0;
  stand->data=irrigation;
  irrigation->irrigation=FALSE;
  irrigation->irrig_event=0;
  irrigation->irrig_system=NOIRRIG;
  irrigation->ec=1;
  irrigation->conv_evap=irrigation->net_irrig_amount=irrigation->dist_irrig_amount=irrigation->irrig_amount=irrigation->irrig_stor =irrigation->no3_stor =0.0;
} /* of 'new_agriculture' */
