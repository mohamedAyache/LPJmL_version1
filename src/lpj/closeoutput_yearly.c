/***************************************************************************/
/**                                                                       **/
/**      c  l  o  s  e  o  u  t  p  u  t  _  y  e  a  r  l  y  .  c       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function closes yearly output files                               **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 18.06.2012                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void closeoutput_yearly(Outputfile *output,const Config *config)
{
 int i;
 if(isroot(*config))
   for(i=0;i<config->n_out;i++)
     if(config->outputvars[i].oneyear && output->files[config->outputvars[i].id].isopen)
       switch(config->outputvars[i].filename.fmt)
       {
         case RAW: case TXT:
           fclose(output->files[config->outputvars[i].id].fp.file);
           break;
         case CDF:
           close_netcdf(&output->files[config->outputvars[i].id].fp.cdf);
           break;
       }
} /* of 'closeoutput_yearly' */
