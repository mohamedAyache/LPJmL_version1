/***************************************************************************/
/**                                                                       **/
/**                     h  e  a  d  e  r  s  i  z  e  .  c                **/
/**                                                                       **/
/**     Function gets header size                                         **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  15.01.2013                                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

size_t headersize(const char *headername,int version)
{
  return sizeof(int)+((version==1) ? sizeof(Header_old) : sizeof(Header))+strlen(headername);
} /* of 'headersize' */
