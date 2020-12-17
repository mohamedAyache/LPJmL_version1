/***************************************************************************/
/**                                                                       **/
/**                 s  t  o  r  e  c  l  i  m  a  t  e  .  c              **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  12.10.2011                                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define checkptr(ptr) if(ptr==NULL) {printallocerr("data"); return TRUE; }

Bool storeclimate(Climatedata *store,/* pointer to climate data to be stored */
                  Climate *climate,  /* climate pointer data is read */
                  const Cell grid[], /* LPJ grid */
                  int firstyear,     /* first year of climate to be read */
                  int nyear,         /* number of years to be read */
                  const Config *config /* LPJ configuration */
                 )                     /* returns TRUE on error */
{
  int year,j;
  long count;
  /* allocate arrays for climate storage */
  store->temp=newvec(Real,climate->file_temp.n*nyear);
  checkptr(store->temp);
  store->prec=newvec(Real,climate->file_prec.n*nyear);
  checkptr(store->prec);
  if(climate->data.tmax!=NULL)
  {
    store->tmax=newvec(Real,climate->file_tmax.n*nyear);
    checkptr(store->tmax);
  }
  else
    store->tmax=NULL;
  if(climate->data.sun!=NULL)
  {
    store->sun=newvec(Real,climate->file_cloud.n*nyear);
    checkptr(store->sun);
  }
  else
    store->sun=NULL;
  if(climate->data.lwnet!=NULL)
  {
    store->lwnet=newvec(Real,climate->file_lwnet.n*nyear);
    checkptr(store->lwnet);
  }
  else
    store->lwnet=NULL;
  if(climate->data.swdown!=NULL)
  {
    store->swdown=newvec(Real,climate->file_swdown.n*nyear);
    checkptr(store->swdown);
  }
  else
    store->swdown=NULL;
  if(climate->data.wet!=NULL)
  {
    store->wet=newvec(Real,climate->file_wet.n*nyear);
    checkptr(store->wet);
  }
  else
    store->wet=NULL;
  if(climate->data.wind!=NULL)
  {
    store->wind=newvec(Real,climate->file_wind.n*nyear);
    checkptr(store->wind);
  }
  else
    store->wind=NULL;
  if(climate->data.tamp!=NULL)
  {
    store->tamp=newvec(Real,climate->file_tamp.n*nyear);
    checkptr(store->tamp);
  }
  else
    store->tamp=NULL;
  if(climate->data.lightning!=NULL)
  {
    store->lightning=newvec(Real,climate->file_lightning.n);
    checkptr(store->lightning);
    for(j=0;j<climate->file_lightning.n;j++)
      store->lightning[j]=climate->data.lightning[j];
  }
  else
    store->lightning=NULL;
  for(year=firstyear;year<firstyear+nyear;year++)
  {
    if(getclimate(climate,grid,year,config))
      return TRUE;
    count=climate->file_temp.n*(year-firstyear);
    for(j=0;j<climate->file_temp.n;j++)
      store->temp[count++]=climate->data.temp[j];
    count=climate->file_prec.n*(year-firstyear);
    for(j=0;j<climate->file_prec.n;j++)
      store->prec[count++]=climate->data.prec[j];
    if(store->sun!=NULL)
    {
      count=climate->file_cloud.n*(year-firstyear);
      for(j=0;j<climate->file_cloud.n;j++)
        store->sun[count++]=climate->data.sun[j];
    }
    if(store->tmax!=NULL)
    {
      count=climate->file_tmax.n*(year-firstyear);
      for(j=0;j<climate->file_tmax.n;j++)
        store->tmax[count++]=climate->data.tmax[j];
    }
    if(store->lwnet!=NULL)
    {
      count=climate->file_lwnet.n*(year-firstyear);
      for(j=0;j<climate->file_lwnet.n;j++)
        store->lwnet[count++]=climate->data.lwnet[j];
    }
    if(store->swdown!=NULL)
    {
      count=climate->file_swdown.n*(year-firstyear);
      for(j=0;j<climate->file_swdown.n;j++)
        store->swdown[count++]=climate->data.swdown[j];
    }
    if(store->wet!=NULL)
    {
      count=climate->file_wet.n*(year-firstyear);
      for(j=0;j<climate->file_wet.n;j++)
        store->wet[count++]=climate->data.wet[j];
    }
    if(store->wind!=NULL)
    {
      count=climate->file_wind.n*(year-firstyear);
      for(j=0;j<climate->file_wind.n;j++)
        store->wind[count++]=climate->data.wind[j];
    }
    if(store->tamp!=NULL)
    {
      count=climate->file_tamp.n*(year-firstyear);
      for(j=0;j<climate->file_tamp.n;j++)
        store->tamp[count++]=climate->data.tamp[j];
    }
  }
  return FALSE;
} /* of 'storeclimate' */

