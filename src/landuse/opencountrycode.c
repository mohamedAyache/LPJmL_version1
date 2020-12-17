/***************************************************************************/
/**                                                                       **/
/**             o  p  e  n  c  o  u  n  t  r  y  c  o  d  e  .  c         **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 10.01.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

FILE *opencountrycode(const Filename *filename,Bool *swap,String headername,
                      int *version,Bool isout)
{
  FILE *file;
  Header header;
  file=fopen(filename->name,"rb");
  if(file==NULL)
  {
    if(isout)
      printfopenerr(filename->name);
    return NULL;
  }
  if(filename->fmt==RAW)
  {
    *swap=FALSE;
    *version=1;
  }
  else
  {
    *version=(filename->fmt==CLM) ? READ_VERSION : 2;
    if(freadanyheader(file,&header,swap,headername,version))
    {
      if(isout)
        fprintf(stderr,"ERROR154: Invalid header in '%s'.\n",filename->name);
      fclose(file);                                                       
      return NULL;
    }
  }
  return file;
} /* of 'opencountrycode' */
