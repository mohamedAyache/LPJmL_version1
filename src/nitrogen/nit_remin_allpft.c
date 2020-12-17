
#include "lpj.h"

/* Remineralisation of all the PFTs/CFTs/Ag trees contained on the current cell. It loops over all the PFTs contained on the stand, and which
 * call the remin functions associated with the pft type. Put the residue created by the "death" of plants this time step into the soil
 *
 * Called in update_annual, update_monthly and update_daily (last routines called).
 *
 * @param Stand stand: pointer to the current stand
 *
 * \author Nicolas Barrier
 *
 */
void nit_remin_allpft(Stand *stand)
{
    int p;
    Pft *pft;


    foreachpft(pft, p, &stand->pftlist){
        if(pft->par->type==TREE){
            nit_remin_tree(pft,stand->annualResP,stand->annualResN);
        }
        else if(pft->par->type==GRASS){
            nit_remin_grass(pft,stand->annualResP,stand->annualResN);
        }
        else if(pft->par->type==CROP){
            nit_remin_crop(pft,stand->annualResP,stand->annualResN);
        }
    } // end of PFT loop


}

