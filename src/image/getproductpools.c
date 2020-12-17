/***************************************************************************/
/**                                                                       **/
/**                 g e t p r o d u c t p o o l s  .  c                   **/
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
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#ifdef IMAGE

Bool getproductpools(Productinit *productinit,
                     Product productpools[],
                     int ncell
                    )
{
  float *vec;
  int cell;

  if(fseek(productinit->file,productinit->offset,SEEK_SET))
  {
    fprintf(stderr,"ERROR150: Cannot seek file to position %d in getproductpools().\n",
            productinit->offset);
    return TRUE;
  } 
  vec=newvec(float,ncell*2);
  if(vec==NULL)
  {
    printallocerr("vec");
    return TRUE;
  }
  if(fread(vec,sizeof(float),ncell*2,productinit->file)!=ncell*2)
  {
    fprintf(stderr,"ERROR151: Cannot read initial product pools.\n");
    free(vec);
    return TRUE;
  } 
  if(productinit->swap) /* Has byte order to be changed? */ 
  {
    printf("swapping byte order for product pools\n");
    for(cell=0;cell<ncell;cell++)
    {
      productpools[cell].fast=(Real)swapfloat(vec[2*cell]);
      productpools[cell].slow=(Real)swapfloat(vec[2*cell+1]);
    }
  }
  else
    for(cell=0;cell<ncell;cell++)
    {
      productpools[cell].fast=(Real)vec[2*cell];
      productpools[cell].slow=(Real)vec[2*cell+1];
    }
  free(vec);
  return FALSE ;
} /* of 'getproductpools' */

#endif
