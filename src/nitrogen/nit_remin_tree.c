
/* Module that handles the remineralisation in trees (replace the call to littersom)
 *
 * \author Nicolas Barrier
 *
 * */

#include "lpj.h"
#include "tree.h"

/* Add the pft tree residues to the soil residue. Wood and leaf residues are all put on
 * the first layer, while root residue are also put on the deeper layers.
 *
 * 08, section 1, equation 1
 *
 * Called in nit_remin_allpft
 *
 * @param Pft pft: pointer to the current tree PFT
 *
 * \author Nicolas Barrier
 */
void nit_remin_tree(Pft *pft,Real * annualResP, Real * annualResN)
{
    Pfttree *pfttree;
    Stand *stand;

    int l;

    Real totrootdist;

    Real residuerootg; // g
    Real residueleafg; // g
    Real residuesapwoodg; //g


    stand = pft->stand;

    /* extract pft specific data */
    pfttree = pft->data;

    //residue.root and residue.leaf and residue.sapwood are annual values
    residuerootg = pfttree->residue.root;
    residueleafg = pfttree->residue.leaf ;
    residuesapwoodg = pfttree->residue.sapwood ;

    /* computes the total root distance */
    totrootdist = 0;
    forrootsoillayer(l)
        totrootdist += pft->par->rootdist[l];

    forrootsoillayer(l){
        if (l==0) {
            /* on surface layer, we add the leaf, root and sapwood residues */
            stand->soil.RESC[l] += pft->par->rootdist[l]/totrootdist*residuerootg + residueleafg + residuesapwoodg;
            stand->soil.RESN[l] += pft->par->rootdist[l]/totrootdist*residuerootg/RATIO_CN_ROOT + residueleafg/RATIO_CN_LEAF + residuesapwoodg/RATIO_CN_SAP;
            stand->soil.RESP[l] += pft->par->rootdist[l]/totrootdist*residuerootg/RATIO_CP_ROOT + residueleafg/RATIO_CP_LEAF + residuesapwoodg/RATIO_CP_SAP;

            annualResP[l]+=pft->par->rootdist[l]/totrootdist*residuerootg/RATIO_CP_ROOT + residueleafg/RATIO_CP_LEAF + residuesapwoodg/RATIO_CP_SAP;
            annualResN[l]+=pft->par->rootdist[l]/totrootdist*residuerootg/RATIO_CN_ROOT + residueleafg/RATIO_CN_LEAF + residuesapwoodg/RATIO_CN_SAP;
        }
        else {
            /* on deep layers, we add only the sapwood residues */
            stand->soil.RESC[l] += pft->par->rootdist[l]/totrootdist*residuerootg;
            stand->soil.RESN[l] += pft->par->rootdist[l]/totrootdist*residuerootg/RATIO_CN_ROOT;
            stand->soil.RESP[l] += pft->par->rootdist[l]/totrootdist*residuerootg/RATIO_CP_ROOT;


            annualResP[l]+=pft->par->rootdist[l]/totrootdist*residuerootg/RATIO_CP_ROOT;
            annualResN[l]+=pft->par->rootdist[l]/totrootdist*residuerootg/RATIO_CN_ROOT;
        }
    }
}
