/***************************************************************************/
/**                                                                       **/
/**               f  w  r  i  t  e  h  e  a  d  e  r  .  c                **/
/**                                                                       **/
/**     Writing file header for LPJ related files.                        **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-06-05 12:01:12 +0200 (jeu., 05 juin 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool fwriteheader(FILE *file,             /* file pointer of binary file */
                  Header header,          /* file header */
                  const char *headername, /* header string */
                  int version             /* header version */
                 )                        /* returns TRUE on error */
{
  if(fwrite(headername,strlen(headername),1,file)!=1)
    return TRUE;
  if(fwrite(&version,sizeof(version),1,file)!=1)
    return TRUE;
  return (fwrite(&header,(version==1) ? sizeof(Header_old) : sizeof(Header),1,file)!=1);
} /* of 'fwriteheader' */
