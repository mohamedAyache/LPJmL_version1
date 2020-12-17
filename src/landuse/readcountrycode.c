/***************************************************************************/
/**                                                                       **/
/**             r  e  a  d  c  o  u  n  t  r  y  c  o  d  e  .  c         **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-07-09 13:39:52 +0200 (mer., 09 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool readcountrycode(FILE *file,Code *code,Bool swap)
{
  if(fread(code,sizeof(Code),1,file)!=1)
     return TRUE;
  if(swap)
  {
    code->country=swapshort(code->country);
    code->region=swapshort(code->region);
  }
  return FALSE;
} /* of *readcountrycode' */
