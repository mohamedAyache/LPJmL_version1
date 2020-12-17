/***************************************************************************/
/**                                                                       **/
/**     w  i  l  d  f  i  r  e  _  i  g  n  i  t  i  o  n  s  .  c        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Landuse Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 09.03.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real wildfire_ignitions(Real d_fdi,    /* daily fire danger index */
                        Real ignition, /* ignition */
                        Real area      /* cell area */
                       )
{
  Real d_numfire;
  d_numfire= d_fdi*ignition* 0.000001 * (area*1e-4); /*per Mio ha * grid cell area*/
  /* printf("number fires  %2.10f\n",d_numfire); */ 
  return d_numfire;
} /* of 'wildfire_ignitions' */
