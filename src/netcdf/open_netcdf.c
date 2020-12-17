/***************************************************************************/
/**                                                                       **/
/**                o  p  e  n  _  n  e  t  c  d  f  .  c                  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function opens NetCDF files for identical filenames only once     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-09-29 16:02:37 +0200 (lun., 29 sept. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#if defined(USE_NETCDF) || defined(USE_NETCDF4)
#include <netcdf.h>

typedef struct
{
  char *filename;
  int ncid;
  int count;
} Item;

static List *list=NULL;
#endif

int open_netcdf(const char *filename,int *ncid,Bool *isopen)
{
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  int i,rc;
  Item *item;
  if(list==NULL)
    list=newlist();
  else
  {
    for(i=0;i<getlistlen(list);i++)
    {
      item=(Item *)getlistitem(list,i);
      if(!strcmp(item->filename,filename)) /* file already open? */
      {
        *ncid=item->ncid; /* yes, copy id */
        *isopen=TRUE;
        item->count++;
        return 0;
      }
    }
  }
  *isopen=FALSE;
  /* file not open now, open it */
  rc=nc_open(filename,NC_NOWRITE,ncid);
  if(rc)
    return rc;
  /* add open file to list */
  item=new(Item);
  if(item==NULL)
    return 1;
  item->filename=strdup(filename);
  if(item->filename==NULL)
  {
    free(item);
    return 1;
  }
  item->ncid=*ncid;
  item->count=1;
  addlistitem(list,item);
  return 0;
#else
  return 1;
#endif
} /* of 'open_netcdf' */

void free_netcdf(int ncid)
{
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  int i;
  Item *item;
  if(list==NULL)
    nc_close(ncid);
  else
    for(i=0;i<getlistlen(list);i++)
    {
      item=(Item *)getlistitem(list,i);
      if(ncid==item->ncid)
      {
        item->count--;
        if(item->count==0) /* last open file? */
        {
          free(item->filename);
          nc_close(ncid);
          dellistitem(list,i);
        }
        break;
     }
   }
#endif
} /* of 'free_netcdf' */
