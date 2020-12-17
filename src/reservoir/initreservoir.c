/***************************************************************************/
/**                                                                       **/
/**                 i  n  i  t  r  e  s  e  r  v  o  i  r  .  c           **/
/**                                                                       **/
/**     Function initializes irrigation network for dam reservoirs        **/
/**                                                                       **/
/**     written by Hester Biemans and Werner von Bloh                     **/
/**     Wageningen University and Research Centre                         **/
/**     and                                                               **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date::                                            $ **/
/**     By         : $Author::                                 $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

typedef struct
{
  int elevation;
  int next;
} Item;

static Bool initreservoir2(Cell grid[],Config *config)
{
  int cell;
  short elevation;
  Real data;
  Bool swap;
  Header header;
  String headername;
  int version;
  FILE *file;
  Input_netcdf input;
  Reservoir reservoir;
  if(config->elevation_filename.fmt==CDF)
  {
    input=openinput_netcdf(config->elevation_filename.name,
                           config->elevation_filename.var,"m",0,config);
    if(input==NULL)
      return TRUE;
    for(cell=0;cell<config->ngridcell;cell++)
    {
       if(readinput_netcdf(input,&data,&grid[cell].coord,&config->resolution))
       {
         closeinput_netcdf(input);
         return TRUE;
       }
       grid[cell].elevation=(int)data;
    }
    closeinput_netcdf(input);
  }
  else
  {
    if((file=openinputfile(&header,&swap,&config->elevation_filename,
                           headername,
                           &version,config))==NULL)
      return TRUE;
    if(fseek(file,sizeof(elevation)*config->startgrid,SEEK_CUR))
    {
      fclose(file);
      return TRUE;
    }
    for(cell=0;cell<config->ngridcell;cell++)
    {
      if(fread(&elevation,sizeof(elevation),1,file)!=1)
      {
        fclose(file);
        return TRUE;
      }
      if(swap)
        grid[cell].elevation=(int)(swapshort(elevation)*header.scalar);
      else
        grid[cell].elevation=(int)(elevation*header.scalar);
    }
    fclose(file);
  }
  if((file=openinputfile(&header,&swap,&config->reservoir_filename,
                         headername,
                         &version,config))==NULL)
    return TRUE;
  if(fseek(file,sizeof(Reservoir)*config->startgrid,SEEK_CUR))
  {
    fclose(file);
    return TRUE;
  }
  for(cell=0;cell<config->ngridcell;cell++)
  {
    if(readreservoir(&reservoir,swap,file))
    {
      fclose(file);
      return TRUE;
    }
    /*if(reservoir.year>0)
      reservoir.year=1; */

    /*if constant landuse 2000, all dams as in 2000 (but 'built' before)*/
    if(config->withlanduse==CONST_LANDUSE)
    {
      if(reservoir.year>0 && reservoir.year<=2000)
        reservoir.year=1;
    }

    if(reservoir.year>0 && reservoir.year<=config->lastyear) /* Is there a dam? */
    {
      if(!grid[cell].ml.dam)
      {
        grid[cell].ml.resdata=new(Resdata);
        if(grid[cell].ml.resdata==NULL)
        {
          printallocerr("resdata");
          fclose(file);
          return TRUE;
        }
      }
      grid[cell].ml.resdata->reservoir=reservoir;

    }
    else /* no */
      grid[cell].ml.resdata=NULL;
  }
  fclose(file);
  return FALSE;
} /* of 'initreservoir2' */

