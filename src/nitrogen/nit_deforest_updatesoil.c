
#include "lpj.h"

/* Updates the nutrient quantity in the soil of newstand and natstand, when the newstand is
 * created from natstand.
 *
 * If diffrac is the fraction of natural stand that goes into newstand, and if NO3nat is the nutrient
 * quantity contained in the stand, then NO3new = diffrac * NO3nat / natfrac, and NO3nat -= diffrac * NO3nat / natfrac.
 *
 * Called in "deforest" (landusechange.c)
 *
 * @param Stand natstand: pointer to the natural stand
 * @param Stand newstand: pointer to the newly created stand
 * @param Real diffrac: natural stand fraction that will
 * be used to create the new stand
 *
 * \author Nicolas Barrier
 */
void nit_deforest_updatesoil(Stand *natstand, Stand *newstand, Real diffrac)
{

    int l;

    if(diffrac+epsilon>=natstand->frac)
    {
        forrootsoillayer(l)
        {
            // if all the natural stand disappears, all the nutrients go from the natural stand
            // to the newly created stand. the natural stand will be removed anyway
            newstand->soil.NO3[l] = natstand->soil.NO3[l];
            newstand->soil.NH4[l] = natstand->soil.NH4[l];
            newstand->soil.PO4[l] = natstand->soil.PO4[l];
            newstand->soil.DOC[l] = natstand->soil.DOC[l];
            newstand->soil.DON[l] = natstand->soil.DON[l];
            newstand->soil.DOP[l] = natstand->soil.DOP[l];
            newstand->soil.DECC[l] = natstand->soil.DECC[l];
            newstand->soil.RESC[l] = natstand->soil.RESC[l];
            newstand->soil.RESN[l] = natstand->soil.RESN[l];
            newstand->soil.RESP[l] = natstand->soil.RESP[l];
            natstand->soil.NO3[l] = 0.0;
            natstand->soil.NH4[l] = 0.0;
            natstand->soil.PO4[l] = 0.0;
            natstand->soil.DOC[l] = 0.0;
            natstand->soil.DON[l] = 0.0;
            natstand->soil.DOP[l] = 0.0;
            natstand->soil.DECC[l] = 0.0;
            natstand->soil.RESC[l] = 0.0;
            natstand->soil.RESN[l] = 0.0;
            natstand->soil.RESP[l] = 0.0;
        }
    }
    else{
        forrootsoillayer(l)
        {
            // if the natural stand still exists at the end, then we
            // update the fraction of the natural and new stand nutrients
            // concentration
            newstand->soil.NO3[l] = diffrac * natstand->soil.NO3[l];
            natstand->soil.NO3[l] -= newstand->soil.NO3[l];
            newstand->soil.PO4[l] = diffrac * natstand->soil.PO4[l];
            natstand->soil.PO4[l] -= newstand->soil.PO4[l];
            newstand->soil.NH4[l] = diffrac * natstand->soil.NH4[l];
            natstand->soil.NH4[l] -= newstand->soil.NH4[l];
            newstand->soil.DOC[l] = diffrac * natstand->soil.DOC[l];
            natstand->soil.DOC[l] -= newstand->soil.DOC[l];
            newstand->soil.DON[l] = diffrac * natstand->soil.DON[l];
            natstand->soil.DON[l] -= newstand->soil.DON[l];
            newstand->soil.DOP[l] = diffrac * natstand->soil.DOP[l];
            natstand->soil.DOP[l] -= newstand->soil.DOP[l];
            newstand->soil.DECC[l] = diffrac * natstand->soil.DECC[l];
            natstand->soil.DECC[l] -= newstand->soil.DECC[l];
            newstand->soil.RESC[l] = diffrac * natstand->soil.RESC[l];
            natstand->soil.RESC[l] -= newstand->soil.RESC[l];
            newstand->soil.RESN[l] = diffrac * natstand->soil.RESN[l];
            natstand->soil.RESN[l] -= newstand->soil.RESN[l];
            newstand->soil.RESP[l] = diffrac * natstand->soil.RESP[l];
            natstand->soil.RESP[l] -= newstand->soil.RESP[l];
        }
    } //end of if statement
}  // end of function
