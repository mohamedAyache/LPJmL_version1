/***************************************************************************/
/**                                                                       **/
/**                 g  e  t  r  o  u  t  e  .  c                          **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads river route from file                               **/
/**                                                                       **/
/**     written by Stefanie Rost, Werner von Bloh, Sibyll Schaphoff       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool getroute(FILE *file, /* File pointer to binary file */
              Routing *r, /* River route read */
              Bool swap   /* Byte order has to be changed */
             )            /* Returns TRUE on error */
{
  /* Function reads river route from file */
  if(fread(r,sizeof(Routing),1,file)!=1)
    return TRUE;
  if(swap)
  {
    /* byte order has to be changed */
    r->index=swapint(r->index);
    r->len=swapint(r->len);
  }
  return FALSE;
} /* of 'getroute' */
