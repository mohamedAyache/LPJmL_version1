/***************************************************************************/
/**                                                                       **/
/**             f  r  e  e  c  r  o  p  d  a  t  e  s  .  c               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function deallocates crop dates data                              **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for landuse Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 27.05.2005                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void freecropdates(Cropdates *cropdates)
{
    free(cropdates);
} /* of 'freecropdates' */
