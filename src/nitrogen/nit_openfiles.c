
/* Reads a nitrogen binary file. This file is a copy of the openclimate.c file.
 * The only difference is that opening files with any number of bands is allowed,
 * while climate files musthave either 12 or 365 bands.
 *
 * In these nitrogen files, nbands=1
 *
 * @param Climatefile file: pointer to a climate file object
 * @param Filename filename: constant pointer to a filename object
 * @param char units: constant pointer to a string
 * @param size_t dsize: data size within the binary file
 * @param Config config: constant pointer to the configuration object
 *
 * \author Nicolas Barrier
 */

#include "lpj.h"

Bool nit_openfiles(Climatefile *file,   /* pointer to climate file */
        const Filename *filename,   /* file name and format */
        const char *units,   /* units in NetCDF file or NULL */
        size_t dsize,        /* size of data item in binary file */
        const Config *config /* LPJ configuration */
        )                     /* returns TRUE on error */
{
    Header header;
    String headername;
    int last,version;
    char *s;
    file->fmt=filename->fmt;

    if(filename->fmt==FMS)
    {
        file->isdaily=TRUE;
        file->firstyear=config->firstyear;
        return FALSE;
    }
    file->oneyear=FALSE;
    if(filename->fmt==CDF) /* file is in NetCDF format? */
    {
        s=strstr(filename->name,"[");
        if(s!=NULL && sscanf(s,"[%d-%d]",&file->firstyear,&last)==2)
        {
            /* one file for each simulation year */
            file->filename=mkfilename(filename->name);
            if(isroot(*config))
            {
                s=malloc(strlen(file->filename)+12);
                sprintf(s,file->filename,file->firstyear);
                openclimate_netcdf(file,s,filename->var,units,config);
                free(s);
            }
#ifdef USE_MPI
            MPI_Bcast(&file->isdaily,1,MPI_INT,0,config->comm);
#endif
            if(isroot(*config))
                free_netcdf(file->ncid);
            file->oneyear=TRUE;
            file->units=units;
            file->nyear=last-file->firstyear+1;
            /* file->isdaily=TRUE; */
            if(file->filename==NULL)
                return TRUE;
            file->n=(file->isdaily) ? NDAYYEAR*config->ngridcell : NMONTH*config->ngridcell;
            file->var=filename->var;
            return FALSE;
        }
        else
            return mpi_openclimate_netcdf(file,filename->name,filename->var,units,config);
    }
    if((file->file=openinputfile(&header,&file->swap,
                    filename,
                    headername,
                    &version,config))==NULL)
        return TRUE;
    if (header.order==YEARCELL)
    {
        if(isroot(*config))
            fprintf(stderr,"ERROR127: Order YEARCELL in '%s' is not supported in this LPJ-Version.\n"
                    "Please reorganize your input data!\n",filename->name);
        fclose(file->file);
        return TRUE;
    }

    /* barrier.n: no more consideration on the number of bands in the file
       if(filename->fmt!=RAW && header.nbands!=NMONTH && header.nbands!=NDAYYEAR)
       {
       if(config->rank==0)
       fprintf(stderr,"ERROR124: Invalid number of bands %d in '%s'.\n",
       header.nbands,filename->name);
       fclose(file->file);
       return TRUE;
       }
       */

    file->version=version;
    file->firstyear=header.firstyear;
    file->scalar=header.scalar;
    file->nyear=header.nyear;

    if(filename->fmt==RAW)
    {
        header.nbands=12;
        file->offset=config->startgrid*header.nbands*dsize;
    }
    else
        file->offset=(config->startgrid-header.firstcell)*header.nbands*
            dsize+headersize(headername,version);
    file->isdaily=(header.nbands==NDAYYEAR);
    file->size=header.ncell*header.nbands*dsize;
    file->n=header.nbands*header.ncell/*config->ngridcell*/;

    return FALSE;
} /* of 'nit_openfiles' */
