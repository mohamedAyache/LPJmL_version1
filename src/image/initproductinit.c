/***************************************************************************/
/**                                                                       **/
/**                 i n i t p r o d u c t i n i t . c                     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function opens file with initial product pool data                **/
/**                                                                       **/
/**     written by Christoph Mueller                                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 10.01.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#ifdef IMAGE

Productinit *initproductinit(const Config *config)
{
  Productinit *productinit;
  Header header;
  int version;
  String headername;
  if(config->prodpool_init_filename.fmt==CDF)
  {
    if(isroot(*config))
      fputs("ERROR211: NetCDF input currently not supported for product pools.\n",stderr);
    return NULL;
  }
  productinit=new(Productinit);
  if(productinit==NULL)
    return NULL;
  
  if((productinit->file=openinputfile(&header,&productinit->swap,
                                      &config->prodpool_init_filename,
                                      headername,
                                      &version,config))==NULL)
  {
    free(productinit);
    return NULL;
  }
  productinit->firstyear=header.firstyear;
  productinit->nband=header.nbands;
  productinit->size=header.ncell*sizeof(float)*2;
  if(config->prodpool_init_filename.fmt==RAW)
    productinit->offset=sizeof(float)*(config->startgrid-header.firstcell)*2;
  else
    productinit->offset=headersize(headername,version)+sizeof(float)*(config->startgrid-header.firstcell)*2;
  productinit->scalar=header.scalar;
  return productinit;
} /* of 'initproductinit' */

#endif
