/***************************************************************************/
/**                                                                       **/
/**               h  u  m  a  n  i  g  n  i  t  i  o  n  .  c             **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Landuse Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 3.4.07 Ben Poulter                                   **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real humanignition(Real popdens,
                   Ignition* ignition)
{
  Real a_nd, human_ign, k_pd;

  //a_nd = ignition->human;     /*KIRSTEN: ACHTUNG nur voruebergehend bis a_nd Input ueberprueft!!*/
  a_nd = 0.11;
  a_nd = ignition->human;     /*KIRSTEN: ACHTUNG nur voruebergehend bis a_nd Input ueberprueft!!*/
  k_pd = 6.8*(exp(-0.5*sqrt(popdens)));
  human_ign = k_pd * a_nd * popdens;
  /*printf( "a_nd= %.5f popdens= %.5f\n",a_nd,popdens);*/
  return human_ign;
} /* of 'humanignition' */
