/***************************************************************************/
/**                                                                       **/
/**               f  s  c  a  n  l  i  m  i  t  .  c                      **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Functions reads limit from configuration file                     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 07.09.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool fscanlimit(FILE *file,  /* pointer to text file */
                Limit *limit, /* on return limit read */
                Bool isout
               )             /* returns TRUE on error */
{
  if(fscanreal(file,&limit->low,"low limit",isout))
    return TRUE;
  if(fscanreal(file,&limit->high,"high limit",isout))
    return TRUE;
  return FALSE;
} /* of 'fscanlimit' */
