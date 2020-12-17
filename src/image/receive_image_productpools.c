/***************************************************************************/
/**                                                                       **/
/**     r e c e i v e _ i m a g e _ p r o d u c t p o o l s . c           **/
/**                                                                       **/
/**     extension of LPJ to couple LPJ online with IMAGE                  **/
/**                                                                       **/
/**     written by Christoph Mueller                                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 10.01.2013 by Werner von Bloh                        **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#ifdef IMAGE

Bool receive_image_productpools(Cell *cell,          /* LPJ grid */
                                const Config *config /* LPJ configuration */
                               )                     /* returns TRUE on error */
{
  Bool rc;
  int i;
  Timber *image_timber_distribution;
  Real sum;
#ifdef USE_MPI
  int *counts,*offsets;
#endif

  /* get timber product pools from IMAGE */
  image_timber_distribution = newvec(Timber,config->ngridcell);
  check(image_timber_distribution);
#ifdef USE_MPI
  counts=newvec(int,config->ntask);
  check(counts);
  offsets=newvec(int,config->ntask);
  check(offsets);
  getcounts(counts,offsets,config->nall,sizeof(Timber)/sizeof(float),config->ntask);
  rc=mpi_read_socket(config->in,(float *)image_timber_distribution,MPI_FLOAT,
                     config->nall*sizeof(Timber)/sizeof(float),counts,
                     offsets,config->rank,config->comm);
  free(counts);
  free(offsets);
#else
  rc=readfloat_socket(config->in,(float *)image_timber_distribution,
                      config->ngridcell*sizeof(Timber)/sizeof(float));

#ifdef DEBUG_IMAGE
  printf("received timber product pools\n");
  fflush(stdout);
#endif
#endif
  for(i=0;i<config->ngridcell;i++)
  {
    if(!cell[i].skip)
    {
      sum=image_timber_distribution[i].fast + image_timber_distribution[i].slow;
#ifdef DEBUG_IMAGE
      /*if(sum > 1.000000001 || sum < 1.0-0.000000001)*/
      if(sum < 1.0-0.000000001 && sum > 0.00000001)
      /*if(sum>0.00000001 && sum<1.0)*/
/*      if(sum>0)
      {
        printf("DARNIT NUMBERS! timber distribution in %d is %.20f %.20f timber_frac %g fburn %g\n",
          i,image_timber_distribution[i].fast,image_timber_distribution[i].slow,
          cell[i].image_data->timber_frac,cell[i].image_data->fburnt);
        fflush(stdout);
      }*/
#endif
      /* correcting timber distribution fractions if needed */
      if(sum>1.0)
      {
        if(image_timber_distribution[i].fast>1.0) 
          image_timber_distribution[i].fast=1.0;
        if(image_timber_distribution[i].slow>1.0)
          image_timber_distribution[i].slow=1.0;
        image_timber_distribution[i].fast=(float)1.0-image_timber_distribution[i].slow;
      }

      cell[i].ml.image_data->timber_f.fast=(Real)image_timber_distribution[i].fast;
      cell[i].ml.image_data->timber_f.slow=(Real)image_timber_distribution[i].slow;
#ifdef DEBUG_IMAGE
      /*if(cell[i].coord.lon>-2.5 && cell[i].coord.lon<-2.0 && cell[i].coord.lat>48.0 && cell[i].coord.lat<48.5)*//*(i==1 || i==67032)*/
      if(cell[i].coord.lon>102.2 && cell[i].coord.lon<102.3 && cell[i].coord.lat>28.7 && cell[i].coord.lat<28.8)
      {
        printf("cell %d %g/%g\n",i,cell[i].coord.lon,cell[i].coord.lat);
        fflush(stdout);
        printf("timber pools[%d]: %f %f\n",i,image_data[i].fast,image_data[i].slow);
        fflush(stdout);
        printf("timber distribution[%d]: %f %f\n",i,image_timber_distribution[i].fast,image_timber_distribution[i].slow);
        fflush(stdout);
      }
#endif
    }
  }
  free(image_timber_distribution);
  return rc;
} /* of 'receive_image_productpools' */

#endif
