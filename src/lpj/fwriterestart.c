/***************************************************************************/
/**                                                                       **/
/**                 f  w  r  i  t  e  r  e  s  t  a  r  t  .  c           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Functions writes restart file. Due to 32bit position vector       **/
/**     file size is limited to 2GB.                                      **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#ifndef _WIN32
#include <unistd.h>
#endif

#define MSGTAG 101 /* MPI message tag */

Bool fwriterestart(const Cell grid[],   /* cell array               */
                   int npft,            /* number of natural PFTs   */
                   int ncft,            /* number of crop PFTs      */
                   int year,            /* year                     */
                   const Config *config /* LPJ configuration        */
                  )                     /* returns FALSE on success */
{
#ifdef USE_MPI
  MPI_Status status;
  Bool iserror;
#endif
  FILE *file;
  long long *index;
  Header header;
  Restartheader restartheader;
  if(config->rank==0)
    /* create file */
    file=fopen(config->write_restart_filename,"wb");
  else
  {
#ifdef USE_MPI
    /* wait until previous task has finished work */
    MPI_Recv(&iserror,1,MPI_INT,config->rank-1,MSGTAG,config->comm,&status);
    if(iserror)
    {
      if(config->rank<config->ntask-1)
        MPI_Send(&iserror,1,MPI_INT,config->rank+1,MSGTAG,config->comm);
      return TRUE;
    }
#endif
    /* append file */
    file=fopen(config->write_restart_filename,"r+b");
  }
  if(file==NULL)
  {
    printfopenerr(config->write_restart_filename);
#ifdef USE_MPI
    iserror=TRUE;
    if(config->rank<config->ntask-1)
      MPI_Send(&iserror,1,MPI_INT,config->rank+1,MSGTAG,config->comm);
#endif
    return TRUE;
  }
  if(config->rank==0)
  {
    /* set header data */
    header.order=CELLYEAR;
    header.firstyear=year;
    header.nyear=1;
    header.firstcell=config->startgrid;
    header.ncell=config->nall;
    header.nbands=npft+ncft;
    header.scalar=1;
    header.cellsize=(float)config->resolution.lat;
    /* write header */
    fwriteheader(file,header,RESTART_HEADER,RESTART_VERSION);
    restartheader.landuse=(config->withlanduse!=NO_LANDUSE);
    restartheader.sdate_option=config->sdate_option;
    restartheader.river_routing=config->river_routing;
    restartheader.datatype=(sizeof(Real)==sizeof(float)) ? LPJ_FLOAT : LPJ_DOUBLE;
    fwrite(&restartheader,sizeof(restartheader),1,file);
    /* skip index vector */
    fseek(file,sizeof(long long)*config->nall,SEEK_CUR);
  }
  else
    fseek(file,0,SEEK_END); /* seek to end of file for append */
  index=newvec(long long,config->ngridcell);
  check(index);
  /* write cell data and get index vector */
  if(fwritecell(file,index,grid,config->ngridcell,npft,config->nagtree,ncft,config->sdate_option,config->river_routing)!=config->ngridcell)
  {
    fprintf(stderr,"ERROR153: Cannot write data in restart file '%s': %s\n",
            config->write_restart_filename,strerror(errno));
    free(index);
    fclose(file);
#ifdef USE_MPI
    iserror=TRUE;
    if(config->rank<config->ntask-1)
      MPI_Send(&iserror,1,MPI_INT,config->rank+1,MSGTAG,config->comm);
#endif
    return TRUE;
  }
  /* seek just after header + config->startgrid-config->firstgrid  */
  fseek(file,headersize(RESTART_HEADER,RESTART_VERSION)+sizeof(Restartheader)+sizeof(long long)*(config->startgrid-config->firstgrid),SEEK_SET);
  /* write position vector */
  fwrite(index,sizeof(long long),config->ngridcell,file);
#ifndef _WIN32
  if(config->ntask>1)
    /* wait until all data are completely written */
    fsync(fileno(file));
#endif
  fclose(file);
  free(index);
#ifdef USE_MPI
  iserror=FALSE;
  if(config->rank<config->ntask-1)
    /* send message to next task to write further data */
    MPI_Send(&iserror,1,MPI_INT,config->rank+1,MSGTAG,config->comm);
#endif
  return FALSE;
} /* of 'fwriterestart' */