void restoreclimate(Climate *climate, /* pointer to climate data */
                    const Climatedata *store,int year)
{
  int i;
  long index;
  index=year*climate->file_temp.n;
  for(i=0;i<climate->file_temp.n;i++)
    climate->data.temp[i]=store->temp[index++];
  index=year*climate->file_prec.n;
  for(i=0;i<climate->file_prec.n;i++)
    climate->data.prec[i]=store->prec[index++];
  if(store->tmax!=NULL)
  {
    index=year*climate->file_tmax.n;
    for(i=0;i<climate->file_tmax.n;i++)
      climate->data.tmax[i]=store->tmax[index++];
  }
  if(store->sun!=NULL)
  {
    index=year*climate->file_cloud.n;
    for(i=0;i<climate->file_cloud.n;i++)
      climate->data.sun[i]=store->sun[index++];
  }
  if(store->lwnet!=NULL)
  {
    index=year*climate->file_lwnet.n;
    for(i=0;i<climate->file_lwnet.n;i++)
      climate->data.lwnet[i]=store->lwnet[index++];
  }
  if(store->swdown!=NULL)
  {
    index=year*climate->file_swdown.n;
    for(i=0;i<climate->file_swdown.n;i++)
      climate->data.swdown[i]=store->swdown[index++];
  }
  if(store->wet!=NULL)
  {
    index=year*climate->file_wet.n;
    for(i=0;i<climate->file_wet.n;i++)
      climate->data.wet[i]=store->wet[index++];
  }
  if(store->wind!=NULL)
  {
    index=year*climate->file_wind.n;
    for(i=0;i<climate->file_wind.n;i++)
      climate->data.wind[i]=store->wind[index++];
  }
  if(store->tamp!=NULL)
  {
    index=year*climate->file_tamp.n;
    for(i=0;i<climate->file_tamp.n;i++)
      climate->data.tamp[i]=store->tamp[index++];
  }
} /* of 'restoreclimate' */

void moveclimate(Climate *climate,Climatedata store,int year)
{
  climate->data.prec=store.prec+climate->file_prec.n*year;
  climate->data.temp=store.temp+climate->file_temp.n*year;
  if(climate->data.tmax!=NULL)
    climate->data.tmax=store.tmax+climate->file_tmax.n*year;
  if(climate->data.sun!=NULL)
    climate->data.sun=store.sun+climate->file_cloud.n*year;
  if(climate->data.lwnet!=NULL)
    climate->data.lwnet=store.lwnet+climate->file_lwnet.n*year;
  if(climate->data.swdown!=NULL)
    climate->data.swdown=store.swdown+climate->file_swdown.n*year;
  if(climate->data.wet!=NULL)
    climate->data.wet=store.wet+climate->file_wet.n*year;
  if(climate->data.wind!=NULL)
    climate->data.wind=store.wind+climate->file_wind.n*year;
  if(climate->data.tamp!=NULL)
    climate->data.tamp=store.tamp+climate->file_tamp.n*year;
} /* of 'moveclimate' */
