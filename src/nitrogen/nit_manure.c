
#include "lpj.h"
#include "agriculture.h"

/* Module associated with manure on agriculture.
 *
 * \author Nicolas Barrier
 *
 */

/* global variable containing gridded manure data.
 * it contains the sedac ratio and the N manure for crop and pasture (fao data) */
Manuredata *manuredata;

/* global vars for the manure crop and pasture files (which are used during all the run,
 * contrary to the sedac files which are used only once */
Climatefile *manure_n_crop_file, *manure_n_past_file; Climatefile *manure_sedac_p_file; Climatefile *manure_sedac_n_file;

/* Initialisation of the manure N for crop/pasture.
 * - opening of the input N manure files.
 * - allocation of the crop/pasture N manure variables.
 *
 * Called in "lpjml" if "landuse" and "with_manure" are activated.
 *
 *  WARNING: must be called AFTER the "nit_init_manure_sedac" function since, the latter allocates
 *  the manuredata variable.
 *
 *  @param Config config: constant pointer to the configuration object
 *
 * \author Nicolas Barrier
 * \update Mohamed Ayache
 *
 */
Bool nit_init_manure_n(const Config *config)
{

    /* allocation of file pointers */
    manure_n_crop_file = new(Climatefile);
    manure_n_past_file = new(Climatefile);

    /* opening of the two files */
    if(nit_openfiles(manure_n_crop_file, &config->manure_n_crop_filename, "%", sizeof(int), config))
    {
        free(manure_n_crop_file);
        return TRUE;
    }

    if(nit_openfiles(manure_n_past_file, &config->manure_n_past_filename, "%", sizeof(int), config))
    {
        free(manure_n_past_file);
        return TRUE;
    }

    /* allocation of the data arrays */
    manuredata->man_ann_crop_n = newvec(Real, manure_n_crop_file->n);
    manuredata->man_ann_past_n = newvec(Real, manure_n_past_file->n);

    return FALSE;
}

/* Update of the FAO data array from binary files on yearly time step.
 *
 * Data are in kg (per country per year)
 *
 * Called in "iterate" on yearly time steps.
 *
 * @param Config config: constant pointer to the config object
 * @param Cell grid: array of Cells
 * @param int year: Current year
 *
 * \author Nicolas Barrier.
 *
 * */
Bool nit_update_manure_file(const Config *config, Cell grid[], int year)
{
    nit_readfiles(manure_n_crop_file, manuredata->man_ann_crop_n, 0.0, manure_n_crop_file->scalar, grid, year, config);
    nit_readfiles(manure_n_past_file, manuredata->man_ann_past_n, 0.0, manure_n_past_file->scalar, grid, year, config);

    return FALSE;
}


/* Initialisation of the manure sedac variable, which is a 2D file (lon/lat).
 *  - opening of the manure sedac files (N/P)
 *  - allocation of the "manuredata" variable.
 *  - creation of two temporary variables (sedacn and sedacp)
 *  - calculation of the ratios sedecn/sedacp, storage in the manuredata->sedac variable.
 *  - desallocation of the temporary variables.
 *
 *  Note that the manuredata->man_sedac array as no units, while sedac data are in
 *  kg (per country per year)
 *
 *  Called in "lpjml".
 *
 *  WARNING: must be called BEFORE the "nit_init_manure_n" function since the latter assumes that
 *  the manuredata variable has been allocated.
 *
 *  \author Nicolas Barrier.
 *
 */
