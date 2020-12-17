/***************************************************************************/
/**                                                                       **/
/**               u  p  d  a  t  e  _  a  n  n  u  a  l  .  c             **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function performs necessary updates after iteration over one      **/
/**     year                                                              **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     See file AUTHORS for list of authors contributing to this code    **/
/**                                                                       **/
/**     Last change: $Date:: 2016-02-11 11:10:20 +0100 (jeu., 11 févr.#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
void update_annual(Cell *cell,           /* Pointer to cell */
        int npft,             /* number of natural pfts */
        int ncft,             /* number of crop pfts */
        Real popdens,         /* population density (capita/km2) */
        int year,
        Bool intercrop,
        const Config *config
        )
{
    int s,i;
    Stand *stand;

    if(cell->ml.dam)
        update_reservoir_annual(cell);
    annual_climbuf(&cell->climbuf);
    if(config->sdate_option==NO_FIXED_SDATE ||
            (config->sdate_option==FIXED_SDATE && year<=param.sdate_fixyear)||
            (config->sdate_option==PRESCRIBED_SDATE && year<=param.sdate_fixyear))
        cell->climbuf.atemp_mean20_fix=cell->climbuf.atemp_mean20;

    /* count number of years without harvest
     * and set 20-year mean to zero if no harvest
     * occurred within the last 10 years
     */

    if((year<config->firstyear && config->sdate_option!=PRESCRIBED_SDATE) ||
            config->sdate_option==NO_FIXED_SDATE)
        update_cropdates(cell->ml.cropdates,ncft);

    cell->output.mUPTRESDIFF=0;
    foreachstand(stand,s,cell->standlist)
    {

        // barrier.n: initialisation of uptake and residue to 0
        nit_init_uptake_annual(stand);
        nit_init_residue_annual(stand);

        stand->soil.mean_maxthaw=(stand->soil.mean_maxthaw-stand->soil.mean_maxthaw/CLIMBUFSIZE)+stand->soil.maxthaw_depth/CLIMBUFSIZE;
        if(annual_stand(stand,npft,ncft,popdens,year,intercrop,config))
        {
            /* stand has to be deleted */
            delstand(cell->standlist,s);
            s--; /* adjust stand index */
        }

        // barrier.n: uptake and remineralisation

        nit_save_annual_uptake_allpft(stand);


        nit_remin_allpft(stand);

        for (i =0 ; i<LASTLAYER; i++) {

        if ((stand->annualUptakeP[i] - stand->annualResP[i]) >0)
        {
           // stand->soil.RESP[i]+=(stand->annualUptakeP[i] - stand->annualResP[i]);

        }
        if ((stand->annualUptakeN[i] - stand->annualResN[i]) >0)
        {  // stand->soil.RESN[i]+=(stand->annualUptakeN[i] - stand->annualResN[i]);


       // stand->soil.RESC[i]+=(stand->annualUptakeN[i] - stand->annualResN[i])*RATIO_CN_ROOT*10;

        }
        //C should never be limiting for the decomposers so the quantity of C we have doesn't really matter as long as it is big enough

        // if some residue remains consider them as part of the next year (to avoid creation of matter)
        if ((stand->annualUptakeP[i] - stand->annualResP[i]) >0) {stand->annualResP[i] =0;}
        else {stand->annualResP[i] = stand->annualResP[i] - stand->annualUptakeP[i]; }
        if ((stand->annualUptakeN[i] - stand->annualResN[i]) >0) {stand->annualResN[i] =0;}
        else {stand->annualResN[i] = stand->annualResN[i] - stand->annualUptakeN[i]; }


        stand->annualUptakeN[i] =0;
        stand->annualUptakeP[i] =0;

        }


    } /* of foreachstand */
    foreachstand(stand,s,cell->standlist)
        cell->output.soil_storage+=soilwater(&stand->soil)*stand->frac*cell->coord.area;
    cell->output.fpc[0] = 1-cell->ml.cropfrac_rf-cell->ml.cropfrac_ir-cell->lakefrac-cell->ml.reservoirfrac;
#ifdef IMAGE
    cell->output.prod_turnover=product_turnover(cell->ml.image_data);
    cell->output.product_pool_fast=cell->ml.image_data->timber.fast;
    cell->output.product_pool_slow=cell->ml.image_data->timber.slow;
#endif
} /* of 'update_annual' */
