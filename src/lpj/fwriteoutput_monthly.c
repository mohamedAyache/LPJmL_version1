/***************************************************************************/
/**                                                                       **/
/**   f  w  r  i  t  e  o  u  t  p  u  t  _  m  o  n  t  h  l  y  .  c    **/
///**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-06-19 15:14:31 +0200 (ven., 19 juin 2#$ **/
/**     By         : $Author:: jonasjae                        $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

static void writemonth(Outputfile *output,int index,float *data,int year,
        int month,
        const Config *config)
{
    int i;
#ifdef USE_MPI
    MPI_Status status;
#endif
    for(i=0;i<config->count;i++)
        data[i]*=config->outnames[index].scale;
#ifdef USE_MPI
    switch(output->method)
    {
        case LPJ_MPI2:
            MPI_File_write_at(output->files[index].fp.mpi_file,
                    ((year-config->firstyear)*NMONTH+month)*config->total+config->offset,
                    data,config->count,MPI_FLOAT,&status);
            break;
        case LPJ_GATHER:
            switch(output->files[index].fmt)
            {
                case RAW:
                    mpi_write(output->files[index].fp.file,data,MPI_FLOAT,config->total,
                            output->counts,output->offsets,config->rank,config->comm);
                    break;
                case TXT:
                    mpi_write_txt(output->files[index].fp.file,data,MPI_FLOAT,config->total,
                            output->counts,output->offsets,config->rank,config->comm);
                    break;
                case CDF:
                    mpi_write_netcdf(&output->files[index].fp.cdf,data,MPI_FLOAT,
                            config->total,
                            output->files[index].oneyear ? month : (year-config->firstyear)*12+month,
                            output->counts,output->offsets,config->rank,
                            config->comm);
                    break;
            }
            break;
        case LPJ_SOCKET:
            if(isroot(*config))
                writeint_socket(output->socket,&index,1);
            mpi_write_socket(output->socket,data,MPI_FLOAT,config->total,
                    output->counts,output->offsets,config->rank,config->comm);
            break;
    } /* of switch */
#else
    if(output->method==LPJ_SOCKET)
    {
        writeint_socket(output->socket,&index,1);
        writefloat_socket(output->socket,data,config->count);
    }
    else
        switch(output->files[index].fmt)
        {
            case RAW:
                if(fwrite(data,sizeof(float),config->count,output->files[index].fp.file)!=config->count)
                    fprintf(stderr,"ERROR204: Error writing output: %s.\n",strerror(errno));
                break;
            case TXT:
                for(i=0;i<config->count-1;i++)
                    fprintf(output->files[index].fp.file,"%g ",data[i]);
                fprintf(output->files[index].fp.file,"%g\n",data[config->count-1]);
                break;
            case CDF:
                write_float_netcdf(&output->files[index].fp.cdf,data,
                        output->files[index].oneyear ? month : (year-config->firstyear)*12+month,
                        config->count);
                break;
        }
#endif
} /* of 'writemonth' */

