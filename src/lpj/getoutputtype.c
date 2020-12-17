/***************************************************************************/
/**                                                                       **/
/**          g  e  t  o  u  t  p  u  t  t  y  p  e  .  c                  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function determines output data type                              **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 28.03.2011                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Type getoutputtype(int index)
{
  switch(index)
  {
    case SEASONALITY: case SDATE: case HDATE: case GRID: case COUNTRY:
    case REGION:
      return LPJ_SHORT;
    default:
      return LPJ_FLOAT;
  }
} /* of 'getoutputtype' */
