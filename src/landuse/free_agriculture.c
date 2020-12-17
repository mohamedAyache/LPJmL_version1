/***************************************************************************/
/**                                                                       **/
/**           f  r  e  e  _  a  g  r  i  c  u  l  t  u  r  e  .  c        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function deallocates irrigation data of stand                     **/
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

void free_agriculture(Stand *stand)
{
  free(stand->data);
} /* of 'free_agriculture' */
