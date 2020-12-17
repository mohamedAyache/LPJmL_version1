
/* Module that handles the remineralisation in grass (replace the call to littersom)
 *
 * \author Nicolas Barrier
 * */

#include "lpj.h"
#include "grass.h"

/* Add the pft grass residues to the soil residue. Leaf residues are all put on
 * the first layer, while root residue are also put on the deeper layers.
 *
 * 08, section 1, equation 1
 *
 * Called in nit_remin_allpft
 *
 * @param Pft pft: pointer to the current grass PFT
 *
 * \author Nicolas Barrier
 */
void nit_remin_grass(Pft *pft,Real * annualResP, Real * annualResN)
{

    Pftgrass *pftgrass;
    Stand *stand;

    int l;

    Real totrootdist;

    Real residuerootg; // g
    Real residueleafg; // g

    stand = pft->stand;

    /* extract pft specific data */
    pftgrass = pft->data;

    //residue.root and residue.leaf are annual values
    //for the moment we simply divide by (365+12+1) to "convert to daily value. It is not perfect as it doesn't account
    //for monthly or daily changes in uptake.root and uptake.leaf though they should be minimal
    residuerootg =pftgrass->residue.root  ;
    residueleafg =pftgrass->residue.leaf  ;

    /* computes the total root distance */
    totrootdist = 0;
    forrootsoillayer(l)
        totrootdist += pft->par->rootdist[l];

    forrootsoillayer(l){
        if (l==0) {
            /* in surface layer, we add both root and leaf residues */
            stand->soil.RESC[l] += pft->par->rootdist[l]/totrootdist*residuerootg + residueleafg;
            stand->soil.RESN[l] += pft->par->rootdist[l]/totrootdist*residuerootg/RATIO_CN_ROOT + residueleafg/RATIO_CN_LEAF;
            stand->soil.RESP[l] += pft->par->rootdist[l]/totrootdist*residuerootg/RATIO_CP_ROOT + residueleafg/RATIO_CP_LEAF;


            annualResP[l]+=pft->par->rootdist[l]/totrootdist*residuerootg/RATIO_CP_ROOT + residueleafg/RATIO_CP_LEAF;
            annualResN[l]+=pft->par->rootdist[l]/totrootdist*residuerootg/RATIO_CN_ROOT + residueleafg/RATIO_CN_LEAF;
        }
        else {
            /* in deep layers, only root residues */
            stand->soil.RESC[l] += pft->par->rootdist[l]/totrootdist*residuerootg;
            stand->soil.RESN[l] += pft->par->rootdist[l]/totrootdist*residuerootg/RATIO_CN_ROOT;
            stand->soil.RESP[l] += pft->par->rootdist[l]/totrootdist*residuerootg/RATIO_CP_ROOT;

            annualResP[l]+=pft->par->rootdist[l]/totrootdist*residuerootg/RATIO_CP_ROOT;
            annualResN[l]+=pft->par->rootdist[l]/totrootdist*residuerootg/RATIO_CN_ROOT;
        }
    }
}
