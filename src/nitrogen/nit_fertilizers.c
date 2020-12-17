
#include "lpj.h"
#include "agriculture.h"

/* global variable containing gridded manure data. it
 * contains the sedac ratio and the N manure for crop and pasture (fao data) */
Fertdata *fertdata;

/* global vars for the manure crop and pasture files (which are used during all the run,
 * contrary to the sedac files which are used only once */
Climatefile *fert_n_file, *fert_p_file;

/* Updates of the IFA data array from the binary files (each year).
 *
 * Note that the data are in thousands tons (per country per year)
 *
 * Called in iterate.c
 *
 * @param Config config: constant pointer to the configuration object
 * @param Cell grid: array of Cell
 * @param int year: current year
 *
 * \author Nicolas Barrier
 * \update Mohamed Ayache
 * */
Bool nit_update_fert_file(const Config *config, Cell grid[], int year)
{
    nit_readfiles(fert_n_file, fertdata->fert_n, 0.0, fert_n_file->scalar, grid, year, config);
    nit_readfiles(fert_p_file, fertdata->fert_p, 0.0, fert_p_file->scalar, grid, year, config);

    return FALSE;
}

/* Initialisation of the IFA fertilizers files
 *
 * \author Nicolas Barrier
 *
 * @param Config config: constant pointer to the configuration object
 *
 * */
Bool nit_init_fert(const Config *config)
{

    /* allocation of pointers */
    fert_n_file = malloc(sizeof(Climatefile));
    fert_p_file = malloc(sizeof(Climatefile));
    fertdata = malloc(sizeof(Fertdata));

    /* opening of the two files */

    if(nit_openfiles(fert_n_file, &config->fert_n_filename, "%", sizeof(int), config))
    {
        free(fert_n_file);
        return TRUE;
    }

    if(nit_openfiles(fert_p_file, &config->fert_p_filename, "%", sizeof(int), config))
    {
        free(fert_p_file);
        return TRUE;
    }

    /* allocation of the data arrays */
    fertdata->fert_n = newvec(Real, fert_n_file->n);
    fertdata->fert_p = newvec(Real, fert_p_file->n);

    return FALSE;
}

/* Daily update of fertilizers
 *
 * @param Config config: constant pointer to the config object
 * @param Cell cell: pointer to the current cell
 * @param int day: Current day
 * @param Real fracn: Yearly input of N fertilizers (read from file)
 * @param Real fracp: Yearly input of P fertilizers (read from file)
 * @param int year: Current year
 *
 *
 * */
