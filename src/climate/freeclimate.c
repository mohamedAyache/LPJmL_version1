/***************************************************************************/
/**                                                                       **/
/**                   f  r  e  e  c  l  i  m  a  t  e  .  c               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function closes open files and frees allocated memory             **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 20.09.2011                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void freeclimatedata(Climatedata *data)
{
  free(data->tmax);
  free(data->prec);
  free(data->temp);
  free(data->sun);
  free(data->lwnet);
  free(data->swdown);
  free(data->wet);
  free(data->wind);
  free(data->tamp);
  free(data->lightning);
} /* of 'freeclimatedata' */

void freeclimate(Climate *climate,Bool isroot)
{
  if(climate!=NULL)
  {
    closeclimatefile(&climate->file_temp,isroot);
    closeclimatefile(&climate->file_prec,isroot);
    if(climate->data.tmax!=NULL)
      closeclimatefile(&climate->file_tmax,isroot);
    if(climate->data.sun!=NULL)
      closeclimatefile(&climate->file_cloud,isroot);
    if(climate->data.lwnet!=NULL)
      closeclimatefile(&climate->file_lwnet,isroot);
    if(climate->data.swdown!=NULL)
      closeclimatefile(&climate->file_swdown,isroot);
    if(climate->data.wind!=NULL)
      closeclimatefile(&climate->file_wind,isroot);
    if(climate->data.tamp!=NULL)
      closeclimatefile(&climate->file_tamp,isroot);
    if(climate->data.wet!=NULL)
      closeclimatefile(&climate->file_wet,isroot);
#ifdef IMAGE
    if(climate->file_temp_var.file!=NULL)
      closeclimatefile(&climate->file_temp_var,isroot);
    if(climate->file_prec_var.file!=NULL)
      closeclimatefile(&climate->file_prec_var,isroot);
#endif
    free(climate->co2.data);
    freeclimatedata(&climate->data);
  }
  free(climate);
} /* of 'freeclimate' */
