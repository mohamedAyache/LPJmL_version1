/*

   Daily uptake of Dissolved Inorganic Nitrogen and Phosphoreous by crops

   \author Nicolas Barrier

*/

#include "lpj.h"
#include "crop.h"

/* Nutrient uptake from the soil from pft crop. Assume that
 * leafs and storing organs only uptake from the first layer, while
 * roots may uptake from deeper layers.
 *
 * Note that we assume that PULSES and SOIL_CROPS_SOYBEAN do
 * not uptake nitrogens!
 *
 * 07, section 1
 *
 * @param Pft pft: pointer to the Pft object
 *
 * \author Nicolas Barrier
 * 
 * \update Mohamed Ayache
 */
void nit_uptake_crop(Pft *pft,int day, Real * annualUptakeP, Real * annualUptakeN)
{

    /* looping indexes */
    int l;

    /* pointers to specific pft data */
    Pftcrop *pftcrop;

    /* surface uptakes */
    Real surfuptno3;
    Real surfuptpo4;
    /* total uptakes */
    Real uptno3;
    Real uptpo4;
    Real uptnh4;

    Real soilno3;
    Real soilnh4;

    Real totrootdist;

    Stand *stand;

    Real uptakerootg; // g/m2
    Real uptakeleafg; // g/m2
    Real uptakesog; //g/m2

    stand = pft->stand;

    /* extraction of crop specific data */
    pftcrop = pft->data;


    uptakerootg = pftcrop->uptake.root ;
    uptakeleafg = pftcrop->uptake.leaf ;
    uptakesog = pftcrop->uptake.so ;

    if (day > 91 && day <275)//spring summer
    {
            uptakerootg +=pftcrop->annual_uptake.root / (365+12) *1.9;
            uptakeleafg +=pftcrop->annual_uptake.leaf/ (365+12) *1.9;
            uptakesog+=pftcrop->annual_uptake.so/ (365+12) *1.9;
    }
    else{

            uptakerootg +=pftcrop->annual_uptake.root / (365+12)*0.1;
            uptakeleafg +=pftcrop->annual_uptake.leaf / (365+12)*0.1;
            uptakesog+=pftcrop->annual_uptake.so/(365+12)*0.1;;
    }

    /* determination of root distance (for weighting root uptakes) */
    totrootdist = 0;
    forrootsoillayer(l)
        totrootdist += pft->par->rootdist[l];

    /* if no pulses or soybean, then N uptake */
    if((pft->par->id != PULSES) && (pft->par->id != OIL_CROPS_SOYBEAN)) {

        /* surface uptake (leafs and storage) */
        /* last terms in equation 1
         * no need for surfuptnh4 since the terms within the brackets is the same as for NO3*/
        surfuptno3 = uptakeleafg/(RATIO_CN_LEAF) + uptakesog/(RATIO_CN_STOR);
        surfuptpo4 = uptakeleafg/(RATIO_CP_LEAF) + uptakesog/(RATIO_CP_STOR);

        forrootsoillayer(l){

            soilno3 = stand->soil.NO3[l];
            soilnh4 = stand->soil.NH4[l];

            if (l==0){

                // check if N is present in the soil.
                // if not, no uptake
                if (soilno3 + soilnh4 == 0.0)
                    uptno3 = uptnh4 = 0.0;
                else{
                    // equation 1
                    uptno3 = (surfuptno3 + uptakerootg/(RATIO_CN_ROOT)*pft->par->rootdist[l]/totrootdist) * soilno3/(soilno3+soilnh4) ;
                    uptnh4 = 0;//(surfuptno3 + uptakerootg/(RATIO_CN_ROOT)*pft->par->rootdist[l]/totrootdist) * soilnh4/(soilno3+soilnh4) ;
                }
                uptpo4 = surfuptpo4 + uptakerootg/(RATIO_CP_ROOT)*pft->par->rootdist[l]/totrootdist;

                if ((uptno3<0 && uptno3>10e-20) || ( uptnh4<0 && uptnh4>10e-20) || ( uptpo4<0 && uptpo4>10e-20)) {fputs("WARNING: crop no3, po4 or nh4 uptake is negavite\n",stderr);}


                if ((uptno3>stand->soil.NO3[l] || uptnh4>stand->soil.NH4[l] || uptpo4> stand->soil.PO4[l]) &&  pft->stand->cell->coord.lat< 43.5)
                {
                    if (uptno3>stand->soil.NO3[l]) printf("WARNING: crop NO3 uptake is bigger than what is available in layer %d uptake %e soil %e\n", l,uptno3,stand->soil.NO3[l]);
                    if (uptnh4>stand->soil.NH4[l]) printf("WARNING: crop NH4 uptake is bigger than what is available in layer %d uptake %e soil %e\n", l,uptnh4,stand->soil.NH4[l]);
                    if (uptpo4>stand->soil.PO4[l]) printf("WARNING: crop PO4 uptake is bigger than what is available in layer %d uptake %e soil %e\n", l,uptpo4,stand->soil.PO4[l]);
                    //fputs("WARNING: tree no3, po4 or nh4 uptake is bigger than what is available\n",stderr);
                }

                stand->soil.NO3[l] -= uptno3;
                stand->soil.PO4[l] -= uptpo4;
                stand->soil.NH4[l] -= uptnh4;

                annualUptakeP[l]+=uptpo4;
                annualUptakeN[l]+=uptnh4+uptno3;

            }
            else{
                // if not the surface layer, no consideration of the surfuptake
                // check if N is present in the soil.
                // if not, no uptake
                if (soilno3 + soilnh4 == 0.0)
                    uptno3 = uptnh4 = 0.0;
                else{
                    uptno3 = (uptakerootg/(RATIO_CN_ROOT)*pft->par->rootdist[l]/totrootdist) * soilno3/(soilno3+soilnh4) ;
                    uptnh4 = 0; //(uptakerootg/(RATIO_CN_ROOT)*pft->par->rootdist[l]/totrootdist) * soilnh4/(soilno3+soilnh4);
                }
                uptpo4 = uptakerootg/(RATIO_CP_ROOT)*pft->par->rootdist[l]/totrootdist ;

                if ((uptno3<0 && uptno3>10e-20) || ( uptnh4<0 && uptnh4>10e-20) || ( uptpo4<0 && uptpo4>10e-20)) {fputs("WARNING: crop no3, po4 or nh4 uptake is negavite\n",stderr);}


                if ((uptno3>stand->soil.NO3[l] || uptnh4>stand->soil.NH4[l] || uptpo4> stand->soil.PO4[l]) &&  pft->stand->cell->coord.lat< 43.5)
                {
                    if (uptno3>stand->soil.NO3[l]) printf("WARNING: crop NO3 uptake is bigger than what is available in layer %d uptake %e soil %e\n", l,uptno3,stand->soil.NO3[l]);
                    if (uptnh4>stand->soil.NH4[l]) printf("WARNING: crop NH4 uptake is bigger than what is available in layer %d uptake %e soil %e\n", l,uptnh4,stand->soil.NH4[l]);
                    if (uptpo4>stand->soil.PO4[l]) printf("WARNING: crop PO4 uptake is bigger than what is available in layer %d uptake %e soil %e\n", l,uptpo4,stand->soil.PO4[l]);
                    //fputs("WARNING: tree no3, po4 or nh4 uptake is bigger than what is available\n",stderr);
                }

                stand->soil.NO3[l] -= uptno3;
                stand->soil.PO4[l] -= uptpo4;
                stand->soil.NH4[l] -= uptnh4;

                annualUptakeP[l]+=uptpo4;
                annualUptakeN[l]+=uptnh4+uptno3;

            }
        } /* end of soil layer loop */
    } /*end of pft check */

    /* for soybeans and others, no uptake of N */
    else{

        // same as previously but only for PO4
        surfuptpo4 = uptakeleafg/(RATIO_CP_LEAF) + uptakesog/(RATIO_CP_STOR);

        forrootsoillayer(l){
            if (l==0){
                uptpo4 = surfuptpo4 + uptakerootg/(RATIO_CP_ROOT)*pft->par->rootdist[l]/totrootdist ;
                uptpo4 = min(uptpo4, stand->soil.PO4[l]);
                stand->soil.PO4[l] -= uptpo4;
            }
            else{
                uptpo4 = uptakerootg/(RATIO_CP_ROOT)*pft->par->rootdist[l]/totrootdist ;
                uptpo4 = min(uptpo4, stand->soil.PO4[l]);
                stand->soil.PO4[l] -= uptpo4;
            }
            if (uptpo4<0) {fputs("WARNING: crop po4 uptake is negative\n",stderr);}

        }
    } /* end of pft type check  */

} /* end of pft loop */

void nit_save_annual_uptake_crop(Pft *pft)
{

    /* pointers to specific pft data */
    Pftcrop *pftcrop;

    /* extraction of crop specific data */
    pftcrop = pft->data;

    pftcrop->annual_uptake.root = pftcrop->uptake.root;
    pftcrop->annual_uptake.leaf = pftcrop->uptake.leaf;
    pftcrop->annual_uptake.so = pftcrop->uptake.so ;

} /* end of pft loop */
