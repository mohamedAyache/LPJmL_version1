/***************************************************************************/
/**                                                                       **/
/**             b  i  g  e  n  d  i  a  n  .  c                           **/
/**                                                                       **/
/**     Determine whether byte order is big or little endian              **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 05.03.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include "types.h"
#include "swap.h"

typedef struct
{
  char hi,lo;
} Int16;

static Bool checkendian(Int16 *data)
{
  return data->hi==0;
} /* of 'checkendian' */

Bool bigendian(void)  /* return TRUE if byte order is big-endian */
{
  short data=0xff;
  return checkendian((Int16 *)&data);
} /* of 'bigendian' */
