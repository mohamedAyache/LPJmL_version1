/***************************************************************************/
/**                                                                       **/
/**                 w  a  t  e  r  u  s  e  f  c  n  s  .  c              **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function implements wateruse functions                            **/
/**                                                                       **/
/**     written by Stefanie Rost, Werner von Bloh, Sibyll Schaphoff       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 13.12.2012                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

struct wateruse
{
  Climatefile file;
};               /* definition of opaque datatype Wateruse */

Wateruse initwateruse(const Config *config)
{
  Wateruse wateruse;
  Header header;
  String headername;
  int version;
  wateruse=new(struct wateruse);
  if(wateruse==NULL)
    return NULL;
  wateruse->file.fmt=config->wateruse_filename.fmt;
  if(config->wateruse_filename.fmt==CDF)
  { 
    if(opendata_netcdf(&wateruse->file,config->wateruse_filename.name,config->wateruse_filename.var,NULL,config))
    {
      free(wateruse);
      return NULL;
    }
  }
  else
  {
    if((wateruse->file.file=openinputfile(&header,&wateruse->file.swap,
                                          &config->wateruse_filename,
                                          headername,
                                          &version,config))==NULL)
    {
      free(wateruse);
      return NULL;
    }
    wateruse->file.firstyear=header.firstyear;
    wateruse->file.nyear=header.nyear;
    wateruse->file.size=header.ncell*sizeof(int);
    if(config->wateruse_filename.fmt==RAW)
      wateruse->file.offset=sizeof(int)*(config->startgrid-header.firstcell);
    else
      wateruse->file.offset=headersize(headername,version)+sizeof(int)*(config->startgrid-header.firstcell);
    wateruse->file.scalar=(version==1) ? 1000 : header.scalar;
  }
  return wateruse;
} /* of 'initwateruse' */

Bool getwateruse(Wateruse wateruse,   /* Pointer to wateruse data */
                 Cell grid[],         /* cell grid */
                 int year,            /* year of wateruse data (AD) */
                 const Config *config /* LPJ configuration */
                )                     /* returns TRUE on error */
{
  int cell,*vec;
  Real *data;
  if(year>=wateruse->file.firstyear && year<wateruse->file.firstyear+wateruse->file.nyear)
  {
    if(wateruse->file.fmt==CDF)
    {
      data=newvec(Real,config->ngridcell);
      if(data==NULL)
      {
        printallocerr("data");
        return TRUE;
      }
      if(readdata_netcdf(&wateruse->file,data,grid,year,config))
      {
        free(data);
        return TRUE;
      }
      for(cell=0;cell<config->ngridcell;cell++)
        grid[cell].discharge.wateruse=data[cell];
      free(data);
    }
    else
    {
      if(fseek(wateruse->file.file,
               wateruse->file.offset+wateruse->file.size*(year-wateruse->file.firstyear),
               SEEK_SET))
      {
        fprintf(stderr,"ERROR150: Cannot seek file to year %d in wateruse().\n",year);
        return TRUE;
      } 
      vec=newvec(int,config->ngridcell);
      if(vec==NULL)
      {
        printallocerr("vec");
        return TRUE;
      }
      if(fread(vec,sizeof(int),config->ngridcell,wateruse->file.file)!=config->ngridcell)
      {
        fprintf(stderr,"ERROR151: Cannot read wateruse for year %d.\n",year);
        free(vec);
        return TRUE;
      } 
      if(wateruse->file.swap) /* Has byte order to be changed? */ 
        for(cell=0;cell<config->ngridcell;cell++)
          grid[cell].discharge.wateruse=((Real)swapint(vec[cell]))*wateruse->file.scalar;
      else
        for(cell=0;cell<config->ngridcell;cell++)
          grid[cell].discharge.wateruse=((Real)vec[cell])*wateruse->file.scalar;
      free(vec);
    }
  }
  else 
    /* no wateruse data available for year, set all to zero */
    for(cell=0;cell<config->ngridcell;cell++)
      grid[cell].discharge.wateruse=0;
  return FALSE;
} /* of 'getwateruse' */

void freewateruse(Wateruse wateruse,Bool isroot)
{
  if(wateruse!=NULL)
  {
    if(wateruse->file.fmt==CDF)
      closeclimate_netcdf(&wateruse->file,isroot);
    else
      fclose(wateruse->file.file);
    free(wateruse);
  }
} /* of 'freewateruse' */
