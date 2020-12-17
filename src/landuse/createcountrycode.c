/***************************************************************************/
/**                                                                       **/
/**       c  r  e  a  t  e  c  o  u  n  t  r  y  c  o  d  e  .  c         **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 19.02.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

FILE *createcountrycode(const char *filename,int year,int ncell)
{
  FILE *file;
  Header header;
  file=fopen(filename,"wb");
  if(file==NULL)
  {
    printfopenerr(filename);
    return NULL;
  }
  header.firstcell=0;
  header.ncell=ncell;
  header.order=CELLYEAR;
  header.nbands=2;
  header.firstyear=year;
  header.nyear=1;
  if(fwriteheader(file,header,LPJ_COUNTRY_HEADER,LPJ_COUNTRY_VERSION))
  {
    fprintf(stderr,"ERROR152: Cannot write header in '%s'.\n",filename);
    fclose(file);                                                       
    return NULL;
  }
  return file;
} /* of 'createcountrycode' */
