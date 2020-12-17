/*
 *
 * Module for phytoplankton dynamics (10)
 *
 * \author Nicolas Barrier
 *
 */

#include "lpj.h"

/* Initialises the phytoplankton using the value of 1gC/m3 of water
 *
 * Called in "newgrid2"
 *
 * @param Cell cell: pointer to the current cell
 *
 * \author Nicolas Barrier
 */
void nit_init_phyto(Cell *cell)
{

    int i, ncoeff;
    Real phyto;  // phyto content (in g)

    if (cell->discharge.queue == NULL)
    {
      fprintf(stderr,"WARNING: no water in river (discharge.queue) cannot initialize phyto \n");
    }
    else
        {
            /* Initialisation for rivers */
            ncoeff = queuesize(cell->discharge.queue);
            for(i=0; i<ncoeff; i++)
            {
                phyto = getqueue(cell->discharge.queue, i) * 1.0e-3; // conversion from dm3 into m3
                up_queue(cell->discharge.PHYQueue, phyto, i);
            }


        }
    /* Initialisation for lakes */
    cell->discharge.PHYlake = cell->discharge.dmass_lake * 1.0e-3;
}


/*
 * Update of nutrient concentration in water associated with phytoplankton dynamics.
 *
 * Arguments are the cell (for water temp), pointers to phyto and nutrients (no3, nh4, po4) concentrations and
 * the volume of water. Note that the pointer values are updated.
 *
 * Called in "nit_update_river" and "nit_update_lakes"
 *
 * @param Cell cell: constant pointer to the current cell
 * @param Real phyto: pointer to the phytoplankton content in water (g)
 * @param Real no3: pointer to the NO3 content in water (g)
 * @param Real nh4: pointer to the nh4 content in water (g)
 * @param Real po4: pointer to the po4 content in water (g)
 * @param Real vwater: constant volume of water (m3)
 *
 * \author Nicolas Barrier
 * \update Mohamed Ayache
 *
 */
void nit_update_phyto(const Cell *cell, Real *phyto, Real *no3, Real *nh4, Real *po4, const Real vwater)
{
    Real Gphyto; /* daily growth of phytoplankton (no units) */

    Real Nlim; /* N Limitation (no units) */
    Real Plim; /* P limitation (no units) */

    /* temporary variable for storing the sum of NO3/NH4 prior to update */
    Real temp_no3_nh4;
    /*equations roots*/
    Real no3_root; Real no3_delta;
    Real po4_root; Real po4_delta;

    /* ===================================================================== Update for river */

    // if no water is present, leaves the functions
    if(vwater <= 10e-20)
        return;

    // calculation of the two limitation terms (equation 1, terms within the min function)
    // Check wether P or N limited. Note that if denominateur is 0, then
    // we set Nlim and Plim to 0.
    Nlim = (*no3 + *nh4)/(*no3 + *nh4 + KN_PHYTO*vwater);
    Plim = *po4 /(*po4 + KP_PHYTO*vwater);

    /* calculation of the daily growth of phytoplankton) */
    Gphyto = MU_PHYTO * nit_tempdep(cell->water_temp, 24.0, 14.0) * min(Nlim, Plim);

    // sum of NO3 + PO4 (denominator of equation 3)
    temp_no3_nh4 = *no3 + *nh4;

    if (*phyto > epsilon)
    {
    /*limit Gphyto so that no3 (and nh4) never becomes negative, Gphyto must be < to the positive root of the quadratic equation *no3 -Gphyto * (*phytoUpdated/CN_RATIO_PHYTO) * (*no3)/(temp_no3_nh4) */
    no3_delta=(1-M_PHYTO)*(*phyto)*(1-M_PHYTO)*(*phyto) - 4 * (*phyto) * (-CN_RATIO_PHYTO)*temp_no3_nh4;
    no3_root= (-(1- M_PHYTO)*(*phyto)+(sqrt(no3_delta)))/2/(*phyto);

    Gphyto = min(Gphyto, no3_root);

    /*limit Gphyto so that po4 never becomes negative, Gphyto must be < to the positive root of the quadratic equation *po4 - Gphyto * (*phytoUpdated/CP_RATIO_PHYTO);) */
    po4_delta = (1-M_PHYTO)*(*phyto)*(1-M_PHYTO)*(*phyto) - 4* (-*phyto)*CP_RATIO_PHYTO*(*po4);
    po4_root= (-(1- M_PHYTO)*(*phyto)+(sqrt(po4_delta)))/2/(*phyto);

    Gphyto = min(Gphyto, po4_root);
    }

    /* update of phytoplankton biomass */
    *phyto *= (1 - M_PHYTO + Gphyto);

    if(temp_no3_nh4 != 0)
    {
        *no3 -= Gphyto * (*phyto/CN_RATIO_PHYTO) * (*no3)/(temp_no3_nh4);
        *nh4 -= Gphyto * (*phyto/CN_RATIO_PHYTO) * (*nh4)/(temp_no3_nh4);
    }

    /* update of nutrient concentration in water */
    *po4 -= Gphyto * (*phyto/CP_RATIO_PHYTO);


    /* remove small negative values coming from calculus errors*/
    if (*no3 < 0 && *no3 > -epsilon) {*no3 =0;}
    if (*nh4 < 0 && *nh4 > -epsilon) {*nh4 =0;}
    if (*po4 < 0 && *po4 > -epsilon) {*po4 =0;}
}