void fwriteoutput_monthly(Outputfile *output,
        const Cell grid[],
        int month,
        int year,
        const Config *config
        )
{

    int count,cell;
    float *fvec;
    fvec=newvec(float,config->count);
    check(fvec);
    if(output->files[MNPP].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mnpp;
        writemonth(output,MNPP,fvec,year,month,config);
    }
    if(output->files[MGPP].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mgpp;
        writemonth(output,MGPP,fvec,year,month,config);
    }
    if(output->files[MRH].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mrh;
        writemonth(output,MRH,fvec,year,month,config);
    }
    if(output->files[MFPAR].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mfpar;
        writemonth(output,MFPAR,fvec,year,month,config);
    }
    if(output->files[MRUNOFF].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mrunoff;
        writemonth(output,MRUNOFF,fvec,year,month,config);
    }
    if(output->files[MDISCHARGE].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mdischarge;
        writemonth(output,MDISCHARGE,fvec,year,month,config);
    }
    if(output->files[MWATERAMOUNT].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mwateramount;
        writemonth(output,MWATERAMOUNT,fvec,year,month,config);
    }
    if(output->files[MTRANSP].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mtransp;
        writemonth(output,MTRANSP,fvec,year,month,config);
    }
    if(output->files[MTRANSP_B].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mtransp_b;
        writemonth(output,MTRANSP_B,fvec,year,month,config);
    }
    if(output->files[MEVAP].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mevap;
        writemonth(output,MEVAP,fvec,year,month,config);
    }
    if(output->files[MEVAP_B].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mevap_b;
        writemonth(output,MEVAP_B,fvec,year,month,config);
    }
    if(output->files[MINTERC].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.minterc;
        writemonth(output,MINTERC,fvec,year,month,config);
    }
    if(output->files[MINTERC_B].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.minterc_b;
        writemonth(output,MINTERC_B,fvec,year,month,config);
    }
    if(output->files[MPET].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mpet;
        writemonth(output,MPET,fvec,year,month,config);
    }
    if(output->files[MSWC1].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mswc[0];
        writemonth(output,MSWC1,fvec,year,month,config);
    }
    if(output->files[MSWC2].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mswc[1];
        writemonth(output,MSWC2,fvec,year,month,config);
    }
    if(output->files[MSWC3].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mswc[2];
        writemonth(output,MSWC3,fvec,year,month,config);
    }
    if(output->files[MSWC4].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mswc[3];
        writemonth(output,MSWC4,fvec,year,month,config);
    }
    if(output->files[MSWC5].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mswc[4];
        writemonth(output,MSWC5,fvec,year,month,config);
    }
    if(output->files[MROOTMOIST].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mrootmoist;
        writemonth(output,MROOTMOIST,fvec,year,month,config);
    }
    if(output->files[MIRRIG].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mirrig;
        writemonth(output,MIRRIG,fvec,year,month,config);
    }
    if(output->files[MRETURN_FLOW_B].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mreturn_flow_b;
        writemonth(output,MRETURN_FLOW_B,fvec,year,month,config);
    }
    if(output->files[MEVAP_LAKE].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mevap_lake;
        writemonth(output,MEVAP_LAKE,fvec,year,month,config);
    }
    if(output->files[MEVAP_RES].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mevap_res;
        writemonth(output,MEVAP_RES,fvec,year,month,config);
    }
    if(output->files[MPREC_RES].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mprec_res;
        writemonth(output,MPREC_RES,fvec,year,month,config);
    }
    if(output->files[MFIREC].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mfirec;
        writemonth(output,MFIREC,fvec,year,month,config);
    }
    if(output->files[MNFIRE].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mnfire;
        writemonth(output,MNFIRE,fvec,year,month,config);
    }
    if(output->files[MFIREDI].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mfiredi;
        writemonth(output,MFIREDI,fvec,year,month,config);
    }
    if(output->files[MFIREEMISSION].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mfireemission;
        writemonth(output,MFIREEMISSION,fvec,year,month,config);
    }
    if (output->files[MBURNTAREA].isopen)
    {
        count = 0;
        for (cell = 0; cell < config->ngridcell; cell++)
            if (!grid[cell].skip)
                fvec[count++] = (float) grid[cell].output.mburntarea;
        writemonth(output, MBURNTAREA, fvec, year, month, config);
    }
    if (output->files[MTEMP_IMAGE].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mtemp_image;
        writemonth(output,MTEMP_IMAGE,fvec,year,month,config);
    }
    if(output->files[MPREC_IMAGE].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mprec_image;
        writemonth(output,MPREC_IMAGE,fvec,year,month,config);
    }
    if(output->files[MSUN_IMAGE].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.msun_image;
        writemonth(output,MSUN_IMAGE,fvec,year,month,config);
    }
    if(output->files[MWET_IMAGE].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mwet_image;
        writemonth(output,MWET_IMAGE,fvec,year,month,config);
    }
    if(output->files[MSOILTEMP1].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.msoiltemp1;
        writemonth(output,MSOILTEMP1,fvec,year,month,config);
    }
    if(output->files[MSOILTEMP2].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.msoiltemp2;
        writemonth(output,MSOILTEMP2,fvec,year,month,config);
    }
    if(output->files[MSOILTEMP3].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.msoiltemp3;
        writemonth(output,MSOILTEMP3,fvec,year,month,config);
    }
    if(output->files[MSOILTEMP4].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.msoiltemp4;
        writemonth(output,MSOILTEMP4,fvec,year,month,config);
    }
    if(output->files[MSOILTEMP5].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.msoiltemp5;
        writemonth(output,MSOILTEMP5,fvec,year,month,config);
    }
    if(output->files[MSOILTEMP6].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.msoiltemp6;
        writemonth(output,MSOILTEMP6,fvec,year,month,config);
    }
    if(output->files[MRES_STORAGE].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mres_storage;
        writemonth(output,MRES_STORAGE,fvec,year,month,config);
    }
    if(output->files[MRES_DEMAND].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mres_demand;
        writemonth(output,MRES_DEMAND,fvec,year,month,config);
    }
    if(output->files[MTARGET_RELEASE].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mtarget_release;
        writemonth(output,MTARGET_RELEASE,fvec,year,month,config);
    }
    if(output->files[MRES_CAP].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mres_cap;
        writemonth(output,MRES_CAP,fvec,year,month,config);
    }
    if(output->files[MWD_UNSUST].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mwd_unsustainable;
        writemonth(output,MWD_UNSUST,fvec,year,month,config);
    }
    if(output->files[MUNMET_DEMAND].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.munmet_demand;
        writemonth(output,MUNMET_DEMAND,fvec,year,month,config);
    }
    if(output->files[MWD_LOCAL].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mwd_local;
        writemonth(output,MWD_LOCAL,fvec,year,month,config);
    }
    if(output->files[MWD_NEIGHB].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mwd_neighb;
        writemonth(output,MWD_NEIGHB,fvec,year,month,config);
    }
    if(output->files[MWD_RES].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mwd_res;
        writemonth(output,MWD_RES,fvec,year,month,config);
    }
    if(output->files[MWD_RETURN].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mwd_return;
        writemonth(output,MWD_RETURN,fvec,year,month,config);
    }
    if(output->files[MCONV_LOSS_EVAP].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mconv_loss_evap;
        writemonth(output,MCONV_LOSS_EVAP,fvec,year,month,config);
    }
    if(output->files[MCONV_LOSS_DRAIN].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mconv_loss_drain;
        writemonth(output,MCONV_LOSS_DRAIN,fvec,year,month,config);
    }
    if(output->files[MSTOR_RETURN].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mstor_return;
        writemonth(output,MSTOR_RETURN,fvec,year,month,config);
    }
    if(output->files[MPREC].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mprec;
        writemonth(output,MPREC,fvec,year,month,config);
    }
    if(output->files[MRAIN].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mrain;
        writemonth(output,MRAIN,fvec,year,month,config);
    }
    if(output->files[MSNOWF].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.msnowf;
        writemonth(output,MSNOWF,fvec,year,month,config);
    }
    if(output->files[MMELT].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mmelt;
        writemonth(output,MMELT,fvec,year,month,config);
    }
    if(output->files[MSWE].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mswe;
        writemonth(output,MSWE,fvec,year,month,config);
    }
    if(output->files[MSNOWRUNOFF].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.msnowrunoff;
        writemonth(output,MSNOWRUNOFF,fvec,year,month,config);
    }
    if(output->files[MRUNOFF_SURF].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mrunoff_surf;
        writemonth(output,MRUNOFF_SURF,fvec,year,month,config);
    }
    if(output->files[MRUNOFF_LAT].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mrunoff_lat;
        writemonth(output,MRUNOFF_LAT,fvec,year,month,config);
    }
    if(output->files[MSEEPAGE].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mseepage;
        writemonth(output,MSEEPAGE,fvec,year,month,config);
    }
    if(output->files[MGCONS_RF].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mgcons_rf;
        writemonth(output,MGCONS_RF,fvec,year,month,config);
    }
    if(output->files[MGCONS_IRR].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mgcons_irr;
        writemonth(output,MGCONS_IRR,fvec,year,month,config);
    }
    if(output->files[MBCONS_IRR].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mbcons_irr;
        writemonth(output,MBCONS_IRR,fvec,year,month,config);
    }
    if(output->files[MIRRIG_RW].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip)
                fvec[count++]=(float)grid[cell].output.mirrig_rw;
        writemonth(output,MIRRIG_RW,fvec,year,month,config);
    }

    /* barrier.n: saving of nut related outputs */
    if(output->files[MNO3_DIS].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mNO3discharge;

            }

        writemonth(output,MNO3_DIS,fvec,year,month,config);
    }
    /* barrier.n: saving of nut related outputs */
    if(output->files[MPO4_DIS].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mPO4discharge;
            }
        writemonth(output,MPO4_DIS,fvec,year,month,config);
    }
    if(output->files[MPO4ADS_DIS].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mPO4adsdischarge;
            }
        writemonth(output,MPO4ADS_DIS,fvec,year,month,config);
    }
    /* barrier.n: saving of nut related outputs */
    if(output->files[MNH4_DIS].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mNH4discharge;
            }
        writemonth(output,MNH4_DIS,fvec,year,month,config);
    }
    if(output->files[MPHY_DIS].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mPhytoCdischarge;
            }
        writemonth(output,MPHY_DIS,fvec,year,month,config);
    }

    /* saving soil related monthly outputs (surface layer) */
    if(output->files[MRESC_SOIL].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mRESC_SOIL[0];
            }
        writemonth(output,MRESC_SOIL,fvec,year,month,config);
    }
    if(output->files[MRESN_SOIL].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mRESN_SOIL[0];
            }
        writemonth(output,MRESN_SOIL,fvec,year,month,config);
    }
    if(output->files[MRESP_SOIL].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mRESP_SOIL[0];
            }
        writemonth(output,MRESP_SOIL,fvec,year,month,config);
    }
    if(output->files[MDECC_SOIL].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mDECC_SOIL[0];
            }
        writemonth(output,MDECC_SOIL,fvec,year,month,config);
    }
    if(output->files[MNO3_SOIL_SUR].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mNO3_SOIL[0];
            }

        writemonth(output,MNO3_SOIL_SUR,fvec,year,month,config);
    }
    if(output->files[MNO3_SOIL_DEP].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mNO3_SOIL[1];
            }

        writemonth(output,MNO3_SOIL_DEP,fvec,year,month,config);
    }
    if(output->files[MNH4_SOIL].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mNH4_SOIL[0];
            }
        writemonth(output,MNH4_SOIL,fvec,year,month,config);
    }
    if(output->files[MPO4_SOIL_SUR].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mPO4_SOIL[0];
            }
        writemonth(output,MPO4_SOIL_SUR,fvec,year,month,config);
    }
    if(output->files[MPO4_SOIL_DEP].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mPO4_SOIL[1];
            }
        writemonth(output,MPO4_SOIL_DEP,fvec,year,month,config);
    }
    if(output->files[MNO3_LAKE].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mNO3_LAKE;
            }
        writemonth(output,MNO3_LAKE,fvec,year,month,config);
    }

    if(output->files[MNH4_LAKE].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mNH4_LAKE;
            }
        writemonth(output,MNH4_LAKE,fvec,year,month,config);
    }

    if(output->files[MPO4_LAKE].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mPO4_LAKE;
            }
        writemonth(output,MPO4_LAKE,fvec,year,month,config);
    }

    if(output->files[MPHY_LAKE].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mPHY_LAKE;
            }
        writemonth(output,MPHY_LAKE,fvec,year,month,config);
    }

    if(output->files[MUPTRESDIFF].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mUPTRESDIFF;
            }
        writemonth(output,MUPTRESDIFF,fvec,year,month,config);
    }
    if(output->files[MNWASTEWATER].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mNWastewater;
            }
        writemonth(output,MNWASTEWATER,fvec,year,month,config);
    }
    if(output->files[MPO4WASTEWATER].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mPO4Wastewater;
            }
        writemonth(output,MPO4WASTEWATER,fvec,year,month,config);
    }
        if(output->files[MNFERTI].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mNferti;
            }
        writemonth(output,MNFERTI,fvec,year,month,config);
    }
    if(output->files[MPO4FERTI].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mPO4ferti;
            }
        writemonth(output,MPO4FERTI,fvec,year,month,config);
    }

        if(output->files[MNMANURE].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mNmanure;
            }
        writemonth(output,MNMANURE,fvec,year,month,config);
    }
    if(output->files[MPO4MANURE].isopen)
    {
        count=0;
        for(cell=0;cell<config->ngridcell;cell++)
            if(!grid[cell].skip){
                fvec[count++]=(float)grid[cell].output.mPO4manure;
            }
        writemonth(output,MPO4MANURE,fvec,year,month,config);
    }


    free(fvec);

} /* of 'fwriteoutput_monthly' */
