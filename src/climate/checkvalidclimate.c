/***************************************************************************/
/**                                                                       **/
/**        c  h  e  c  k  v  a  l  i  d  c  l  i  m  a  t  e  .  c        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function checks NetCDF climate files for missing values and skips **/
/**     corresponding LPJ cells                                           **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 04.02.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

static Bool checkvalid(Climatefile *climate, /* climate data file */
                       Cell grid[],          /* LPJ grid */
                       Config *config        /* LPJ configuration */
                      )                      /* returns TRUE on error */
{
  Bool iserr;
#ifdef USE_MPI
  int i,*counts;
#endif
  int invalid,invalid_total;
  if(climate->fmt==CDF)
  {
    /* check data for first year */
    invalid=checkvalidclimate_netcdf(climate,grid,0,config);
    iserr=(invalid==-1);
    if(iserror(iserr,config))
      return TRUE;
    config->count-=invalid;
#ifdef USE_MPI
    MPI_Reduce(&invalid,&invalid_total,1,MPI_INT,MPI_SUM,0,config->comm);
    counts=newvec(int,config->ntask);
    if(counts==NULL)
    {
      printallocerr("counts");
      iserr=TRUE;
    }
    else
      iserr=FALSE;
    if(iserror(iserr,config))
      return TRUE;
    MPI_Allgather(&config->count,1,MPI_INT,counts,1,MPI_INT,
                  config->comm);
    config->offset=0;
    for(i=0;i<config->rank;i++)
      config->offset+=counts[i];
    MPI_Allreduce(&config->count,&config->total,1,MPI_INT,MPI_SUM,
                  config->comm);
    free(counts);
#else
    invalid_total=invalid;
    config->total=config->count;
#endif
    if(config->total==0)
    {
      if(isroot(*config))
        fputs("ERROR207: No cell with valid climate data found.\n",stderr);
      return TRUE;
    }
    if(isroot(*config) && invalid_total)
      fprintf(stderr,"WARNING008: No climate data for %d cells, will be skipped.\n",
              invalid_total);
  }
  return FALSE;
} /* of 'checkvalid' */

Bool checkvalidclimate(Climate *climate, /* pointer to climate file */
                       Cell grid[],      /* LPJ grid */
                       Config *config    /* LPJ configuration */
                      )                  /* returns TRUE on error */
{
  /* check data in temperature file */
  if(checkvalid(&climate->file_temp,grid,config))
    return TRUE;
  return FALSE;
} /* of 'checkvalidclimate' */
