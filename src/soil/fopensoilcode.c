/***************************************************************************/
/**                                                                       **/
/**              f  o  p  e  n  s  o  i  l  c  o  d  e  .  c              **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function opens soil code file                                     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 17.01.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

FILE *fopensoilcode(const Filename *filename, /* filename  of soil code file */
                    Bool *swap,           /* byte order has to be changed */
                    int *version,         /* returns version of CLM file */
                    Bool isout            /* enable output of error messages */
                   )                      /* returns file pointer or NULL */
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
    *swap=FALSE;
  else
  {
    if(filename->fmt==CLM)
      *version=READ_VERSION;
    else
      *version=2;
    if(freadheader(file,&header,swap,LPJSOIL_HEADER,version))
    {
      if(isout)
        fprintf(stderr,"ERROR154: Invalid header in '%s'.\n",filename->name);
      fclose(file);
      return NULL;
    }
  }
  return file;
} /* of 'fopensoilcode' */
