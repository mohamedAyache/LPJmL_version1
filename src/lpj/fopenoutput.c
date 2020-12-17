/***************************************************************************/
/**                                                                       **/
/**                  f  o  p  e  n  o  u  t  p  u  t  .  c                **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function creates files for output                                 **/
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

#include <signal.h>
#include "lpj.h"

static void handler(int UNUSED(num))
{
  fail(SOCKET_ERR,FALSE,"Output channel is broken"); 
} /* of 'handler' */

static int getnyear(int index)
{
  if(index==REGION || index==COUNTRY || index==GRID)
    return 0;
  if(ismonthlyoutput(index))
    return 12;
  if(isdailyoutput(index)) 
    return NDAYYEAR;
  return 1;
} /* of 'getnyear' */

static Bool create(Netcdf *cdf,const char *filename,int index,
                   Coord_array *array,const Config *config)
{
  int size;
  size=outputsize(config->outputvars[index].id,
                  config->npft[GRASS]+config->npft[TREE],
                  config->nbiomass,
                  config->nagtree,
                  config->npft[CROP]);
  if(size==1)
    return create_netcdf(cdf,filename,
                         (config->outputvars[index].id==GRID) ? "soilcode" :
                         config->outnames[config->outputvars[index].id].var,
                         (config->outputvars[index].id==GRID) ? "soil code" :
                         config->outnames[config->outputvars[index].id].descr,
                         (config->outputvars[index].id==GRID) ? "" :
                         config->outnames[config->outputvars[index].id].unit,
                         getoutputtype(config->outputvars[index].id),
                         getnyear(config->outputvars[index].id),array,config);
  else
    return create_pft_netcdf(cdf,filename,
                             config->outputvars[index].id,
                             config->npft[GRASS]+config->npft[TREE],
                             config->npft[CROP],
                             config->outnames[config->outputvars[index].id].var,
                             config->outnames[config->outputvars[index].id].descr,
                             config->outnames[config->outputvars[index].id].unit,
                             getoutputtype(config->outputvars[index].id),
                             array,config);
} /* of 'create' */

static void openfile(Outputfile *output,const Cell grid[],
                     const char *filename,int i,
                     const Config *config)
{
  output->files[config->outputvars[i].id].fmt=config->outputvars[i].filename.fmt;
  if(config->outputvars[i].filename.fmt==CDF)
  {
    switch(config->outputvars[i].id)
    {
      case ADISCHARGE: case GRID:
        if(output->index_all==NULL)
        {
          output->index_all=createcoord_all(grid,config);
          if(output->index_all==NULL)
          {
            output->files[config->outputvars[i].id].isopen=FALSE;
            return;
          }
        }
        break;
      default:
        if(output->index==NULL)
        {
          output->index=createcoord(output,grid,config);
          if(output->index==NULL)
          {
            output->files[config->outputvars[i].id].isopen=FALSE;
            return;
          }
        }
        break;
    }
    if(isroot(*config))
    {
      if(!config->outputvars[i].oneyear && 
       !create(&output->files[config->outputvars[i].id].fp.cdf,filename,i,
                (config->outputvars[i].id==ADISCHARGE || config->outputvars[i].id==GRID) ? output->index_all : output->index,config))
      output->files[config->outputvars[i].id].isopen=TRUE;
    }
  }
  else if(isroot(*config) && !config->outputvars[i].oneyear)
    switch(config->outputvars[i].filename.fmt)
    {
      case RAW:
        if((output->files[config->outputvars[i].id].fp.file=fopen(filename,"wb"))==NULL)
          printfopenerr(config->outputvars[i].filename.name);
        else
          output->files[config->outputvars[i].id].isopen=TRUE;
        break;
      case TXT:
        if((output->files[config->outputvars[i].id].fp.file=fopen(filename,"w"))==NULL)
          printfopenerr(config->outputvars[i].filename.name);
        else
          output->files[config->outputvars[i].id].isopen=TRUE;
        break;
    }
  output->files[config->outputvars[i].id].oneyear=config->outputvars[i].oneyear;
} /* of 'openfile' */

