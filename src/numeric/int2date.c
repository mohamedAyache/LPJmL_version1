/***************************************************************************/
/**                                                                       **/
/**                     i  n  t  2  d  a  t  e  .  c                      **/
/**                                                                       **/
/**     Function converts long int in the format YYYYMMDD into date       **/
/**     string                                                            **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  29.01.2009                                          **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include "types.h"
#include "date.h"

char *int2date(long date)
{
  static char str[13];
  sprintf(str,"%s %2d %4d",months[(date % 10000)/100-1],
          (int)(date % 100),(int)(date/10000));
  return str;
} /* of 'int2date' */