Bool nit_init_manure_sedac(const Config *config, Cell grid[])
{

    int icell;
    Real *man_n, *man_p;

    /* initialisation of the manure data.
     * open the two binary manure files, and
     * extracts the N/P ratio
     * \author Nicolas Barrier */

    /* declaration of the two pointers to the climate file */

    /* initialisation of the two file pointers
     * and of the global manuredata array */
    manure_sedac_p_file = new(Climatefile);
    manure_sedac_n_file = new(Climatefile);
    manuredata = new(Manuredata);


    /* Opening of the two manure files. Output is passed to the file pointers */
    if(nit_openfiles(manure_sedac_p_file, &config->manure_sedac_p_filename, "%", sizeof(int), config))
    {
        free(manure_sedac_p_file);
        return TRUE;
    }

    if(nit_openfiles(manure_sedac_n_file, &config->manure_sedac_n_filename, "%", sizeof(int), config))
    {
        free(manure_sedac_n_file);
        return TRUE;
    }

    /* Initialisation of the data arrays (one element per grid cell) */
    man_n = newvec(Real, manure_sedac_n_file->n);
    man_p = newvec(Real, manure_sedac_p_file->n);
    manuredata->man_sedac = newvec(Real, manure_sedac_p_file->n);

    /* Reading the manure files, and storing the data into the manuredata arrays.
     * /!\ WARNING /!\ The "year" argument has been set to manure_n->firstyear in
     * order to insure that the first index is read, no matter which config->firstyear
     * or file->firstyear
     */

    nit_readfiles(manure_sedac_n_file, man_n, 0.0, manure_sedac_n_file->scalar, grid, manure_sedac_n_file->firstyear, config);
    nit_readfiles(manure_sedac_p_file, man_p, 0.0, manure_sedac_p_file->scalar, grid, manure_sedac_p_file->firstyear, config);

    // calculation of the sedac ratio N/P
    for (icell=0; icell<manure_sedac_p_file->n; icell++){
            if(man_p[icell] ==0 || man_n[icell]==0)
            {
                *(manuredata->man_sedac + icell) = 1./5.;
            }
            else
            {
                *(manuredata->man_sedac + icell) = *(man_p + icell) / *(man_n + icell);
            }


    }

    /* desallocation of the man_n and man_p arrays */
    free(man_n);
    free(man_p);
    free(manure_sedac_n_file);
    free(manure_sedac_p_file);

    return FALSE;
}


/* Daily update of manure.
 *
 * Called in "iterateyear_river"
 *
 * WARNING: because of the stand philosophy of LPJml, we cannot put the manure two weeks before sowing, since
 * two weeks before sowing the stand is not AGRICULTURE but SETASIDE, with no crop pft on it...
 *
 * @param Config config: constant pointer to configuration object (to retrieve number of cft/pft/ag_trees)
 * @param Cell cell: pointer to the cell array (used to update the nutrients soil contents)
 * @param int day: current day of the year (ranges 1-366)
 * @param Real manpast: annual N manure for pasture (read from the file)
 * @param Real mancrop: annual N manure for crop (read from the file)
 * @param Real mansedac: N/P sedac ratio for the current year
 * @param int year: current year
 *
 * \author Nicolas Barrier
 *
 * */