Outputfile *fopenoutput(const Cell grid[], /* LPJ grid */
                        int n,         /* size of output file array */
                        const Config *config /* LPJ configuration */
                       )               /* return output file data */
{
  int i;
#ifdef USE_MPI
  int count;
#endif
  Bool isopen;
  char *filename;
  Outputfile *output;
  output=new(Outputfile);
  check(output);
  output->method=config->outputmethod;
  output->files=newvec(File,n);
  check(output->files);
  output->n=n;
  output->index=output->index_all=NULL; 
  for(i=0;i<n;i++)
    output->files[i].isopen=FALSE;
  output->withdaily=FALSE;
#ifdef USE_MPI
  if(output->method!=LPJ_MPI2)
  {
    output->counts=newvec(int,config->ntask);
    check(output->counts);
    output->offsets=newvec(int,config->ntask);
    check(output->offsets);
    count=config->count;
    MPI_Allgather(&count,1,MPI_INT,output->counts,1,MPI_INT,
                  config->comm);
    /* calculate array offsets */
    output->offsets[0]=0;
    for(i=1;i<config->ntask;i++)
      output->offsets[i]=output->offsets[i-1]+output->counts[i-1];
  }
#endif
  if(output->method==LPJ_SOCKET)
  {
    if(isroot(*config))
    {
      output->socket=connect_socket(config->hostname,config->port,0);
      if(output->socket==NULL)
      {
        fputs("ERROR167: Cannot establish connection.\n",stderr);
        isopen=FALSE;
      }
      else
      {
#ifndef _WIN32
        signal(SIGPIPE,handler);
#endif
        isopen=TRUE;
        write_socket(output->socket,"LPJ",3);
        writeint_socket(output->socket,&config->total,1);
      }
    }
#ifdef USE_MPI
    MPI_Bcast(&isopen,1,MPI_INT,0,config->comm);
#endif
    if(!isopen)
    {
#ifdef USE_MPI
      output->counts=output->offsets=NULL;
#endif
      return output; 
    }
  }
  outputnames(output,config);
  for(i=0;i<config->n_out;i++)
  {
    if(hassuffix(config->outputvars[i].filename.name,".gz"))
    { 
      output->files[config->outputvars[i].id].compress=TRUE;
      filename=stripsuffix(config->outputvars[i].filename.name);
      check(filename);
    }
    else
    {
      output->files[config->outputvars[i].id].compress=FALSE;
      filename=config->outputvars[i].filename.name;
    }
    output->files[config->outputvars[i].id].filename=config->outputvars[i].filename.name;
#ifdef USE_MPI
    switch(output->method)
    {
      case LPJ_MPI2:
        if(MPI_File_open(config->comm,filename,
                         MPI_MODE_CREATE|MPI_MODE_WRONLY,MPI_INFO_NULL,
                         &output->files[config->outputvars[i].id].fp.mpi_file))
        {
          if(isroot(*config))
            fprintf(stderr,"ERROR100: Cannot create output file '%s'.\n",
                    filename);
        }
        else
        {
          switch(config->outputvars[i].id)
          {
            case SDATE: case HDATE: case GRID: case COUNTRY: case REGION: case SEASONALITY:
              MPI_File_set_view(output->files[config->outputvars[i].id].fp.mpi_file,
                                0, MPI_SHORT, 
                                MPI_SHORT,"native", MPI_INFO_NULL);
              break;
            default:
              MPI_File_set_view(output->files[config->outputvars[i].id].fp.mpi_file,
                                0, MPI_FLOAT,
                                MPI_FLOAT,"native", MPI_INFO_NULL);
          }
          output->files[config->outputvars[i].id].isopen=TRUE;
        }
        break;
      case LPJ_SOCKET:
        output->files[config->outputvars[i].id].isopen=TRUE;
        break;
      case LPJ_GATHER:
        openfile(output,grid,filename,i,config);
        MPI_Bcast(&output->files[config->outputvars[i].id].isopen,1,MPI_INT,
                  0,config->comm);
        break;
    } /* of 'switch' */
#else
    switch(output->method)
    {
      case LPJ_FILES:
        openfile(output,grid,filename,i,config);
        break;
      case LPJ_SOCKET:
        output->files[config->outputvars[i].id].isopen=TRUE;
        break;
    } /* of 'switch' */
#endif
    if(output->files[config->outputvars[i].id].compress)
      free(filename);
    if(output->files[config->outputvars[i].id].isopen &&
       config->outputvars[i].id>=D_LAI && config->outputvars[i].id<=D_PET)
      output->withdaily=TRUE;
  }
  return output;
} /* of 'fopenoutput' */

void openoutput_yearly(Outputfile *output,int year,const Config *config)
{
  String filename;
  int i,size;
  for(i=0;i<config->n_out;i++)
    if(config->outputvars[i].oneyear)
    {
      if(isroot(*config))
      {
        snprintf(filename,STRING_LEN,config->outputvars[i].filename.name,year);
        switch(config->outputvars[i].filename.fmt)
        {
          case RAW:
            if((output->files[config->outputvars[i].id].fp.file=fopen(filename,"wb"))==NULL)
            {
              printfopenerr(filename);
              output->files[config->outputvars[i].id].isopen=FALSE;
            }
            else
              output->files[config->outputvars[i].id].isopen=TRUE;
            break;
          case TXT:
            if((output->files[config->outputvars[i].id].fp.file=fopen(filename,"w"))==NULL)
            {
              output->files[config->outputvars[i].id].isopen=FALSE;
              printfopenerr(filename);
            }
            else
              output->files[config->outputvars[i].id].isopen=TRUE;
            break;
          case CDF:
            size=outputsize(config->outputvars[i].id,
                            config->npft[GRASS]+config->npft[TREE],
                            config->nbiomass,
                            config->nagtree,
                            config->npft[CROP]);
           if(size==1)
             output->files[config->outputvars[i].id].isopen=!create1_netcdf(&output->files[config->outputvars[i].id].fp.cdf,filename,
                           config->outnames[config->outputvars[i].id].var,
                           config->outnames[config->outputvars[i].id].descr,
                           config->outnames[config->outputvars[i].id].unit,
                           getoutputtype(config->outputvars[i].id),
                           getnyear(config->outputvars[i].id),
                           (config->outputvars[i].id==ADISCHARGE) ? output->index_all : output->index,year,config);
           else
             output->files[config->outputvars[i].id].isopen=!create1_pft_netcdf(&output->files[config->outputvars[i].id].fp.cdf,filename,
                           config->outputvars[i].id,
                           config->npft[GRASS]+config->npft[TREE],
                           config->npft[CROP],
                           config->outnames[config->outputvars[i].id].var,
                           config->outnames[config->outputvars[i].id].descr,
                           config->outnames[config->outputvars[i].id].unit,
                           getoutputtype(config->outputvars[i].id),
                               output->index,config);

        }
     }
#ifdef USE_MPI
     MPI_Bcast(&output->files[config->outputvars[i].id].isopen,1,MPI_INT,0,config->comm);
#endif
     if(output->files[config->outputvars[i].id].isopen &&
       config->outputvars[i].id>=D_LAI && config->outputvars[i].id<=D_PET)
      output->withdaily=TRUE;
   }
} /* of 'openoutput_yearly */
