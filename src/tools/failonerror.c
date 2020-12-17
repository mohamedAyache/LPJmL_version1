/***************************************************************************/
/**                                                                       **/
/**              f  a  i  l  o  n  e  r  r  o  r  .  c                    **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function writes out error message and terminates program          **/
/**     The parallel version checks the return code on all tasks and      **/
/**     prints error message only on task zero.                           **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 02.04.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void failonerror(const Config *config,  /* grid configuration */
                 int rc,             /* return code (0=no error) */
                 int errorcode,      /* error code returned from program */
                 const char *msg     /* error message printed */
                )
{
#ifdef USE_MPI
  int sum;
  String s;
  /* sum up error codes of all tasks and broadcast result */
  MPI_Allreduce(&rc,&sum,1,MPI_INT,MPI_SUM,config->comm);
  if(sum)
  {
    /* error occured, end program */
    MPI_Finalize(); /* finish MPI */
    if(config->rank==0)
    {
      if(config->ntask==1)
        fail(errorcode,FALSE,msg); /* write error message and exit*/
      if(sum==config->ntask)
        snprintf(s,STRING_LEN,"%s on all tasks",msg);
      else
        snprintf(s,STRING_LEN,"%s on %d tasks",msg,sum);
      fail(errorcode,FALSE,s); /* write error message and exit*/
    }
    else
      exit(errorcode);     /* exit only */
  }
#else
  if(rc)
    fail(errorcode,FALSE,msg);
#endif
} /* of 'failonerror' */
