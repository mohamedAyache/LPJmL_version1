
#include "lpj.h"

/* Module for nitrification (09)
 *
 * \author Nicolas Barrier
 *
 */

static Real f_wfps(const Soil *soil,      /* Soil data */
        int l                  /* soil layer */
        )                       /* return soil temperature (deg C) */
{
    Real x, z;
    x=(soil->w[l]*soil->par->whcs[l]+soil->par->wpwps[l]+soil->w_fw[l]+soil->ice_fw[l])/soil->par->wsats[l];
    z = pow((x-B_NIT)/N_NIT,Z_NIT)*pow((x-C_NIT)/M_NIT,D_NIT);
    return z;
} /* of 'f_wfps' */


/* Nitrification in water (09, section 2). Inputs are the current cell (for water temp)
 * and pointers to the no3 and nh4 concentrations, which are updated.
 *
 * Called in "nit_update_river" and "nit_update_lakes"
 *
 * @param Cell cell: constant pointer to the current cell.
 * @param Real no3: pointer to the NO3 content in water (updated)
 * @param Real nh4: pointer to the NO4 content in water (updated)
 *
 * \author Nicolas Barrier
 */
void nit_nitrification_water(const Cell *cell, const Real vwat, Real *no3, Real *nh4)
{

    Real nitr_water;  //daily amount of NH4 denitrified in the water (g)

    // if the water volume is too small, nothing is done
    if (vwat == 0)
        return;

    /*update of river values */
    /* compared to AS's document, two updates:
     * - in the denominator, multiplication of KWAT
     *   by the volume (work in concentration
     *   instead of weights
     * - multiplication of VMAX_WAT_NITR by vwat since it is in g.m-3.d-1
     */
    nitr_water = vwat * VMAX_WAT_NITR * nit_tempdep(cell->water_temp, 24.0, 6.0) * (*nh4)/(*nh4 + K_WAT_NITR*vwat);

    *no3 += nitr_water;
    *nh4 -= nitr_water;
}


/* Nitrification in soil, extracted from the nitrogen branch
 * and adapted by Nicolas Barrier.
 *
 * Since the NO3 content in the nitrogen branch is in g/m2 while in g here, it is first
 * converted into the right unit by dividing by the stand area (i.e. cell area x stand fraction).
 * At the end, it is converted back into the right unit.
 *
 * Called in "update_daily".
 *
 * \author Nicolas Barrier
 *
 * */
void nit_nitrification(Stand *stand){

    int l;
    Real fac_wfps;
    Real fac_temp;
    Real F_NO3;
    /* Real F_N2O; barrier.n: unused */
    Soil *soil;

    F_NO3 = 0;

    /* setting the soil pointer */
    soil = &stand->soil;

    /* NO3 and N2O from nitrification */
    forrootsoillayer(l)
    {

        fac_wfps = f_wfps(soil, l);
        fac_temp = f_temp(soil->temp[l]);
        F_NO3 = K_MAX*soil->NH4[l] * fac_temp * fac_wfps;
        if(F_NO3 > soil->NH4[l])
            F_NO3 = soil->NH4[l];

        soil->NO3[l] += F_NO3;
#ifdef SAFE
        //if(soil->NO3[l]<-epsilon*100)
        if(soil->NO3[l]<-0.1)
            fail(NEGATIVE_SOIL_NO3_ERR,TRUE,"littersom: Negative soil NO3=%g in layer %d in cell (%.2f %.2f)",soil->NO3[l],l,stand->cell->coord.lat,stand->cell->coord.lon);
#endif
        soil->NH4[l] -= F_NO3;
#ifdef SAFE
        if(soil->NH4[l]<-epsilon)
            fail(NEGATIVE_SOIL_NH4_ERR,TRUE,"Negative soil NH4=%g in layer %d in cell (%.2f,%.2f)",soil->NH4[l],l,stand->cell->coord.lat,stand->cell->coord.lon);
#endif
        /* flux.nitrogen += F_N2O; */
        /* F_N2O is given back for output */

    }
}
