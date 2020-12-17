/***************************************************************************/
/**                                                                       **/
/**              s  e  e  k  s  o  i  l  c  o  d  .  c                    **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function seeks in soil code file to specified position            **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 05.12.2012                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

int seeksoilcode(FILE *file,  /* file pointer */
                 int pos,     /* position in soil code file */
                 int version, /* version of CLM file */
                 int nsoil    /* number of soil types */
                )             /* return code of fseek */
{
  if(nsoil>UCHAR_MAX)
    return fseek(file,pos*sizeof(short)+headersize(LPJSOIL_HEADER,version),SEEK_SET);
  else
    return fseek(file,pos,SEEK_SET);
} /* of 'seeksoilcode' */
