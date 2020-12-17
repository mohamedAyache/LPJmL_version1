/***************************************************************************/
/**                                                                       **/
/**                     p  o  p  d  e  n  s  .  c                         **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Definition of datatype Popdens                                    **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Landuse Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 21.02.2007 M. Gumpenberger                           **/
/**     Last change: $Date:: 2014-11-28 14:33:41 +0100 (ven., 28 nov. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

struct popdens
{
  Climatefile file;
  Real *npopdens;
}; /* Definition of opaque datatype Popdens */

Popdens initpopdens(const Config *config /* LPJ configuration */
                   )                     /* returns pointer to population
                                            struct or NULL */
{
  Header header;
  Popdens popdens;
  String headername;
  int i,version;
  
  if(config->popdens_filename.name==NULL)
    return NULL;
  popdens=new(struct popdens);
  if(popdens==NULL)  
    return NULL;
  popdens->file.fmt=config->popdens_filename.fmt;
  if(config->popdens_filename.fmt==CDF)
  {
    if(opendata_netcdf(&popdens->file,config->popdens_filename.name,
                       config->popdens_filename.var,
                       "km-2",config))
    {
      free(popdens);
      return NULL;
    }
  }
  else
  { 
    if((popdens->file.file=openinputfile(&header,&popdens->file.swap,
                                         &config->popdens_filename,
                                         headername,
                                         &version,config))==NULL)
    {
      free(popdens);
      return NULL;
    }

    popdens->file.firstyear=header.firstyear;
    popdens->file.size=header.ncell*sizeof(short);
    popdens->file.scalar=header.scalar;
    if(config->popdens_filename.fmt==RAW)
      popdens->file.offset=config->startgrid*sizeof(short);
    else
      popdens->file.offset=config->startgrid*sizeof(short)+headersize(headername,version);
  
  /*popdens->nbands=header.nbands;*/
  
  /*printf("firstyear %d\n",popdens->firstyear);     1901
    printf("size %d\n",popdens->size);             67420*2 
    printf("n %d\n",popdens->n);             1, wenn nur 1 Zelle gerechnet wird
    printf("offset %d\n",popdens->offset);        50029*2+38=100096
    printf("config->startgrid %d\n",config->startgrid);      Pixelnummer z.B. 50029 fuer Bangladesh
    printf("headersize %d\n",headersize(LPJ_POPDENS_HEADER));     38 
    printf("header.nyear %d\n",header.nyear); 
    printf("header.firstyear %d\n",header.firstyear); 
    printf("header.order %d\n",header.order); 
    printf("header.firstcell %d\n",header.firstcell); 
    printf("header.ncell %d\n",header.ncell); 
    printf("header.nbands %d\n",header.nbands); */

  }
  popdens->file.n=config->ngridcell;
  if((popdens->npopdens=newvec(Real,popdens->file.n))==NULL)
  {
    printallocerr("npopdens");
    if(popdens->file.fmt==CDF)
      closeclimate_netcdf(&popdens->file,isroot(*config));
    else
      fclose(popdens->file.file);
    free(popdens);
    return NULL;
  }
  for(i=0;i<popdens->file.n;i++)
    popdens->npopdens[i]=0;
  return popdens;
} /* of 'initpopdens' */

/*
- called in lpj()
- opens the popdens input file (see also building file for the popdens Input (like cfts26_lu2clm.c)
- sets the popdens variables (see also manage.h)
*/

Bool readpopdens(Popdens popdens,     /* pointer to population data */
                 int year,            /* year (AD) */
                 const Cell grid[],   /* LPJ grid */
                 const Config *config /* LPJ configuration */
                )                     /* returns TRUE on error */
{
  short *vec;
  int i;
  
  year-=popdens->file.firstyear;
  if(year<0)
    year=0;
  if(popdens->file.fmt==CDF)
    return readdata_netcdf(&popdens->file,popdens->npopdens,grid,year,config);
  if(fseek(popdens->file.file,year*popdens->file.size+popdens->file.offset,SEEK_SET))
  {
    fprintf(stderr,"ERROR184: Cannot seek to population density of year %d in getpopdens().\n",year+popdens->file.firstyear);
    return TRUE;
  }
  vec=newvec(short,popdens->file.n);
  if(vec==NULL)
  {
    printallocerr("vec");
    return TRUE;
  }
  if(fread(vec,sizeof(short),popdens->file.n,popdens->file.file)!=popdens->file.n)
  {
    fprintf(stderr,"ERROR184: Cannot read population density of year %d in getpopdens().\n",year+popdens->file.firstyear);
    free(vec);
    return TRUE;
  }
  
  if(popdens->file.swap)
    for(i=0;i<popdens->file.n;i++)
      popdens->npopdens[i]=swapshort(vec[i])*popdens->file.scalar;
  else
    for(i=0;i<popdens->file.n;i++)
      popdens->npopdens[i]=vec[i]*popdens->file.scalar;
  free(vec);

  return FALSE;
} /* of 'readpopdens' */

Real getpopdens(const Popdens popdens,int cell)
{
  return popdens->npopdens[cell];
}  /* of 'getpopdens' */

void freepopdens(Popdens popdens,Bool isroot)
{
  if(popdens!=NULL)
  {
    if(popdens->file.fmt==CDF)
      closeclimate_netcdf(&popdens->file,isroot);
    else
      fclose(popdens->file.file);
    free(popdens->npopdens);
    free(popdens);
  }
} /* of 'freepopdens' */
