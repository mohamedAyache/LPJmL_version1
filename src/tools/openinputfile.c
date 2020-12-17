/***************************************************************************/
/**                                                                       **/
/**               o  p  e  n  i  n  p  u  t  f  i  l  e  .  c             **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Functions opens input file and reads file header                  **/
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

FILE *openinputfile(Header *header, /* pointer to file header */
                    Bool *swap, /* byte order has to be changed (TRUE/FALSE) */
                    const Filename *filename, /* file name */
                    String headername, /* clm file header string */
                    int *version, /* clm file version */
                    const Config *config /* grid configuration */
                   )           /* returns file pointer to open file or NULL */
{
  FILE *file;
  if((file=fopen(filename->name,"rb"))==NULL)
  {
    if(config->rank==0)
      printfopenerr(filename->name);
    return NULL;
  }
  if(filename->fmt==RAW)
  {
    header->order=CELLYEAR;
    header->firstyear=config->firstyear;
    header->nyear=config->lastyear-config->firstyear+1;
    *swap=FALSE;
    header->firstcell=config->firstgrid;
    header->ncell=config->nall;
    header->nbands=0;
    header->scalar=1;
    *version=1;
  }
  else
  {
    *version=(filename->fmt==CLM) ? READ_VERSION : 2;
    if(freadanyheader(file,header,swap,headername,version))
    {
      if(config->rank==0)
        fprintf(stderr,"ERROR154: Invalid header in '%s'.\n",filename->name);
      fclose(file);
      return NULL;
    }
    if(*version>1 && round(header->cellsize*1000)/1000.!=config->resolution.lat)
    {
      if(config->rank==0)
        fprintf(stderr,"ERROR154: Cell size %g different from %g in '%s'.\n",
                header->cellsize,config->resolution.lat,filename->name);
      fclose(file);
      return NULL;
    }
    if(header->firstyear>config->firstyear)
      if(config->rank==0)
        fprintf(stderr,"WARNING004: First year in '%s'=%d greater than %d.\n",
                filename->name,header->firstyear,config->firstyear);
    if(config->firstgrid<header->firstcell ||
       config->nall+config->firstgrid>header->ncell+header->firstcell)
    {
      if(config->rank==0)
        fprintf(stderr,"ERROR155: gridcells [%d,%d] in '%s' not in [%d,%d].\n",
                header->firstcell,header->ncell+header->firstcell-1,filename->name,
                config->firstgrid,config->nall+config->firstgrid-1);
      fclose(file);
      return NULL;
    }
  }
  return file;
} /* of 'openinputfile' */
