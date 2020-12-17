
/* Module that handles the remineralisation for crops (replace the call to littersom) */

#include "lpj.h"
#include "crop.h"

/* Add the pft crop residues to the soil residue. Leaf and storage organ residues are all put on
 * the first layer, while root residue are also put on the deeper layers.
 *
 * 08, section 1, equation 1
 *
 * Called in nit_remin_allpft
 *
 * \author Nicolas Barrier
 */
void nit_remin_crop(Pft *pft,Real * annualResP, Real * annualResN)
{

    Pftcrop *pftcrop;
    Stand *stand;

    int l;

    Real totrootdist;

    stand = pft->stand;

    /* extract pft specific data */
    pftcrop = pft->data;

    /* computes the total root distance */
    totrootdist = 0;
    forrootsoillayer(l)
        totrootdist += pft->par->rootdist[l];

    forrootsoillayer(l){
        if (l==0) {
            /* on surface layer, we add the leaf and storing organs residues */
            stand->soil.RESC[l] += pft->par->rootdist[l]/totrootdist*pftcrop->residue.root + \
                                pftcrop->residue.leaf + \
                                pftcrop->residue.so;
            stand->soil.RESN[l] += pft->par->rootdist[l]/totrootdist*pftcrop->residue.root/RATIO_CN_ROOT + \
                                pftcrop->residue.leaf/RATIO_CN_LEAF + \
                                pftcrop->residue.so/RATIO_CN_STOR;
            stand->soil.RESP[l] += pft->par->rootdist[l]/totrootdist*pftcrop->residue.root/RATIO_CP_ROOT + \
                                   pftcrop->residue.leaf/RATIO_CP_LEAF + \
                                   pftcrop->residue.so/RATIO_CP_STOR;


            annualResP[l]+= pft->par->rootdist[l]/totrootdist*pftcrop->residue.root/RATIO_CP_ROOT + \
                                   pftcrop->residue.leaf/RATIO_CP_LEAF + \
                                   pftcrop->residue.so/RATIO_CP_STOR;

            annualResN[l]+=pft->par->rootdist[l]/totrootdist*pftcrop->residue.root/RATIO_CN_ROOT + \
                                pftcrop->residue.leaf/RATIO_CN_LEAF + \
                                pftcrop->residue.so/RATIO_CN_STOR;
        }
        else {
            /* on deep soil layers, only root residues are added */
            stand->soil.RESC[l] += pft->par->rootdist[l]/totrootdist*pftcrop->residue.root;
            stand->soil.RESN[l] += pft->par->rootdist[l]/totrootdist*pftcrop->residue.root/RATIO_CN_ROOT;
            stand->soil.RESP[l] += pft->par->rootdist[l]/totrootdist*pftcrop->residue.root/RATIO_CP_ROOT;


            annualResP[l]+=pft->par->rootdist[l]/totrootdist*pftcrop->residue.root/RATIO_CP_ROOT;
            annualResN[l]+=pft->par->rootdist[l]/totrootdist*pftcrop->residue.root/RATIO_CN_ROOT;
        }

    }
}
