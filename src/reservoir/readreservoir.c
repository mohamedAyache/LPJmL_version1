/***************************************************************************/
/**                                                                       **/
/**          r  e  a  d  r  e  s  e  r  v  o  i  r  .  c                  **/
/**                                                                       **/
/**     Function reads reservoir data from binary file                    **/
/**                                                                       **/
/**     written by Hester Biemans                                         **/
/**     Wageningen University and Research Centre                         **/
/**                                                                       **/
/**     Last change: 26.02.2009 by Werner von Bloh                        **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool readreservoir(Reservoir *res, /* reservoir data to read */
                   Bool swap,      /* Byte order has to be changed (TRUE/FALSE) */
                   FILE *file      /* pointer to binary file */
                  )                /* returns TRUE on error */
{
  if(freadint1(&res->year,swap,file)!=1)
    return TRUE;
  if(freadfloat(&res->capacity,1,swap,file)!=1)
    return TRUE;
  res->capacity*=1e12;
  if(freadfloat(&res->area,1,swap,file)!=1)
    return TRUE;
  if(freadint1(&res->inst_cap,swap,file)!=1)
    return TRUE;
  if(freadint1(&res->height,swap,file)!=1)
    return TRUE;
  if(freadint(res->purpose,NPURPOSE,swap,file)!=NPURPOSE)
    return TRUE;
  return FALSE;
} /* of 'readreservoir' */
