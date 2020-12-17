/***************************************************************************/
/**                                                                       **/
/**        f  p  r  i  n  t  h  e  a  d  e  r  .  c                       **/
/**                                                                       **/
/**     Printing file header for LPJ related files.                       **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  06.12.2012                                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void fprintheader(FILE *file,Header header)
{
  fprintf(file,"Order:\t\t");
  switch(header.order)
  {
    case CELLYEAR:
      fprintf(file,"CELLYEAR\n");
      break;
    case YEARCELL:
      fprintf(file,"YEARCELL\n");
      break;
    default:
      fprintf(file,"%d\n",header.order);
  }
  fprintf(file,"First year:\t%6d\n"
         "Last year:\t%6d\n"
         "First cell:\t%6d\n"
         "Number of cells:%6d\n"
         "Number of bands:%6d\n"
         "cellsize:\t%4.2f\n"
         "conversion factor:\t%g\n",
         header.firstyear,header.firstyear+header.nyear-1,
         header.firstcell,header.ncell,header.nbands,header.cellsize,header.scalar);
} /* of 'fprintheader' */
