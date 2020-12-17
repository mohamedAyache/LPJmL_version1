/***************************************************************************/
/**                                                                       **/
/**             f  w  r  i  t  e  c  e  l  l  .  c                        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function writes cell array and calculates index vector            **/
/**     Index vector is used at reading to seek to specific LPJ cells     **/
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

int fwritecell(FILE *file,        /* File pointer of binary file */
        long long index[], /* index vector to be calculated */
        const Cell grid[], /* cell data array */
        int ncell,         /* number of cells */
        int npft,          /* number of PFTs */
        int nagtree,       /* number of agriculture tree PFTs */
        int ncft,          /* number of crop PFTs */
        int sdate_option,  /* sowing date option (0-2) */
        Bool river_routing /* river routing (TRUE/FALSE) */
        )                   /* returns number of cells written */
{
    int cell;
    Byte b;

    for(cell=0;cell<ncell;cell++)
    {
        if(index!=NULL)
            index[cell]=ftell(file); /* store actual position in index vector */
        b=(Byte)grid[cell].skip;
        fwrite(&b,sizeof(b),1,file);
        if(river_routing)
        {
            nit_write_restarts_water(file, grid[cell]);
            if(fwrite(&grid[cell].discharge.dmass_lake,sizeof(Real),1,file)!=1)
                break;
            if(fwrite(&grid[cell].discharge.dfout,sizeof(Real),1,file)!=1)
                break;
            if(fwrite(&grid[cell].discharge.dmass_river,sizeof(Real),1,file)!=1)
                break;
            if(fwrite(&grid[cell].discharge.dmass_sum,sizeof(Real),1,file)!=1)
                break;
            if(fwritequeue(file,grid[cell].discharge.queue)){
                break;
            }
            b=(Byte)grid[cell].ml.dam;
            fwrite(&b,sizeof(b),1,file);
            if(grid[cell].ml.dam)
            {
                if(fwriteresdata(file,grid+cell))
                    break;
            }
        }
        if(!grid[cell].skip)
        {
            if(fwrite(&grid[cell].discharge.waterdeficit,sizeof(Real),1,file)!=1)
                break;
            if(fwritestandlist(file,grid[cell].standlist,npft+ncft)!=
                    grid[cell].standlist->n)
                break;
            if(fwrite(&grid[cell].ml.cropfrac_rf,sizeof(Real),1,file)!=1)
                break;
            if(fwrite(&grid[cell].ml.cropfrac_ir,sizeof(Real),1,file)!=1)
                break;
            if(fwriteclimbuf(file,&grid[cell].climbuf))
                break;
            if(fwritecropdates(file,grid[cell].ml.cropdates,ncft))
                break;
            if(sdate_option>NO_FIXED_SDATE)
            {
                if(fwrite(grid[cell].ml.sdate_fixed,sizeof(int),2*ncft,file)!=2*ncft)
                    break;
            }
            if(fwrite(grid[cell].ml.sowing_month,sizeof(int),2*ncft,file)!=2*ncft)
                break;
            if(fwrite(grid[cell].ml.gs,sizeof(int),2*ncft,file)!=2*ncft)
                break;

            if(grid[cell].ml.landfrac!=NULL)
                fwritelandfrac(file,grid[cell].ml.landfrac,ncft,nagtree);

            if(fwrite(grid[cell].ml.sowing_date, sizeof(int),2*ncft,file)!=2*ncft)
                break;
        }

    } /* of 'for(cell=...)' */

    return cell;
} /* of 'fwritecell' */