void nit_manure(const Config *config, Cell *cell, int day, Real manpast, Real mancrop, Real mansedac, int year, int cellnumber){

    Real frac_man;  /*fraction of expanded manure */
    int countcode;  /* country code of the current grid cell */

    /* looping indexes  */
    int s;
    int p;
    int idsow; // index of the pft within the sowing date array (2*(ncft))
    int idap;  // index of the pft within the application array (2*(ncft+nagtree))

    int idyear;  // index of the year within the crop/grass fractions

    Pft *pft;

    Stand *stand;

    /* daily n and p manures */
    Real mann_daily, manp_daily; //g/m2
    Real mann_annual, manp_annual;//g/m2

    Real standArea; //m2

    Pftcrop *pftcrop;
    Irrigation *irrigdata;

    int sowdate;

    Real nit_cropfrac_res;
    Real nit_grassfrac_res;

    idyear = year - countryfrac->firstyear;

    countcode = cell->ml.manage.par->id; /* country index */

    /* initialisation of the daily manure to 0 */
    mann_daily = manp_daily = 0;



    foreachstand(stand,s,cell->standlist){
           /* initialisation of the daily manure to 0 */
        mann_daily = manp_daily = 0;
        mann_annual=manp_annual=0;

        switch(stand->type->landusetype)  /* checking the stand type */
        {
            case GRASSLAND:  // if we are on a grassland stand (i.e pasture), the manure fraction is constant

                if ((day>= 121) && (day<= 274))  /* if between 1th may and 30th septembre, equi-repartition of the manure */
                {
                    frac_man = 1/154. ;   // constant manure fraction
                    foreachpft(pft, p, &stand->pftlist)
                    {
                        // annual N manure  for grassland for the current cell
                        // 1e3 => conversion kg to g
                        // mann_annual => g / m2
                        nit_grassfrac_res= nit_grassfrac(countcode, idyear);

                        if (nit_grassfrac_res >0)
                        {
                            mann_annual = 1e3 * manpast / nit_grassfrac_res ;
                        }
                        else
                        {
                            mann_annual =0;
                        }
                        manp_annual = mann_annual * mansedac;   // annual P manure for grassland for the current cell
                        mann_daily = frac_man * mann_annual ;  // daily N manure on the stand (g)
                        manp_daily = frac_man * manp_annual ;  // daily P manure on the stand

                    }
                }
                break;

            case AGRICULTURE:  // if we are on a non permanent crop stand

                irrigdata = stand->data;  // determines whether irrigation or not

                /* non-permanent crop */
                foreachpft(pft, p, &stand->pftlist)
                {
                    pftcrop = pft->data;
                    /* extraction of the cft id (correction of the non-crops pfts/cfts) */
                    idsow = pft->par->id - config->npft[TREE] - config->npft[GRASS];
                    /* extraction of the index within the crop+agtree array */
                    idap = pft->par->id - (config->npft[TREE] + config->npft[GRASS] - config->nagtree);
                    /* if irrigated stand, we increment the both indexes */
                    if (irrigdata->irrigation==TRUE){
                        idsow += config->npft[CROP];
                        idap += config->npft[CROP] + config->nagtree;
                    }

                    /* sowing day on the current cell for the current crop*/
                    sowdate = cell->ml.sowing_date[idsow] ;  /* 2 weeks before sowing */
                    if (sowdate < 0)  /* correction of the sowdate index if negative */
                        sowdate += 365;

                    if(pftcrop->wtype==TRUE){
                        /* for winter type crops 2 applications */
                        /* first manure application on sowing (since not possible 2 weeks before)*/
                        if(day==sowdate && stand->cell->ml.application[idap].manure[0]==1){
                            frac_man = 0.5;   // first fraction

                            nit_cropfrac_res= nit_cropfrac(countcode, idyear);

                            if (nit_cropfrac_res >0)
                            {
                                mann_annual = 1e3 * mancrop / nit_cropfrac_res;
                            }
                            else
                            {
                                mann_annual =0;
                            }

                            manp_annual = mann_annual * mansedac;
                            mann_daily = frac_man * mann_annual ;
                            manp_daily = frac_man * manp_annual ;
                            stand->lastdate_man = day;  // update of the last manure date for the current stand
                            stand->cell->ml.application[idap].manure[0]=0;   // application of the application array for the current cell
                            stand->cell->ml.application[idap].manure[1]=1;   // application of the application array for the current cell

                        }
                        // second application on the first of march if the last 3 days were not rainy
                        else if(day>=60 && cell->preccheck[3]<3 && stand->cell->ml.application[idap].manure[1]==1){
                            frac_man = 0.5;

                             nit_cropfrac_res= nit_cropfrac(countcode, idyear);

                             if (nit_cropfrac_res >0)
                             {
                                 mann_annual = 1e3 * mancrop / nit_cropfrac_res;
                             }
                             else
                             {
                                 mann_annual =0;
                             }

                            manp_annual = mann_annual * mansedac;
                            mann_daily = frac_man * mann_annual ;
                            manp_daily = frac_man * manp_annual ;
                            stand->lastdate_man = day;
                            stand->cell->ml.application[idap].manure[1]=0;

                        }


                    }  /* end testing on winter type */
                    else if (pftcrop->wtype==FALSE){
                        /* for summer/temperate type crops, 2 applications */
                        // first one: on sowing (since not possible to apply 2 weeks before sowing)
                        if(day==sowdate && stand->cell->ml.application[idap].manure[0]==1){  /* sowdate and sowingdate + 3 weeks */
                            frac_man = 0.5;
                            nit_cropfrac_res=  nit_cropfrac(countcode, idyear);

                             if (nit_cropfrac_res >0)
                             {
                                  mann_annual = 1e3 * mancrop /nit_cropfrac_res;
                             }
                             else
                             {
                                mann_annual =0;
                             }

                            manp_annual = mann_annual * mansedac;
                            mann_daily = frac_man * mann_annual ;
                            manp_daily = frac_man * manp_annual ;
                            stand->lastdate_man = day;  // update of the last manure date
                            stand->cell->ml.application[idap].manure[0]=0;
                            stand->cell->ml.application[idap].manure[1]=1;

                        }
                        // second application 3 weeks after the last one if no rain on the last three days
                        else if(day>=stand->lastdate_man+3*7 && cell->preccheck[3]<3 && stand->cell->ml.application[idap].manure[1]==1){  /* sowdate and sowingdate + 3 weeks */
                            frac_man = 0.5;

                            nit_cropfrac_res = nit_cropfrac(countcode, idyear);

                            if (nit_cropfrac_res >0)
                            {
                                mann_annual = 1e3 * mancrop / nit_cropfrac_res ;
                            }
                            else
                            {
                                mann_annual=0;
                            }

                            manp_annual = mann_annual * mansedac;
                            mann_daily = frac_man * mann_annual  ;
                            manp_daily = frac_man * manp_annual  ;
                            stand->lastdate_man = day;
                            stand->cell->ml.application[idap].manure[1]=0;

                        }
                    }  /* if testing on winter type */
                } /* end looping on pft */
                break;

            case AGRICULTURE_TREE: case AGRICULTURE_GRASS:

                irrigdata = stand->data;  // determines whether irrigation or not

                /* permanent crop */
                foreachpft(pft, p, &stand->pftlist)
                {
                    idap = pft->par->id - (config->npft[TREE] + config->npft[GRASS] - config->nagtree);
                    /* if irrigated stand, we increment the cft index */
                    if (irrigdata->irrigation==TRUE){
                        idap += config->npft[CROP] + config->nagtree;
                    }

                    // for agricultural trees, two applications
                    // the first one on the 1st of march if no precipitation on the last 3 days
                    if (day>=60 && cell->preccheck[3]<3 && stand->cell->ml.application[idap].manure[0]==1)  /* if 1st of march and 3 weeks after 1st of march */
                    {
                        frac_man = 0.5;

                        nit_cropfrac_res = nit_cropfrac(countcode, idyear);

                        if (nit_cropfrac_res >0)
                        {
                            mann_annual = 1e3 * mancrop /nit_cropfrac_res ;
                        }
                        else
                        {
                            mann_annual=0;
                        }

                        manp_annual = mann_annual * mansedac;
                        mann_daily = frac_man * mann_annual  ;
                        manp_daily = frac_man * manp_annual  ;
                        stand->lastdate_man = day;
                        stand->cell->ml.application[idap].manure[0]=0;
                        stand->cell->ml.application[idap].manure[1]=1;
                    }
                    // the second application occurs 3 weeks after the first one if no precipitation during the last three days
                    if(day>=stand->lastdate_man+3*7 && cell->preccheck[3]<3 && stand->cell->ml.application[idap].manure[1]==1)
                    {
                        frac_man = 0.5;

                        nit_cropfrac_res=nit_cropfrac(countcode, idyear);

                        if (nit_cropfrac_res > 0)
                        {
                            mann_annual = 1e3 * mancrop / nit_cropfrac_res ;
                        }
                        else
                        {
                            mann_annual=0;
                        }


                        manp_annual = mann_annual * mansedac;
                        mann_daily = frac_man * mann_annual  ;
                        manp_daily = frac_man * manp_annual  ;
                        stand->lastdate_man = day;
                        stand->cell->ml.application[idap].manure[1]=0;
                    }
                }
                break;

        } /* end of switch */

        /* update of the soil residual */
        stand->soil.RESN[0] += mann_daily;
        stand->soil.RESP[0] += manp_daily;

        cell->output.mNmanure+=mann_daily*stand->cell->coord.area* stand->frac;
        cell->output.mPO4manure+=manp_daily*stand->cell->coord.area* stand->frac;
    }

}  /* end of function */
