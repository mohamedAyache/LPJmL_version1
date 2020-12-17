/***************************************************************************/
/**                                                                       **/
/**             f  w  r  i  t  e  _  c  r  o  p  .  c                     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     Function writes crop specific variables into binary file          **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 25.11.2005                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "crop.h"

Bool fwrite_crop(FILE *file,const Pft *pft)
{
    return (fwrite(pft->data, sizeof(Pftcrop), 1, file) != 1);
} /* of 'fwrite_crop' */
