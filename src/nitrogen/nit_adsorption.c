#include "lpj.h"

/* PO4 adsorption/desorption in soil for current stand (loop over all soil layers).
 *
 * Called in *update_daily*
 *
 * @param Stand *stand
 *
 *
 * */
void nit_soil_adsorption(Stand *stand)
{

    int l;
    Real vwater;   /* volume water in the soil (m3) */
    Real weight;  /* soil weights (kg soil) */
    Real totalPsoil; /*quantity of adsorbed P + quantity of "free" P  (gP)*/
    Real constantK;
    Real delta;
    Soil *soil;
    Real standSurface;

    soil= &(stand->soil);
    standSurface= stand->frac*stand->cell->coord.area;

    for(l=0; l<LASTLAYER; l++)
    {

        // alwater returns mm, *1e-3 to convert into m
        // multiplication by stand area to have m3
        vwater = allwater(soil, l) * 1e-3 * stand->cell->coord.area * stand->frac;
        if (vwater > 0)
        {
            // weights must be in kilos
            // area = m2, bulkdens=kg/m3, soildepth=mm => conversion of mm into m (*1e-3)
            weight = stand->cell->coord.area * stand->soil.par->bulkdens[l] * soildepth[l] * 1e-3 * stand->frac;
            //printf("l %d soildepth[l] %e \n", l,soildepth[l]);

            //ads = weight * A * pow(stand->soil.PO4[l]/vwater, B/A);//adsorption formula

            totalPsoil= (stand->soil.PO4[l]+stand->soil.PO4adsorbed[l])*standSurface;

            //find the equilibrium of adsorbed vs "free" Po4, solve the equation totalPsoil-stand->soil.PO4=weight * A * pow(stand->soil.PO4/vwater, B/A);
            //as B/A equals roughtly 0.5 we approximately solve this as a quadratic equation. It should be fine as we don't need a very precise P model anyway.

            //constantK= weight*A/(pow(vwater,B/A));
            constantK= weight*A/(pow(vwater,0.5));
            //soil.PO4 + constantK*pow(soil.PO4,B/A)-totalPSoil =0
            //x + K*racine(x) -total =0
            //y*y + K*y -total =0

            delta = constantK*constantK + 4* totalPsoil;

            //y = (-K + racine (delta))/2
            stand->soil.PO4[l]= (pow((-constantK + pow(delta,0.5))/2.,2.))/standSurface ;
            stand->soil.PO4adsorbed[l] = totalPsoil/standSurface-stand->soil.PO4[l];

            if (stand->soil.PO4[l]<-0.01)fail(NEGATIVE_SOIL_PO4_ERR,TRUE,"infil_perc_rain: Pixel: %.2f %.2f PO4=%g<0 in layer %d", stand->cell->coord.lat,stand->cell->coord.lon,stand->soil.PO4[l],l);
        }
        else
        {
            //if (stand->soil.PO4[l] != 0) printf("WARNING: no more water in the soil layer %d PO4 in this layer should be 0 but is %e\n",l,stand->soil.PO4[l]);
        }


    }
}

/* PO4 adsorption in water. It takes as an input the current cell, the total
 * volume of water considered (either lake or river water) and the associated PO4 quantity.
 *
 * Called in nit_update_river (for river) and in nit_update_lakes (for lakes)
 *
 * @param Cell cell: pointer to the current cell (constant)
 * @param Real vwater: water volume considered (m3)
 * @param Real po4: pointer to the associated PO4 content (g)
 *
 */
void nit_wat_adsorption(const Cell *cell, const Real vwater, Real *po4, Real *po4adsorbed, int cellnumber)
{

    Real totalPWater; /*quantity of adsorbed P + quantity of "free" P  (gP)*/
    Real csusp; // concentration of suspended solids in the water (g/m3)
    Real delta;
    Real b;

    // nitrogen.h:
    // V MAXP,SUSP = maximal adsorption rate of P to suspended solids (g/g)
    // Kwater,ads = half saturation constant for P adsorption (g/m3)
    //

    // if the water volume is 0, then nothing is done
    // (prevents NaNs)
    if(vwater <= 10e-20)
        return;

    // Warning: in equation 3 of pdf document, the input is a concentration cont = VMAX_P_SUSP * (*po4)/(*po4 + KWATER_ADS*vwater);  /* equation (3) */
    /* calculation of suspended solids in water depending on the surface type, eq. (4)
    //We look for the new equilibrium between po4 and po4adsorbed
    //To do so we need to solve the quadratic equation we get from replacing po4adsorbed by totalPWater-po4 in po4adsorbed = cont * csusp * vwater
    //We need to solve -po4*po4 +po4(totalPWater- (csusp*vwater*VMAX_P_SUSP)- (KWATER_ADS*vwater)) + KWATER_ADS*vwater*totalPWater=0

    csusp = (cell->surf_for * CSUSP_FOR + cell->surf_grass * CSUSP_GRASS + cell->surf_crop * CSUSP_CROP);

    totalPWater=(*po4)+(*po4adsorbed);

    b= totalPWater- (csusp*vwater*VMAX_P_SUSP)- (KWATER_ADS*vwater);
    delta = b*b +4*KWATER_ADS*vwater*totalPWater;

    (*po4)=(b+pow(delta,0.5))/2;
    (*po4adsorbed)= totalPWater -(*po4);

}

/* Update of the cell forest, crop and grass fraction
 *
 * Called in "iterateyear_river"
 *
 * @param Config config: constant pointer to the configuration object
 * @param Cell cell: pointer to the current cell
 */
void nit_udpdate_frac_for_ads(const Config *config, Cell *cell)
{
    //Stand *stand;
    int j;

    // initialisation of the crop fractions to 0
    cell->surf_crop = cell->surf_for = cell->surf_grass = 0.0;

    if(config->withlanduse)
    {
        for(j=0; j<NGRASS; j++)
        {
            cell->surf_grass += cell->ml.landfrac[0].grass[j];
            cell->surf_grass += cell->ml.landfrac[1].grass[j];
        }

        for(j=0; j<config->npft[CROP]; j++){
            cell->surf_crop += cell->ml.landfrac[0].crop[j];
            cell->surf_crop += cell->ml.landfrac[1].crop[j];
        }

        for(j=0; j<config->nagtree; j++){
            cell->surf_crop += cell->ml.landfrac[0].ag_tree[j];
            cell->surf_crop += cell->ml.landfrac[1].ag_tree[j];
        }
    }

    cell->surf_for = 1 - cell->surf_crop - cell->surf_grass;
}
