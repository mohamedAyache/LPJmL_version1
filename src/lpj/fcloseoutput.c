/***************************************************************************/
/**                                                                       **/
/**               f  c  l  o  s  e  o  u  t  p  u  t  .  c                **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function closes all open output files. For buffer output method   **/
/**     all data stored in buffers are written to disk.                   **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 18.06.2012                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

static int compress(const char *filename)
{
  int rc;
  char *cmd,*newfile;
  newfile=stripsuffix(filename);
  if(newfile==NULL)
    return TRUE;
  cmd=malloc(strlen(newfile)+strlen("gzip -f")+2);
  if(cmd==NULL)
  {
    free(newfile);
    return TRUE;
  }
  strcat(strcpy(cmd,"gzip -f "),newfile);
  free(newfile);
  rc=system(cmd);
  free(cmd);
  return rc;
} /* of 'compress' */

void fcloseoutput(Outputfile *output,  /* Output file array */
                  const Config *config /* LPJ configuration */
                 )
{
  int i;
  for(i=0;i<output->n;i++)
    if(output->files[i].isopen)  /* output file is open? */
    {
#ifdef USE_MPI
      switch(output->method)
      {
        case LPJ_MPI2:
          MPI_File_close(&output->files[i].fp.mpi_file);
          if(output->files[i].compress)
          {
            MPI_Barrier(config->comm); 
            if(isroot(*config))
              compress(output->files[i].filename);
          }
          break;
        case LPJ_GATHER:
          if(isroot(*config) && !output->files[i].oneyear)
          {
            switch(output->files[i].fmt)
            {
              case RAW: case TXT:
                fclose(output->files[i].fp.file);
                break;
              case CDF:
                close_netcdf(&output->files[i].fp.cdf);
                break;
            }
            if(output->files[i].compress)
              compress(output->files[i].filename);
          }
          break;
      } /* of switch */
#else
      if(output->method==LPJ_FILES && !output->files[i].oneyear)
      {
         switch(output->files[i].fmt)
         {
           case RAW: case TXT:
             fclose(output->files[i].fp.file);
             break;
           case CDF:
             close_netcdf(&output->files[i].fp.cdf);
             break;
         } /* of switch */
         if(output->files[i].compress)
           compress(output->files[i].filename);
      }
#endif
    }
#ifdef USE_MPI
  if(output->method!=LPJ_MPI2)
  {
    free(output->counts);
    free(output->offsets);
    if(output->method==LPJ_SOCKET && isroot(*config) && output->socket!=NULL)
      close_socket(output->socket);
  }
#else
  if(output->method==LPJ_SOCKET && output->socket!=NULL)
    close_socket(output->socket);
#endif
  free(output->files);
  freecoordarray(output->index);
  freecoordarray(output->index_all);
  free(output);
} /* of 'fcloseoutput' */
