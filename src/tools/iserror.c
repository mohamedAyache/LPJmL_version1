/***************************************************************************/
/**                                                                       **/
/**              i  s  e  r  r  o  r  .  c                                **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     The parallel version checks the return code on all tasks          **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 27.09.2011                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool iserror(int rc,              /* return code (0=no error) */
             const Config *config /* LPJ configuration */
            )             /* returns TRUE if one or more tasks have errors */
{
#ifdef USE_MPI
  int sum;
  /* sum up error codes of all tasks and broadcast result */
  MPI_Allreduce(&rc,&sum,1,MPI_INT,MPI_SUM,config->comm);
  return sum!=0;
#else
  return rc!=0; 
#endif
} /* of 'iserror' */
