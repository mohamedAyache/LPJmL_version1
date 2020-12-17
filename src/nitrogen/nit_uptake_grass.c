
#include "lpj.h"
#include "grass.h"

/* Nutrient uptake from the soil from pft grass. Assume that
 * leafs and only uptake from the first layer, while
 * roots may uptake from deeper layers.
 *
 * 07, section 1
 *
 * @param Pft pft: pointer to the current grass PFT
 *
 * \author Nicolas Barrier
 * \update Mohamed Ayache
 */
void nit_uptake_grass (Pft *pft, int day,Real * annualUptakeP, Real * annualUptakeN) /*, const Config *config)  */
{

    /* looping indexes */
    int l;

    /* pointers to specific pft data */
    Pftgrass *pftgrass;

    /* surface uptakes */
    Real surfuptn;
    Real surfuptpo4;
    /* total uptake */
    Real uptno3;
    Real uptpo4;
    Real uptnh4;

    Real soilno3;
    Real soilnh4;

    Real totrootdist;

    Stand *stand;

    Real uptakerootg; // g
    Real uptakeleafg; // g

    stand = (pft->stand);

    /* extraction of tree specific data */
    pftgrass = pft->data;

    //for the moment we simply divide by (365+12) to convert to "daily" value (the uptake is done once a day AND once a month) .
    uptakerootg =pftgrass->uptake.root ;
    uptakeleafg =pftgrass->uptake.leaf ;


    if (day > 91 && day <275)//spring summer
        {
            uptakerootg +=pftgrass->annual_uptake.root / (365+12) *1.9;
            uptakeleafg +=pftgrass->annual_uptake.leaf/ (365+12) *1.9;
    }
    else{

            uptakerootg +=pftgrass->annual_uptake.root / (365+12)*0.1;
            uptakeleafg +=pftgrass->annual_uptake.leaf / (365+12)*0.1;
    }



    /* determining the root weights */
    totrootdist = 0;
    forrootsoillayer(l)
        totrootdist += pft->par->rootdist[l];

    /* uptake through leaf */
    surfuptn = uptakeleafg/(RATIO_CN_LEAF) ;
    surfuptpo4 = uptakeleafg/(RATIO_CP_LEAF) ;

    /* soil uptake */
    forrootsoillayer(l){

        /* computes the N nutrients quantity in soil */
        soilno3 = stand->soil.NO3[l];
        soilnh4 = stand->soil.NH4[l];

        if (l==0){
            // on surface layer, we have the uptake from both the root and the leaf
            // check if N nutrients are available (prevents division by 0)
            if (soilno3 + soilnh4 == 0.0)
                uptno3 = uptnh4 = 0.0;
            else{
                uptno3 = (surfuptn + uptakerootg/(RATIO_CP_ROOT)*pft->par->rootdist[l]/totrootdist) ;//* soilno3/(soilno3+soilnh4) ;
                uptnh4 = (surfuptn + uptakerootg/(RATIO_CP_ROOT)*pft->par->rootdist[l]/totrootdist) * soilnh4/(soilno3+soilnh4) ;
            }
            uptpo4 = surfuptpo4 + uptakerootg/(RATIO_CP_ROOT)*pft->par->rootdist[l]/totrootdist ;

            /* insure that uptakes is positive and that soil nutrients is never negative
             */
            /*uptno3 = max(uptno3, 0);
            uptpo4 = max(uptpo4, 0);
            uptnh4 = max(uptnh4, 0);*/
            if ((uptno3<0 && uptno3>10e-20) || ( uptnh4<0 && uptnh4>10e-20) || ( uptpo4<0 && uptpo4>10e-20)) {fputs("WARNING: grass no3, po4 or nh4 uptake is negative\n",stderr);}
            if ((uptno3>stand->soil.NO3[l] || uptnh4>stand->soil.NH4[l] || uptpo4> stand->soil.PO4[l]) &&  pft->stand->cell->coord.lat< 43.5)
                {
                    if (uptno3>stand->soil.NO3[l]) printf("WARNING: grass NO3 uptake is bigger than what is available in layer %d uptake %e soil %e\n", l,uptno3,stand->soil.NO3[l]);
                    if (uptnh4>stand->soil.NH4[l]) printf("WARNING: grass NH4 uptake is bigger than what is available in layer %d uptake %e soil %e\n", l,uptnh4,stand->soil.NH4[l]);
                    if (uptpo4>stand->soil.PO4[l]) printf("WARNING: grass PO4 uptake is bigger than what is available in layer %d uptake %e soil %e\n", l,uptpo4,stand->soil.PO4[l]);
                puts("WARNING: grass no3, po4 or nh4 uptake is bigger than what is available\n",stderr);

                }

            stand->soil.NO3[l] -= uptno3;
            stand->soil.PO4[l] -= uptpo4;
            stand->soil.NH4[l] -= uptnh4;

            annualUptakeP[l]+=uptpo4;
            annualUptakeN[l]+=uptnh4+uptno3;

       }
        else{
            // on deep layers, we have only uptake from the roots
            // check if N nutrients are available (prevents division by 0)
            if (soilno3 + soilnh4 == 0.0)
                // if not, no N uptake
                uptno3 = uptnh4 = 0.0;
            else{
                uptno3 = (uptakerootg/(RATIO_CN_ROOT)*pft->par->rootdist[l]/totrootdist) ;//* soilno3/(soilno3+soilnh4) ;
                uptnh4 = (uptakerootg/(RATIO_CN_ROOT)*pft->par->rootdist[l]/totrootdist) * soilnh4/(soilno3+soilnh4);
            }
            uptpo4 = uptakerootg/(RATIO_CP_ROOT)*pft->par->rootdist[l]/totrootdist ;

            /* insure that uptakes is positive and that soil nutrients is never negative
             */
            /*uptno3 = max(uptno3, 0);
            uptpo4 = max(uptpo4, 0);
            uptnh4 = max(uptnh4, 0);*/
            if ((uptno3<0 && uptno3>10e-20) || ( uptnh4<0 && uptnh4>10e-20) || ( uptpo4<0 && uptpo4>10e-20)) {fputs("WARNING: grass no3, po4 or nh4 uptake is negavite\n",stderr);
            if ((uptno3>stand->soil.NO3[l] || uptnh4>stand->soil.NH4[l] || uptpo4> stand->soil.PO4[l] )&&  pft->stand->cell->coord.lat< 43.5)
            {
                    if (uptno3>stand->soil.NO3[l]) printf("WARNING: grass NO3 uptake is bigger than what is available in layer %d uptake %e soil %e\n", l,uptno3,stand->soil.NO3[l]);
                    if (uptnh4>stand->soil.NH4[l]) printf("WARNING: grass NH4 uptake is bigger than what is available in layer %d uptake %e soil %e\n", l,uptnh4,stand->soil.NH4[l]);
                    if (uptpo4>stand->soil.PO4[l]) printf("WARNING: grass PO4 uptake is bigger than what is available in layer %d uptake %e soil %e\n", l,uptpo4,stand->soil.PO4[l]);

                    //fputs("WARNING: grass no3, po4 or nh4 uptake is bigger than what is available \n",stderr);
            }

            }

            stand->soil.NO3[l] -= uptno3;
            stand->soil.PO4[l] -= uptpo4;
            stand->soil.NH4[l] -= uptnh4;

            annualUptakeP[l]+=uptpo4;
            annualUptakeN[l]+=uptnh4+uptno3;
        }
    } /* end of soil layer loop */
} /*end of function */
void nit_save_annual_uptake_grass (Pft *pft)
{
    /* pointers to specific pft data */
    Pftgrass *pftgrass;

    /* extraction of tree specific data */
    pftgrass = pft->data;

    /* correction by multiplication by the surface */
    pftgrass->annual_uptake.root =pftgrass->uptake.root;
    pftgrass->annual_uptake.leaf =pftgrass->uptake.leaf;

} /*end of function */
