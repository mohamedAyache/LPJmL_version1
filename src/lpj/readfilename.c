/***************************************************************************/
/**                                                                       **/
/**                r  e  a  d  f  i  l  e  n  a  m  e  .  c               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads format and file name                               **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 18.02.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool readfilename(FILE *file,         /* pointer to text file read */
                  Filename *filename, /* returns filename and format */
                  const char *path,   /* path added to filename or NULL */
                  Bool isvar,         /* variable name supplied */
                  Bool isout          /* enable error output (TRUE/FALSE) */
                 )                    /* returns TRUE on error */
{
  String name;
  if(fscanint(file,&filename->fmt,"format",isout))
     return TRUE;
  if(filename->fmt<0 || filename->fmt>CDF)
  {
     if(isout)
       fputs("ERROR205: Invalid value for input format.\n",stderr);
     return TRUE;
  }
  if(filename->fmt==FMS)
  {
    filename->var=NULL;
    filename->name=NULL;
    return FALSE;
  }
  if(isvar && filename->fmt==CDF)
  {
    if(fscanstring(file,name))
    {
      if(isout)
        readstringerr("variable");
      return TRUE;
    }
    else
    {
      filename->var=strdup(name);
      if(filename->var==NULL)
      {
        printallocerr("variable");
        return TRUE;
      }
    }
  }
  else
    filename->var=NULL;
  if(fscanstring(file,name))
  {
    if(isout)
      readstringerr("filename");
    free(filename->var);
    return TRUE;
  }
  filename->name=addpath(name,path); /* add path to filename */
  //printf("%s\n", filename->name);
  if(filename->name==NULL)
  {
    free(filename->var);
    return TRUE;
  }
  return FALSE;
} /* of 'readfilename' */
