#include "lpj.h"

/* Reads nitrogen files. Copy of the "readclimate" file. However, here
 * we read integers instead of ints.
 *
 * @param Climatefile file: climate data file
 * @param Real data: array of climate data that will be read
 * @param Real offset: offset for data
 * @param Real scale: scale factor for data
 * @param Cell grid: array of grid cells (used only if netcdf)
 * @param int year: current year
 * @param Config config: constant pointer to the configuration obhect
 *
 * \author Nicolas Barrier
 * */
Bool nit_readfiles(Climatefile *file,   /* climate data file */
        Real data[],         /* climate data read */
        Real offset,         /* offset for data */
        Real scale,          /* scale factor for data*/
        const Cell grid[],   /* LPJ grid */
        int year,            /* year (AD) */
        const Config *config /* LPJ configuration */
        )                     /* return TRUE on error */
{
    int *vec;
    int i,rc;
    long index;

    if(file->fmt==FMS){
        return FALSE;
    }
    index = year-file->firstyear;

    if(index<0 || index>=file->nyear)
    {
        if(isroot(*config))
            fprintf(stderr,"ERROR130: Invalid year %d in nit_readfiles().\n",year);
        return TRUE;
    }
    if(file->fmt==CDF)
        rc=readclimate_netcdf(file,data,grid,index,config);
    else
    {
        if(fseek(file->file,index*file->size+file->offset,SEEK_SET))
            rc=TRUE;
        else
        {
            vec=newvec(int,file->n);
            if(vec==NULL)
            {
                printallocerr("vec");
                rc=TRUE;
            }
            else
            {

                if(fread(vec,sizeof(int),file->n,file->file)!=file->n){
                    rc=TRUE;
                }
                else
                {
                    if(file->swap)
                        for(i=0;i<file->n;i++){
                            data[i]=offset+swapint(vec[i])*scale;
                        }
                    else
                        for(i=0;i<file->n;i++){
                            //printf("i=%d, vec=%d, offset=%f\n", i, vec[i], offset);
                            data[i]=offset+vec[i]*scale;
                            //printf("i=%d, data=%f\n", i, data[i]);
                            //printf("========================\n");
                        }
                    rc=FALSE;
                }
                free(vec);
            }
        }
    }
    //printf("rc = %d", rc);
    return iserror(rc,config);
} /* of'readclimate' */
