
#include "lpj.h"
#include "tree.h"

/* Nutrient uptake from the soil from pft trees. Assume that
 * leafs and sapwood only uptake from the first layer, while
 * roots may uptake from deeper layers.
 *
 * 07, section 1
 *
 * @param Pft pft: pointer to the current tree PFT.
 *
 * \author Nicolas Barrier
 * \Updat Mohamed Ayache
 */
void nit_uptake_tree (Pft *pft, int day,Real * annualUptakeP, Real * annualUptakeN) /*, const Config *config)  */
{

    /* looping indexes */
    int l;

    /* pointers to specific pft data */
    Pfttree *pfttree;

    // Surface uptakes of NO3 and PO4
    Real surfuptn;
    Real surfuptpo4;

    // Total uptake of NO3, PO4 and NH4
    Real uptno3;
    Real uptpo4;
    Real uptnh4;

    // Soil content in NO3 and PO4
    Real soilno3;
    Real soilnh4;

    Real totrootdist;

    Stand *stand;

    Real uptakerootg; // g
    Real uptakeleafg; // g
    Real uptakesapwoodg; //g

    // stabd associated with the pft
    stand = (pft->stand);

    /* extraction of tree specific data */
    pfttree = pft->data;

    //for the moment we simply divide by (365+12) to convert to daily value.
    uptakerootg = pfttree->uptake.root  ;
    uptakeleafg = pfttree->uptake.leaf  ;
    uptakesapwoodg = pfttree->uptake.sapwood ;

    if (day > 91 && day <275)//spring summer
    {
            uptakerootg +=pfttree->annual_uptake.root / (365+12) *1.9;
            uptakeleafg +=pfttree->annual_uptake.leaf / (365+12) *1.9;
            uptakesapwoodg+=pfttree->annual_uptake.sapwood  /(365+12)*1.9;
    }
    else{

            uptakerootg +=pfttree->annual_uptake.root / (365+12)*0.1;
            uptakeleafg +=pfttree->annual_uptake.leaf / (365+12)*0.1;
            uptakesapwoodg+=pfttree->annual_uptake.sapwood  /(365+12)*0.1;
    }

    /* determining the root weights */
    totrootdist = 0;
    forrootsoillayer(l)
        totrootdist += pft->par->rootdist[l];

    /* uptake through leaf and sapwood */
    surfuptn = uptakeleafg/(RATIO_CN_LEAF) + uptakesapwoodg/(RATIO_CN_SAP) ;
    surfuptpo4 = uptakeleafg/(RATIO_CP_LEAF) + uptakesapwoodg/(RATIO_CP_SAP) ;

    /* soil uptake */
    forrootsoillayer(l){

        /* computes the N nutrients quantity in soil */
        soilno3 = stand->soil.NO3[l];
        soilnh4 = stand->soil.NH4[l];

        if (l==0){
            // in the surface layer, uptake through leaf, sapwood and root
            // check that we have N in soils
            // (prevents division by 0)
            if(soilno3 + soilnh4 == 0.0){
                // if no N, NO3upt = NH4upt = 0
                uptno3 = 0.0;
                uptnh4 = 0.0;
            }
            else{
                // uptake on the first layer is the sum of surface uptake and root uptake
                uptno3 = (surfuptn + uptakerootg/(RATIO_CN_ROOT)*pft->par->rootdist[l]/totrootdist);// * soilno3/(soilno3+soilnh4) ;
                uptnh4 = (surfuptn + uptakerootg/(RATIO_CN_ROOT)*pft->par->rootdist[l]/totrootdist) * soilnh4/(soilno3+soilnh4) ;
            }
            // same thing for PO4, except that no risk of division by 0.
            uptpo4 = surfuptpo4 + uptakerootg/(RATIO_CP_ROOT)*pft->par->rootdist[l]/totrootdist ;

            // insure positive uptakes
            /*uptno3 = max(uptno3, 0);
            uptpo4 = max(uptpo4, 0);
            uptnh4 = max(uptnh4, 0);*/
            if ((uptno3<0 && uptno3>10e-20) || ( uptnh4<0 && uptnh4>10e-20) || ( uptpo4<0 && uptpo4>10e-20)) {fputs("WARNING: tree no3, po4 or nh4 uptake is negative\n",stderr); printf("uptno3 %e uptnh4 %e uptpo4 %e\n", uptno3, uptnh4, uptpo4);}//mouginot

            if ((uptno3>stand->soil.NO3[l] || uptnh4>stand->soil.NH4[l] || uptpo4> stand->soil.PO4[l]) &&  pft->stand->cell->coord.lat< 43.5)
                {
                    if (uptno3>stand->soil.NO3[l]) printf("WARNING: tree NO3 uptake is bigger than what is available in layer %d uptake %e soil %e\n", l,uptno3,stand->soil.NO3[l]);
                    if (uptnh4>stand->soil.NH4[l]) printf("WARNING: tree NH4 uptake is bigger than what is available in layer %d uptake %e soil %e\n", l,uptnh4,stand->soil.NH4[l]);
                    if (uptpo4>stand->soil.PO4[l]) printf("WARNING: tree PO4 uptake is bigger than what is available in layer %d uptake %e soil %e\n", l,uptpo4,stand->soil.PO4[l]);
                    fputs("WARNING: tree no3, po4 or nh4 uptake is bigger than what is available\n",stderr);
                }


            stand->soil.NO3[l] -= uptno3;
            stand->soil.PO4[l] -= uptpo4;
            stand->soil.NH4[l] -= uptnh4;

            annualUptakeP[l]+=uptpo4;
            annualUptakeN[l]+=uptnh4+uptno3;

        }
        else{
            // in the deep layers, only uptake through roots
            // check that we have N in soils
            // (prevents division by 0)
            if(soilno3 + soilnh4 == 0.0){
                // if no N, N upt = 0
                uptno3 = 0.0;
                uptnh4 = 0.0;
            }
            else{
                uptno3 = (uptakerootg/(RATIO_CN_ROOT)*pft->par->rootdist[l]/totrootdist) ;//* soilno3/(soilno3+soilnh4) ;
                uptnh4 = (uptakerootg/(RATIO_CN_ROOT)*pft->par->rootdist[l]/totrootdist) * soilnh4/(soilno3+soilnh4);
            }
            uptpo4 = uptakerootg/(RATIO_CP_ROOT)*pft->par->rootdist[l]/totrootdist ;
            /* insure positive uptake and not negative soil nutrient concentrations */
            /*uptno3 = max(uptno3, 0);
            uptpo4 = max(uptpo4, 0);
            uptnh4 = max(uptnh4, 0);*/
            if ((uptno3<0 && uptno3>10e-20) || ( uptnh4<0 && uptnh4>10e-20) || ( uptpo4<0 && uptpo4>10e-20)) {fputs("WARNING: tree no3, po4 or nh4 uptake is negavite\n",stderr);printf("uptno3 %e uptnh4 %e uptpo4 %e\n", uptno3, uptnh4, uptpo4);}//mouginot

            if ((uptno3>stand->soil.NO3[l] || uptnh4>stand->soil.NH4[l] || uptpo4> stand->soil.PO4[l]) &&  pft->stand->cell->coord.lat< 43.5)
                {
                   if (uptno3>stand->soil.NO3[l]) printf("WARNING: tree NO3 uptake is bigger than what is available in layer %d uptake %e soil %e\n", l,uptno3,stand->soil.NO3[l]);
                    if (uptnh4>stand->soil.NH4[l]) printf("WARNING: tree NH4 uptake is bigger than what is available in layer %d uptake %e soil %e\n", l,uptnh4,stand->soil.NH4[l]);
                    if (uptpo4>stand->soil.PO4[l]) printf("WARNING: tree PO4 uptake is bigger than what is available in layer %d uptake %e soil %e\n", l,uptpo4,stand->soil.PO4[l]);
                    fputs("WARNING: tree no3, po4 or nh4 uptake is bigger than what is available\n",stderr);
                }

            stand->soil.NO3[l] -= uptno3;
            stand->soil.PO4[l] -= uptpo4;
            stand->soil.NH4[l] -= uptnh4;

            annualUptakeP[l]+=uptpo4;
            annualUptakeN[l]+=uptnh4+uptno3;

        }

    } /* end of soil layer loop */

} /*end of function */
/*
save the annual uptake so that it can be split into daily uptake
*/
void nit_save_annual_uptake_tree (Pft *pft)
{

   /* pointers to specific pft data */
    Pfttree *pfttree;

    /* extraction of tree specific data */
    pfttree = pft->data;

    pfttree->annual_uptake.root = pfttree->uptake.root ;
    pfttree->annual_uptake.leaf = pfttree->uptake.leaf ;
    pfttree->annual_uptake.sapwood = pfttree->uptake.sapwood ;

} /*end of function */
