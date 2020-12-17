
#include "lpj.h"

/* Module for the initialisation of nutrients concentration.
 * \author Nicolas Barrier
 */

/* Initialisation of nutrients in soil.
 *
 * Arguments are the stand pointer (for modification of the soil variable and access to the cell area)
 * and the pointer to the soil parameter (for access to the bulk density)
 *
 * Only NO3, PO4 and NH4 are initialised
 * Others soil variables are set to 0.0
 *
 * This routine is called in the "newgrid" routine, when natural stands are
 * initialised.
 *
 *
 * @param Stand stand: pointer to the current stand
 * @param Stand stand: constant pointer to the soil parameter array (for having the soil weights)
 *
 * \author Mohamed Ayache
 *
 * */
void nit_init_soil_nutrients(Stand *stand, const Soilpar *soilpar)

{
    Real weight; // soil weights
    Real surface;
    int l;  // vertical loop index

    surface= stand->frac *stand->cell->coord.area;

    forrootsoillayer(l)
    {

        // weights must be in kilos
        // area = m2, bulkdens=k/m3, soildepth=mm => conversion of mm into m
        weight = stand->cell->coord.area * soilpar->bulkdens[l] * soildepth[l] * 1e-3 * stand->frac;
        if (l==0)
        {
        stand->soil.NO3[l] = 6*1e-3*weight/surface;  // conversion from mg/m2 to g/m2
        stand->soil.PO4[l] = 15*1e-3*weight/surface;   // conversion from mg/m2 to g/m2
        stand->soil.PO4adsorbed[l] = 0;//weight * A * pow(stand->soil.PO4[l]*surface/10e8, B/A) /surface; // g/m2
        stand->soil.NH4[l] = 0*1e-3*weight/surface;   // conversion from mg/m2 to g/m2
        stand->soil.DOC[l] = 0.0;
        stand->soil.DON[l] = 0.0;
        stand->soil.DOP[l] = 0.0;
        stand->soil.DECC[l] = 0.0;
        stand->soil.RESC[l] =0.0;
        stand->soil.RESN[l] =0.0;
        stand->soil.RESP[l] = 0.0;
        }
        else{
        stand->soil.NO3[l] = 2*1e-3*weight/surface;  // conversion from mg/m2 to g/m2
        stand->soil.PO4[l] = 4*1e-3*weight/surface;   // conversion from mg/m2 to g/m2
        stand->soil.PO4adsorbed[l] = 0;//weight/surface * A * pow(stand->soil.PO4[l]*surface/10e8, B/A); // g/m2
        stand->soil.NH4[l] = 0*1e-3*weight/surface;   // conversion from mg/m2 to g/m2
        stand->soil.DOC[l] = 0.0;
        stand->soil.DON[l] = 0.0;
        stand->soil.DOP[l] = 0.0;
        stand->soil.DECC[l] = 0.0;
        stand->soil.RESC[l] =0.0;
        stand->soil.RESN[l] =0.0;
        stand->soil.RESP[l] = 0.0;
        }
    }
}
