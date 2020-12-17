/***************************************************************************/
/**                                                                       **/
/**             f  r  e  a  d  c  e  l  l  .  c                           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads cell data from file                                **/
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

#define checkptr(ptr) if(ptr==NULL) { printallocerr(#ptr); return TRUE;}

Bool freadcell(FILE *file,             /* File pointer to binary file */
        Cell *cell,             /* Pointer to cell */
        const Pftpar pftpar[],  /* PFT parameter array */
        int npft,               /* number of natural PFTs */
        int nagtree,            /* number of agriculture tree PFTs */
        int ncft,               /* number of crop PFTs */
        const Soilpar *soilpar, /* pointer to soil parameter */
        const Standtype standtype[], /* array of stand types */
        int nstand,             /* number of stand types */
        Bool landuse_restart,   /* land use enabled in restart file (TRUE/FALSE) */
        int sdate_option,       /* sowing date option (0-2) */
        int sdate_option_restart, /* sowing date option in restart file */
        Bool river_routing,     /* river routing (TRUE/FALSE) */
        Bool swap /* Byte order has to be changed (TRUE/FALSE) */
        )                        /* returns TRUE on error */
{
    Byte b;
    int i;

    if(fread(&b,sizeof(b),1,file)!=1)
        return TRUE;
    cell->skip=b;
    if(river_routing)
    {
        nit_read_restarts_water(file, cell, swap);
        freadreal1(&cell->discharge.dmass_lake,swap,file);
        freadreal1(&cell->discharge.dfout,swap,file);
        freadreal1(&cell->discharge.dmass_river,swap,file);
        freadreal1(&cell->discharge.dmass_sum,swap,file);
        cell->discharge.queue=freadqueue(file,swap);
        if(cell->discharge.queue==NULL)
            return TRUE;
        if(fread(&b,sizeof(b),1,file)!=1)
            return TRUE;
        cell->ml.dam=b;
        if(cell->ml.dam)
        {
            if(freadresdata(file,cell,swap))
                return TRUE;
        }
    } 
    if(!cell->skip)
    {
        /* cell has valid soilcode */
        freadreal1(&cell->discharge.waterdeficit,swap,file);
        cell->gdd=newgdd(npft);
        checkptr(cell->gdd);
        initgdd(cell->gdd,npft);
        /* read stand list */
        cell->standlist=freadstandlist(file,cell,pftpar,npft+ncft,soilpar,
                standtype,nstand,swap);
        if(cell->standlist==NULL)
            return TRUE;
        freadreal1(&cell->ml.cropfrac_rf,swap,file);
        freadreal1(&cell->ml.cropfrac_ir,swap,file);
        freadclimbuf(file,&cell->climbuf,swap);
        cell->ml.cropdates=freadcropdates(file,ncft,swap);
        if(cell->ml.cropdates==NULL)
            return TRUE;
        if(sdate_option>NO_FIXED_SDATE)
        {
            cell->ml.sdate_fixed=newvec(int,2*ncft);
            checkptr(cell->ml.sdate_fixed);
            if(sdate_option_restart>NO_FIXED_SDATE)
                freadint(cell->ml.sdate_fixed,2*ncft,swap,file);
            else
                for(i=0;i<2*ncft;i++)
                    cell->ml.sdate_fixed[i]=0;
        }
        else
            cell->ml.sdate_fixed=NULL;
        cell->ml.sowing_month=newvec(int,2*ncft);
        checkptr(cell->ml.sowing_month);
        freadint(cell->ml.sowing_month,2*ncft,swap,file);
        cell->ml.gs=newvec(int,2*ncft);
        checkptr(cell->ml.gs);
        if(freadint(cell->ml.gs,2*ncft,swap,file)!=2*ncft)
            return TRUE;
        if(cell->ml.landfrac!=NULL && landuse_restart){
            if(freadlandfrac(file,cell->ml.landfrac,ncft,nagtree,swap))
                return TRUE;
        }

        // barrier.n: reads the sowing date, also alliocates the array
        cell->ml.sowing_date = newvec(int,2*ncft);
        freadint(cell->ml.sowing_date,2*ncft,swap,file);
        cell->ml.application = newvec(Application, 2*(ncft+nagtree));
    }

    return FALSE;
} /* of 'freadcell' */
