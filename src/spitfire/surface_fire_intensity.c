/***************************************************************************/
/**                                                                       **/
/*          s u r f a c e _ f i r e _ i n t e n s i t y . c               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Kirsten Thonicke                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 06.05.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define HEAT_CONTENT_FUEL 18000.0

Real surface_fire_intensity(Real fuel_consump,
                            Real fire_frac,
                            Real ros_forward)
{
  Real surface_intens;
  if (fire_frac <= 0.000001)
    surface_intens = 0;
  else
  {
    fuel_consump*=1e-3;
    ros_forward/=60.0;
    surface_intens=HEAT_CONTENT_FUEL*(fuel_consump/fire_frac)*ros_forward;
  }
  return surface_intens;   
} /* of 'surface_fire_intensity' */
