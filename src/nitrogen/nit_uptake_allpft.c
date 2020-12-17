
#include "lpj.h"

/* Module that handles the uptake for all PFTs
 *
 * \author Nicolas Barrier
 *
 */

/* Uptakes over all the PFTs on the stand. Loops over all the PFTs contained on the stand, and which
 * call the uptake functions associated with the pft type. Take in the soil what the plants used to grow this time step
 *
 * Called in update_annual, update_monthly and update_daily (last routines called).
 *
 * @param Stand stand: pointer to the current stand
 *
 * \author Nicolas Barrier
 *
 */
void nit_uptake_allpft(Stand *stand,int day)
{

    int p,i;
    Pft *pft;

    foreachpft(pft, p, &stand->pftlist){
        if(pft->par->type==TREE){
            nit_uptake_tree(pft,day, stand->annualUptakeP,stand->annualUptakeN );
        }
        else if(pft->par->type==GRASS){
            nit_uptake_grass(pft,day,stand->annualUptakeP,stand->annualUptakeN);
        }
        else if(pft->par->type==CROP){
            nit_uptake_crop(pft,day,stand->annualUptakeP,stand->annualUptakeN);
        }
    } // end of PFT loop

    for(i=0; i<LASTLAYER; i++)
    {

        stand->soil.PO4[i]=max(stand->soil.PO4[i],0);
        stand->soil.NO3[i]=max(stand->soil.NO3[i],0);
        stand->soil.NH4[i]=max(stand->soil.NH4[i],0);

    }

}
/*
save the annual uptake values to split them into daily uptake
*/
void nit_save_annual_uptake_allpft(Stand *stand)
{
    int p;
    Pft *pft;

    foreachpft(pft, p, &stand->pftlist){
        if(pft->par->type==TREE){
            nit_save_annual_uptake_tree(pft);
        }
        else if(pft->par->type==GRASS){
            nit_save_annual_uptake_grass(pft);
        }
        else if(pft->par->type==CROP){
            nit_save_annual_uptake_crop(pft);
        }
    } // end of PFT loop
}

