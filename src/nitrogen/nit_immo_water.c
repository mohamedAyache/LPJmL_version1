#include "lpj.h"

/* Decomposition in water (08, section 3)
 *
 * \author Nicolas Barrier
 *
 */

/* Decomposition in water (08, section 3).
 *
 * Inputs are pointers to water relative variables, which are updated in the function
 *
 * Called in "nit_update_river" and "nit_update_lakes"
 *
 * @param Cell cell: constant pointer to the current cell
 * @param Real phyto: phytoplankton C content in the water (in g)
 * @param Real resw: pointer to the residue C content in water (in g)
 * @param Real decc: pointer to the decomposer C content in water (in g)
 * @param Real no3: pointer to the NO3 content in water (in g)
 * @param Real nh4: pointer to the NH4 content in water (in g)
 * @param Real po4: pointer to the PO4 content in water (in g)
 *
 * \author Nicolas Barrier
 *
 */
void nit_immo_water(const Cell *cell, const Real phyto, Real *resw, Real *decc,
                    Real *no3, Real *nh4, Real *po4, const Real vwat)

{
    Real gdec_wat;  //daily consumption of carbon residues by water decomposers (g)

    if(vwat == 0)
        return;

    *resw += M_PHYTO * phyto;
    *resw *= (1 - SED);

    gdec_wat = BETA_RES_WATER * nit_tempdep(cell->water_temp, 24.0, 14.0) * (*resw);

    *resw -= gdec_wat;

    *decc = *decc * (1 - M_PHYTO) + gdec_wat ;

    *nh4 += M_PHYTO * (*decc/CN_RATIO_PHYTO);
    *po4 += M_PHYTO * (*decc/CP_RATIO_PHYTO);
}
