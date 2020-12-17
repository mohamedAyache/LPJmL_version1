/***************************************************************************/
/**                                                                       **/
/**      f  r  e  a  d  r  e  s  t  a  r  t  h  e  a  d  e  r  .  c       **/
/**                                                                       **/
/**     Reading file header for LPJ related files. Detects                **/
/**     whether byte order has to be changed                              **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-08-20 08:58:44 +0200 (mer., 20 août #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool freadrestartheader(FILE *file, /* file pointer of binary file */
                        Restartheader *header, /* file header to be read */
                        Bool swap /* set to TRUE if data is in different order */
                       ) /* returns TRUE on error */
{
  if(fread(header,sizeof(Restartheader),1,file)!=1)
    return TRUE;
  if(swap)
  {
    header->landuse=swapint(header->landuse);
    header->river_routing=swapint(header->river_routing);
    header->sdate_option=swapint(header->sdate_option);
    header->datatype=(Type)swapint((int)header->datatype);
  }
  return FALSE;
} /* of 'freadrestartheader' */
