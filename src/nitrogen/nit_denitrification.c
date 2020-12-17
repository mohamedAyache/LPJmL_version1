
/* Module for the denitrification in water (02)*/

#include "lpj.h"

/* Denitrifaction in water (02, section 02)
 * It takes as input the current cell (for water temperature) and a pointer to
 * the NO3 concentration, which is updated.
 *
 * Called in "nit_update_river" and "nit_update_lakes"
 *
 * @param Cell cell: constant pointer to the current cell
 * @param Real no3: pointer to the NO3 content in water
 *
 * \author Nicolas Barrier
 *
 * */
void nit_watdenit(const Cell *cell, Real *no3, const Real vwat)
{
    Real denwater;  // daily denitrification in water

    // if no water within the grid cell, then nothing is done
    if (vwat == 0)
        return;

    // computation of denitrification (equation 1)
    denwater = *no3 * nit_tempdep(cell->water_temp, 24.0, 14.0)*(1 - exp(-VIT_WAT_DEN/LOAD_WAT));

    /* update of NO3 values */
    *no3 -= denwater;
}

/* Denitrification in soil (02, section 01)
 * Extracted from the nitrogen branch
 * and adapted by Nicolas Barrier.
 *
 * Since in the branch, soil->NO3 is in g/m2 while here it is in g,
 * the value is first converted into the right unit by dividing by the stand area
 * (i.e. cell area * stand fraction). At the end, it is put back into the right unit.
 *
 * Called in "update_daily"
 *
 * \author Nicolas Barrier
 * \update Mohamed Ayache
 */
void nit_denitrification(Stand *stand)
{

    /* determines NO2 and N2 from nitrate NO3 */
    Real N_denit=0; /* amount of nitrate lost to denitrification */
    Real N2O_denit, denit_t;
    Real FT,FW,TCDF;
    Real Corg;
    int l;

    Soil *soil;

    /* setting the soil pointer */
    soil = &stand->soil;

    forrootsoillayer(l)
    {
        /* Corg = soil->pool[l].fast.carbon+soil->pool[l].slow.carbon;
         * barrier.n*/
        Corg = soil->cpool[l].fast + soil->cpool[l].slow ;


        //printf("corg=%e\n", Corg);
        if(Corg<0)
            Corg=0;
        if(soil->temp[l]>epsilon)
            FT = 0.0326+0.00351*pow(soil->temp[l],1.652)-pow((soil->temp[l]/41.748),7.19);
        /* Equation C5 from Smith et al 2014 but only for positive temp */
        else if (soil->temp[l] > 45.9) /* otherwise FT is negative */
            FT=0.0;
        else
            FT=0.0326;
#ifdef DEBUG
       /* printf("w=(%g + %g + %g  + %g + %g )/ %g\n",soil->par->wpwps[l],soil->w[l]*soil->par->whcs[l],soil->ice_depth[l],
                soil->w_fw[l],soil->ice_fw[l],soil->par->wsats[l]);*/
#endif
        denit_t = (soil->par->wpwps[l]+soil->w[l]*soil->par->whcs[l]+soil->ice_depth[l]+
                soil->w_fw[l]+soil->ice_fw[l])/soil->par->wsats[l]; /* denitrification threshold dependent on water filled pore space */

        /* Version without threshold*/
        N_denit = 0.0;
        N2O_denit = 0.0;
        FW = 6.664096e-10*exp(21.12912*denit_t); /* newly fitted parameters on curve with threshold */
        TCDF = 1-exp(-CDN*FT*Corg);
        N_denit = FW*TCDF*soil->NO3[l];
#ifdef SAFE
        if((FW*TCDF)>1.0 && N_denit>(soil->NO3[l]-epsilon*10)){
            fprintf(stdout,"too large denitrification in layer %d: N_denit %g FW %g TCDF %g NO3 %g FT %g Corg %g\n",l,N_denit,FW,TCDF,soil->NO3[l],FT,Corg);
            fflush(stdout);
            N_denit=soil->NO3[l];
        }
#endif

        //printf("barrier.n: n_denit2=%e\n", N_denit);
        if(N_denit>soil->NO3[l])
            N_denit=soil->NO3[l];
        soil->NO3[l]-=N_denit;
        //printf("barrier.n: n_denit3=%e\n", N_denit);
#ifdef SAFE
        if(soil->NO3[l]<-epsilon*10)
            fail(NEGATIVE_SOIL_NO3_ERR,TRUE,"Negative soil NO3=%g in layer %d",soil->NO3[l],l);
#endif
        /* Calculation of N2O from denitrification after Bessou 2010 */
        N2O_denit = 0.11 * N_denit;
        N_denit -= N2O_denit;

        /*
           output->daily.n2_denit  += N_denit;
           output->daily.n2o_denit += N2O_denit;
           output->mn2o_denit+=N2O_denit*standfrac;
           output->mn2_emissions+=N_denit*standfrac; */

    }

} /* of 'denitrification' */
