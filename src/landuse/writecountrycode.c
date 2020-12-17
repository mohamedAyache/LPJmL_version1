/***************************************************************************/
/**                                                                       **/
/**            w  r  i  t  e  c  o  u  n  t  r  y  c  o  d  e  .  c       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
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

int writecountrycode(Outputfile *output,int index,const Cell grid[],
                     const Config *config)
{
#ifdef USE_MPI
  MPI_Status status;
#endif
  int cell,count;
  Bool rc;
  short *vec;
  vec=newvec(short,config->count);
  if(vec==NULL)
  {
    printallocerr("vec");
    rc=TRUE;
  }
  else
    rc=FALSE;
  if(iserror(rc,config))
    return 0;
  count=0;
  for(cell=0;cell<config->ngridcell;cell++)
    if(!grid[cell].skip)
      vec[count++]=(short)grid[cell].ml.manage.par->id;
#ifdef USE_MPI
  switch(output->method)
  {
    case LPJ_MPI2:
      MPI_File_write_at(output->files[index].fp.mpi_file,config->offset,vec,
                        count,MPI_SHORT,&status);
      break;
    case LPJ_GATHER:
      switch(output->files[index].fmt)
      {
        case RAW:
          mpi_write(output->files[index].fp.file,vec,MPI_SHORT,config->total,
                    output->counts,output->offsets,config->rank,config->comm);
          break;
        case TXT:
          mpi_write_txt(output->files[index].fp.file,vec,MPI_SHORT,config->total,
                        output->counts,output->offsets,config->rank,config->comm);
          break;
        case CDF:
          mpi_write_netcdf(&output->files[index].fp.cdf,vec,MPI_SHORT,config->total,
                           NO_TIME,
                           output->counts,output->offsets,config->rank,config->comm);
          break;
      }
      break;
    case LPJ_SOCKET:
      mpi_write_socket(output->socket,vec,MPI_SHORT,config->total,
                       output->counts,output->offsets,config->rank,config->comm);
      break;
   
  } /* of 'switch' */
#else
  if(output->method==LPJ_FILES)
    switch(output->files[index].fmt)
    {
      case RAW:
        if(fwrite(vec,sizeof(short),count,output->files[index].fp.file)!=count)
          fprintf(stderr,"ERROR204: Error writing output: %s.\n",strerror(errno));
        break;
      case TXT:
        for(cell=0;cell<count-1;cell++)
          fprintf(output->files[index].fp.file,"%d ",vec[cell]);
        fprintf(output->files[index].fp.file,"%d\n",vec[count-1]);
        break;
      case CDF:
        write_short_netcdf(&output->files[index].fp.cdf,vec,NO_TIME,count);
        break;
    }
  else
    writeshort_socket(output->socket,vec,count);
#endif
  free(vec);
  return count;
} /* of 'writecountrycode' */