Bool initreservoir(Cell grid[],   /* LPJ grid */
                   Config *config /* LPJ configuration */
                  )               /* returns TRUE on error */
{
  Bool iserr,*visit;
  int cell,i,j,k,count,index;
  Intlist *back,list,new;
  Item *recv;
#ifdef USE_MPI
  int *counts;
  int *offsets;
  Item *send;
#endif
  iserr=initreservoir2(grid,config);
  if(iserror(iserr,config))
    return TRUE;
#ifdef USE_MPI
  send=newvec(Item,config->ngridcell);
  check(send);
  counts=newvec(int,config->ntask);
  check(counts);
  offsets=newvec(int,config->ntask);
  check(offsets);
  getcounts(counts,offsets,config->nall,2,config->ntask);
#endif
  recv=newvec(Item,config->nall);
  check(recv);
  for(cell=0;cell<config->ngridcell;cell++)
  {
#ifdef USE_MPI
    send[cell].next=grid[cell].discharge.next;
    send[cell].elevation=grid[cell].elevation;
#else
    recv[cell].next=grid[cell].discharge.next;
    recv[cell].elevation=grid[cell].elevation;
#endif
  }
#ifdef USE_MPI
  MPI_Allgatherv(send,counts[config->rank],MPI_INT,recv,counts,
                 offsets,MPI_INT,config->comm);
#endif
  back=newvec(Intlist,config->nall);
  check(back);
  for(cell=0;cell<config->nall;cell++)
    initintlist(back+cell);
  for(cell=0;cell<config->nall;cell++)
    if(recv[cell].next>=0)
      addintlistitem(back+recv[cell].next-config->firstgrid,cell);
  initintlist(&list);
#ifdef USE_MPI
  config->irrig_res=pnet_init(config->comm,
                                   (sizeof(Real)==sizeof(double)) ? MPI_DOUBLE : MPI_FLOAT,
                                   config->nall);
#else
  config->irrig_res=pnet_init(sizeof(Real),config->nall);
#endif
  visit=newvec(Bool,config->nall);
  check(visit);
  for(cell=0;cell<config->ngridcell;cell++)
    if(grid[cell].ml.resdata!=NULL)
    {
      for(i=0;i<config->nall;i++)
        visit[i]=FALSE;
      index=cell+config->startgrid-config->firstgrid;
      addintlistitem(&list,index);
      visit[index]=TRUE;
      k=0;
      while(recv[index].next>=0 )/*&& k<20) */
      {
        index=recv[index].next-config->firstgrid;
        if(visit[index])
        {
          fprintf(stderr,
                  "WARNING017: Loop detected in river routing at cell=%d\n",
                  index);
          break;
        }
        visit[index]=TRUE;
        addintlistitem(&list,index);
        k++;
      }
      for(count=0;count<5;count++) /*Amount of cells upstream in reach of reservoir */
      {
        initintlist(&new);
        for(i=0;i<list.n;i++)
        {
          pnet_addconnect(config->irrig_res,
                          cell+config->startgrid-config->firstgrid,
                          list.index[i]);
          if(count<4)
            for(j=0;j<back[list.index[i]].n;j++)
            {
              index=back[list.index[i]].index[j];
              if(!visit[index] &&
                 recv[index].elevation<recv[cell+config->startgrid-config->firstgrid].elevation)
              {
                visit[index]=TRUE;
                addintlistitem(&new,index);
              }
            }
        } /* of for(i=0;...) */
        emptyintlist(&list);
        list=new;
      }
      if(pnet_inlen(config->irrig_res,cell+config->startgrid-config->firstgrid))
      {
        grid[cell].ml.resdata->fraction=newvec(Real,pnet_inlen(config->irrig_res,
                                          cell+config->startgrid-config->firstgrid));
        check(grid[cell].ml.resdata->fraction);
      }
      else
        grid[cell].ml.resdata->fraction=NULL;
    }
  config->irrig_res_back=pnet_dup(config->irrig_res);
  pnet_reverse(config->irrig_res_back);
  pnet_setup(config->irrig_res);
  pnet_setup(config->irrig_res_back);
  for(cell=0;cell<config->ngridcell;cell++)
    if(pnet_inlen(config->irrig_res_back,cell+config->startgrid-config->firstgrid))
    {
      grid[cell].ml.fraction=newvec(Real,pnet_inlen(config->irrig_res_back,
                                 cell+config->startgrid-config->firstgrid));
      check(grid[cell].ml.fraction);
    }
    else
      grid[cell].ml.fraction=NULL;
  for(cell=0;cell<config->nall;cell++)
    emptyintlist(back+cell);
  free(back);
  free(recv);
  free(visit);
#ifdef USE_MPI
  free(send);
  free(counts);
  free(offsets);
#endif
  return FALSE;
} /* of 'initreservoir' */
