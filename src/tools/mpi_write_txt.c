/***************************************************************************/
/**                                                                       **/
/**                  m  p  i  _  w  r  i  t  e  _  t  x  t  .  c          **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function writes text output from all tasks using MPI_Gatherv      **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 05.10.2011                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#ifdef USE_MPI

static int write_float(FILE *file,const float vec[],int n)
{
  int i,rc;
  rc=0;
  for(i=0;i<n-1;i++)
    rc+=fprintf(file,"%g ",vec[i]);
  rc+=fprintf(file,"%g\n",vec[n-1]);
  return rc;
} /* of 'write_float' */

static int write_short(FILE *file,const short vec[],int n)
{
  int i,rc;
  rc=0;
  for(i=0;i<n-1;i++)
    rc+=fprintf(file,"%d ",vec[i]);
  rc+=fprintf(file,"%d\n",vec[n-1]);
  return rc;
} /* of 'write_short' */

int mpi_write_txt(FILE *file,        /* File pointer to text file */
                  void *data,        /* data to be written to disk */
                  MPI_Datatype type, /* MPI datatype of data */
                  int size,
                  int counts[],
                  int offsets[],
                  int rank,          /* MPI rank */
                  MPI_Comm comm      /* MPI communicator */
                 )                   /* returns number of items written to disk */
{
  int rc;
  MPI_Aint extent;
  void *vec;
  MPI_Type_extent(type,&extent);
  if(rank==0)
  {
    vec=malloc(size*extent); /* allocate receive buffer */
    if(vec==NULL)
    {
      printallocerr("vec");
      rc=TRUE;
    }
    else 
      rc=FALSE;
  }
  MPI_Bcast(&rc,1,MPI_INT,0,comm);
  if(rc)
    return 0;
  MPI_Gatherv(data,counts[rank],type,vec,counts,offsets,type,0,comm);
  if(rank==0)
  {
    if(type==MPI_FLOAT)
      rc=write_float(file,vec,size); /* write float data to file */
    else if(type==MPI_SHORT)
      rc=write_short(file,vec,size); /* write short data to file */
    free(vec);
  }
  MPI_Barrier(comm);
  return rc;
} /* of 'mpi_write_txt' */
#endif
