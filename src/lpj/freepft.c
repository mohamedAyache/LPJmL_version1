/***************************************************************************/
/**                                                                       **/
/**                        f  r  e  e  p  f  t  .  c                      **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 24.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void freepft(Pft *pft /* Pointer to pft being freed */
            ) 
{
  pft->par->free(pft); /* call type-specific free function */
} /* of 'freepft' */