void nit_fert(const Config *config, Cell *cell, int day, Real fracn, Real fracp, int year, int cellnumber){

    Real frac_fert;  /*fraction of expanded manure */
    int countcode;  /* country code of the current grid cell */
    /* looping indexes  */
    int s;
    int p;
    Pft *pft;
    Stand *stand;
    int idsow; // index of the pft within the sowing date array (2*(ncft))
    int idap;  // index of the pft within the application array (2*(ncft+nagtree))
    int idyear;  // year index within the crop/grass fraction arrays

    /* daily n and p manures */
    Real fertn_daily, fertp_daily;
    Real fertn_annual, fertp_annual;
    Real standArea;

    Pftcrop *pftcrop;
    Irrigation *irrigdata;

    int sowdate;

    Real nit_allfrac_res;
    Real nit_allfrac_nopulse_res;


    // year index within the array
    idyear = year - countryfrac->firstyear;

    countcode = cell->ml.manage.par->id; /* country index */

    fertn_daily = fertp_daily = 0; //g/m2


     int standnumber =0;

    // loop over the stands of the current cell
    foreachstand(stand, s, cell->standlist){


        fertn_daily = fertp_daily = 0; //g/m2
        fertn_annual=fertp_annual=0;

        standnumber+=1;
        switch(stand->type->landusetype)  /* checking the stand type */
        {

            // If Agriculture stand
            case AGRICULTURE:

                irrigdata = stand->data; // defines whether we have irrigation

                /* loop over all the PFTs */
                foreachpft(pft, p, &stand->pftlist)
                {
                    pftcrop = pft->data;

                    /* extraction of the cft id (correction of the non-crops pfts/cfts) */
                    idsow = pft->par->id - config->npft[TREE] - config->npft[GRASS];
                    /* extraction of the pft index within the application array (2*(ncft+nagtree)) */
                    idap = pft->par->id - (config->npft[TREE] + config->npft[GRASS] - config->nagtree);
                    /* if irrigated stand, we increment the cft index */
                    if (irrigdata->irrigation==TRUE){
                        idsow += config->npft[CROP];
                        idap += config->npft[CROP] + config->nagtree;
                    }

                    /* sowing day */
                    sowdate = cell->ml.sowing_date[idsow] ;  /* sowing date */

                    if(pftcrop->wtype==TRUE){
                        /* for winter type crops, 3 applications
                         * first application on sowing
                         * */
                        if(day>=sowdate && stand->cell->ml.application[idap].ferti[0]==1){
                            frac_fert = 1/3.;
                            // fracn = thousands tons (per year per country)
                            // 1e9 => thousands tons to g

                            nit_allfrac_nopulse_res=nit_allfrac_nopulse(countcode, idyear);
                            nit_allfrac_res= nit_allfrac(countcode, idyear);
                            if (nit_allfrac_nopulse_res > 0)
                                {
                                    fertn_annual = 1e9 * fracn / nit_allfrac_nopulse_res; // g/m2
                                }
                            else
                                {
                                    fertn_annual=0;
                                }
                            if (nit_allfrac_res > 0)
                                {
                                    fertp_annual = 1e9 * fracp / nit_allfrac_res;   // g/m2
                                }
                            else
                                {
                                   fertp_annual =0;

                                }

                            fertn_daily = frac_fert * fertn_annual ;
                            fertp_daily = frac_fert * fertp_annual  ;
                            stand->lastdate_fert = day;
                            stand->cell->ml.application[idap].ferti[0] = 0;
                            stand->cell->ml.application[idap].ferti[1] = 1;
                        }
                        // second application on 1st of April if the last three days were not rainy
                        else if(day>=91 && cell->preccheck[3]<3 && stand->cell->ml.application[idap].ferti[1]==1){
                            frac_fert = 1/3.;

                            nit_allfrac_nopulse_res=nit_allfrac_nopulse(countcode, idyear);
                            nit_allfrac_res= nit_allfrac(countcode, idyear);
                            if (nit_allfrac_nopulse_res > 0)
                                {
                                    fertn_annual = 1e9 * fracn / nit_allfrac_nopulse_res; // g/m2
                                }
                            else
                                {
                                    fertn_annual=0;
                                }
                            if (nit_allfrac_res > 0)
                                {
                                    fertp_annual = 1e9 * fracp / nit_allfrac_res;   // g/m2
                                }
                            else
                                {
                                   fertp_annual =0;

                                }
                            fertn_daily = frac_fert * fertn_annual  ;
                            fertp_daily = frac_fert * fertp_annual  ;
                            stand->lastdate_fert = day;
                            stand->cell->ml.application[idap].ferti[1] = 0;
                            stand->cell->ml.application[idap].ferti[2] = 1;
                        }
                        else if(day>=stand->lastdate_fert+3*7 && cell->preccheck[3]<3 && stand->cell->ml.application[idap].ferti[2]==1){
                            frac_fert = 1/3.;

                            nit_allfrac_nopulse_res=nit_allfrac_nopulse(countcode, idyear);
                            nit_allfrac_res= nit_allfrac(countcode, idyear);
                            if (nit_allfrac_nopulse_res > 0)
                                {
                                    fertn_annual = 1e9 * fracn / nit_allfrac_nopulse_res; // g/m2
                                }
                            else
                                {
                                    fertn_annual=0;
                                }
                            if (nit_allfrac_res != 0)
                                {
                                    fertp_annual = 1e9 * fracp / nit_allfrac_res;   // g/m2
                                }
                            else
                                {
                                   fertp_annual =0;

                                }
                            fertn_daily = frac_fert * fertn_annual  ;
                            fertp_daily = frac_fert * fertp_annual  ;
                            stand->lastdate_fert = day;
                            stand->cell->ml.application[idap].ferti[2] = 0;
                        }
                    }  /* end testing on winter type */

                    else if (pftcrop->wtype==FALSE){
                        /* for summer/temperate type crops, 2 applications
                         * first application at sowwing
                         * */
                        if(day>=sowdate && stand->cell->ml.application[idap].ferti[0]==1){  /* sowdate and sowingdate + 3 weeks */
                            frac_fert = 0.5;

                            nit_allfrac_nopulse_res=nit_allfrac_nopulse(countcode, idyear);
                            nit_allfrac_res= nit_allfrac(countcode, idyear);

                            if (nit_allfrac_nopulse_res > 0)
                                {
                                    fertn_annual = 1e9 * fracn / nit_allfrac_nopulse_res; // g/m2
                                }
                            else
                                {
                                    fertn_annual=0;
                                }
                            if (nit_allfrac_res > 0)
                                {
                                    fertp_annual = 1e9 * fracp / nit_allfrac_res;   // g/m2
                                }
                            else
                                {
                                   fertp_annual =0;

                                }
                            fertn_daily = frac_fert * fertn_annual  ;
                            fertp_daily = frac_fert * fertp_annual  ;
                            stand->lastdate_fert = day;
                            stand->cell->ml.application[idap].ferti[0] = 0;
                            stand->cell->ml.application[idap].ferti[1] = 1;
                        }
                        // second application 3 weeks after sowing
                        else if(day>=sowdate+3*7 && cell->preccheck[3]<3 && stand->cell->ml.application[idap].ferti[1]==1){/*sowdate and sowingdate + 3 weeks*/
                            frac_fert = 0.5;

                            nit_allfrac_nopulse_res=nit_allfrac_nopulse(countcode, idyear);
                            nit_allfrac_res= nit_allfrac(countcode, idyear);
                            if (nit_allfrac_nopulse_res > 0)
                                {
                                    fertn_annual = 1e9 * fracn / nit_allfrac_nopulse_res; // g/m2
                                }
                            else
                                {
                                    fertn_annual=0;
                                }
                            if (nit_allfrac_res > 0)
                                {
                                    fertp_annual = 1e9 * fracp / nit_allfrac_res;   // g/m2
                                }
                            else
                                {
                                   fertp_annual =0;

                                }
                            fertn_daily = frac_fert * fertn_annual  ;
                            fertp_daily = frac_fert * fertp_annual  ;
                            stand->lastdate_fert = day;
                            stand->cell->ml.application[idap].ferti[1] = 0;
                        }
                    }  /* end if testing on winter type */
                } /* end looping on pft */
                break;

            case AGRICULTURE_TREE: case AGRICULTURE_GRASS:

                /* permanent crop */

                irrigdata = stand->data; // defines whether we have irrigation

                foreachpft(pft, p, &stand->pftlist)
                {
                    idap = pft->par->id - (config->npft[TREE] + config->npft[GRASS] - config->nagtree);
                    /* if irrigated stand, we increment the cft index */
                    if (irrigdata->irrigation==TRUE){
                        idap += config->npft[CROP] + config->nagtree;
                    }

                    // for non permanent crops, 2 applications
                    // first application on 1st of April if the last 3 days were not rainy
                    if (day>=91 && cell->preccheck[3]<3 && stand->cell->ml.application[idap].ferti[0]==1)  /* if 1st of march and 3 weeks after 1st of march */
                    {
                        frac_fert = 0.5;

                        nit_allfrac_nopulse_res=nit_allfrac_nopulse(countcode, idyear);
                        nit_allfrac_res= nit_allfrac(countcode, idyear);

                            if (nit_allfrac_nopulse_res > 0)
                                {
                                    fertn_annual = 1e9 * fracn / nit_allfrac_nopulse_res; // g/m2
                                }
                            else
                                {
                                    fertn_annual=0;
                                }
                            if (nit_allfrac_res > 0)
                                {
                                    fertp_annual = 1e9 * fracp / nit_allfrac_res;   // g/m2
                                }
                            else
                                {
                                   fertp_annual =0;

                                }
                        fertn_daily = frac_fert * fertn_annual  ;
                        fertp_daily = frac_fert * fertp_annual  ;
                        stand->lastdate_fert = day;
                        stand->cell->ml.application[idap].ferti[0] = 0;
                        stand->cell->ml.application[idap].ferti[1] = 1;
                    }
                    // 2nd application three weeks after the first one, if the last 3 days were not rainy
                    if(day>=stand->lastdate_fert+3*7 && cell->preccheck[3]<3 && stand->cell->ml.application[idap].ferti[1]==1){
                        frac_fert = 0.5;

                        nit_allfrac_nopulse_res=nit_allfrac_nopulse(countcode, idyear);
                        nit_allfrac_res= nit_allfrac(countcode, idyear);

                            if (nit_allfrac_nopulse_res > 0)
                                {
                                    fertn_annual = 1e9 * fracn / nit_allfrac_nopulse_res; // g/m2
                                }
                            else
                                {
                                    fertn_annual=0;
                                }
                            if (nit_allfrac_res > 0)
                                {
                                    fertp_annual = 1e9 * fracp / nit_allfrac_res;   // g/m2
                                }
                            else
                                {
                                   fertp_annual =0;

                                }
                        fertn_daily = frac_fert * fertn_annual  ;
                        fertp_daily = frac_fert * fertp_annual  ;
                        stand->lastdate_fert = day;
                        stand->cell->ml.application[idap].ferti[1] = 0;
                    }
                }
                break;

        } /* end of switch */

        /* update of the soil residual */

        stand->soil.NO3[0] += FRAC_NO3_FERTN * fertn_daily ;

        stand->soil.NH4[0] += (1-FRAC_NO3_FERTN) * fertn_daily;
        stand->soil.PO4[0] += fertp_daily;

        cell->output.mNferti=fertn_daily *stand->cell->coord.area* stand->frac;

        cell->output.mPO4ferti+=fertp_daily*stand->cell->coord.area* stand->frac ;

    }
}  /* end of function */
