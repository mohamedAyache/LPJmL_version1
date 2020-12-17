
#include "lpj.h"
#include "grass.h"
#include "tree.h"
#include "crop.h"

/* Reset of the "uptake" attribute associated with all daily pft (crop).
 *
 * This routine is called at the beginning of the update_annual, update_monthly and update_daily routines
 *
 * @param Stand stand: pointer to the current stand
 *
 * \author Nicolas Barrier
 * \Update Mohamed Ayache
 */
void nit_init_uptake_daily(Stand *stand)
{

    int p;
    Pft *pft;
    Pfttree *pfttree;
    Pftgrass *pftgrass;
    Pftcrop *pftcrop;
    foreachpft(pft, p, &stand->pftlist){
        if(pft->par->type==TREE){
            pfttree = pft->data;
            pfttree->uptake.root = pfttree->uptake.sapwood = pfttree->uptake.leaf=0.0;
        }
        else if(pft->par->type==GRASS){
            pftgrass = pft->data;
            pftgrass->uptake.root = pftgrass->uptake.leaf=0.0;
        }
        else if(pft->par->type==CROP){
            pftcrop = pft->data;
            pftcrop->uptake.root = pftcrop->uptake.so = pftcrop->uptake.leaf=0.0;
        }
    } // end of PFT loop
}


/* Reset of the "uptake" attribute associated with all annual (uptake is calculated once a year) PFT types (grass tree).
 *
 * This routine is called at the beginning of the update_annual
 *
 * @param Stand stand: pointer to the current stand
 *
 * \author Nicolas Barrier
 */
void nit_init_uptake_annual(Stand *stand)
{

    int p;
    Pft *pft;
    Pfttree *pfttree;
    Pftgrass *pftgrass;
    Pftcrop *pftcrop;
    foreachpft(pft, p, &stand->pftlist){
        if(pft->par->type==TREE){
            pfttree = pft->data;
            pfttree->uptake.root = pfttree->uptake.sapwood = pfttree->uptake.leaf=0.0;
            pfttree->annual_uptake.root = pfttree->annual_uptake.sapwood = pfttree->annual_uptake.leaf=0.0;
        }
        else if(pft->par->type==GRASS){
            pftgrass = pft->data;
            pftgrass->uptake.root = pftgrass->uptake.leaf=0.0;
             pftgrass->annual_uptake.root = pftgrass->annual_uptake.leaf=0.0;
        }
        else if(pft->par->type==CROP){
            pftcrop = pft->data;
            pftcrop->uptake.root = pftcrop->uptake.so = pftcrop->uptake.leaf=0.0;
            pftcrop->annual_uptake.root = pftcrop->annual_uptake.so = pftcrop->annual_uptake.leaf=0.0;
        }
    } // end of PFT loop
}
/* Reset of the "residue" attribute associated with all PFT types (set them equals to 0).
 *
 * This routine is called at the beginning of the update_monthly and update_daily routines
 *
 * @param Stand stand: pointer to the current stand
 *
 * \author Nicolas Barrier
 */
void nit_init_residue_daily(Stand *stand)
{

    int p;
    Pft *pft;
    Pfttree *pfttree;
    Pftgrass *pftgrass;
    Pftcrop *pftcrop;
    foreachpft(pft, p, &stand->pftlist){
        if(pft->par->type==TREE){

            pfttree = pft->data;
            pfttree->residue.root = pfttree->residue.sapwood = pfttree->residue.leaf=0.0;
        }
        else if(pft->par->type==GRASS){

            pftgrass = pft->data;
            pftgrass->residue.root = pftgrass->residue.leaf=0.0;

        }
        else if(pft->par->type==CROP){
            pftcrop = pft->data;
            pftcrop->residue.root = pftcrop->residue.so = pftcrop->residue.leaf=0.0;
        }
    } // end of PFT loop
} // end of nit_init_residue

/* Reset of the "residue" attribute associated with all PFT types (set them equals to 0).
 *
 * This routine is called at the beginning of the update_annual routines
 *
 * @param Stand stand: pointer to the current stand
 *
 * \author Nicolas Barrier

 //do the same thing as the function above remove if not necessary
 */
void nit_init_residue_annual(Stand *stand)
{

    int p;
    Pft *pft;
    Pfttree *pfttree;
    Pftgrass *pftgrass;
    Pftcrop *pftcrop;
    foreachpft(pft, p, &stand->pftlist){
        if(pft->par->type==TREE){
            pfttree = pft->data;
            pfttree->residue.root = pfttree->residue.sapwood = pfttree->residue.leaf=0.0;
        }
        else if(pft->par->type==GRASS){
            pftgrass = pft->data;
            pftgrass->residue.root = pftgrass->residue.leaf=0.0;
        }
        else if(pft->par->type==CROP){
            pftcrop = pft->data;
            pftcrop->residue.root = pftcrop->residue.so = pftcrop->residue.leaf=0.0;
        }
    } // end of PFT loop
} // end of nit_init_residue
