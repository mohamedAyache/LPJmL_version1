
#include "lpj.h"

/* Function that mixes the soil nitrogen of two setaside stands
 *
 *
 * Called in "mixsoil" (itself called in mixsetaside)
 *
 * @param Stand stand1: pointer to the input stand (recipient)
 * @param Stand stand2: constant pointer to the second stand
 *
 * \author Nicolas Barrier
 *
 */
void nit_mix_setaside(Stand *stand1, const Stand *stand2)
{
    int l;

    forrootsoillayer(l)
    {
        mixpool(stand1->soil.NO3[l], stand2->soil.NO3[l], stand1->frac, stand2->frac);
        mixpool(stand1->soil.NH4[l], stand2->soil.NH4[l], stand1->frac, stand2->frac);
        mixpool(stand1->soil.PO4[l], stand2->soil.PO4[l], stand1->frac, stand2->frac);
        mixpool(stand1->soil.PO4adsorbed[l], stand2->soil.PO4adsorbed[l], stand1->frac, stand2->frac);
        mixpool(stand1->soil.DOC[l], stand2->soil.DOC[l], stand1->frac, stand2->frac);
        mixpool(stand1->soil.DON[l], stand2->soil.DON[l], stand1->frac, stand2->frac);
        mixpool(stand1->soil.DOP[l], stand2->soil.DOP[l], stand1->frac, stand2->frac);
        mixpool(stand1->soil.DECC[l], stand2->soil.DECC[l], stand1->frac, stand2->frac);
        mixpool(stand1->soil.RESC[l], stand2->soil.RESC[l], stand1->frac, stand2->frac);
        mixpool(stand1->soil.RESN[l], stand2->soil.RESN[l], stand1->frac, stand2->frac);
        mixpool(stand1->soil.RESP[l], stand2->soil.RESP[l], stand1->frac, stand2->frac);
    }
}

