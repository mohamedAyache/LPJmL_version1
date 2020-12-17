
#include "lpj.h"

/* Percolation/infiltration of nutrients.
 * @param l  layer index
 * @param *stand Pointer to the current stand index (constant)
 * @param *nutrient Pointer to the soil nutrient content (g)
 * @param *Nutperc_ly Pointer to the percolation value from upper layer (g.m-2)
 * @param w_mobile amount of mobile water in the layer (mm)
 * @param srunoff value of the surface water runoff (?)
 * @param runoff Value of the water runoff (?)
 * @param perco value of the percolation
 * @param *soilpar Pointer to the soil parameter (constant)
 *
 * Note that the nutrient concentration and percolation value from upper layer are updated within the fonction. It
 * returns the nutrient leaching (surface + lateral) for the current layer.
 *
 * Note also that in the nitrogen_branch, the input of NO3 are in g.m-2. In our case, NO3 is in g. Therefore, at the beginning
 * of the function, we convert nitrogens into the right units by dividing by the stand area (cell area times stand fraction).
 *
 * Called in "infil_perc_irr" and "infil_perc_rain".
 *
 * \author Nicolas Barrier
 * \date update Mohamed Ayache
 */

Real nit_infilNO3(
        int l,  /* soil layer index */
        const Stand *stand,
        Real *nutrient, /* pointer to the nutrient quantity at a given depth */
        Real *Nutperc_ly, /* Pointer to the percolation value from upper layer */
        Real w_mobile, /* Free water speed */
        Real srunoff,  /* value of the surface runoff */
        Real runoff,   /* Value of the runoff */
        Real perc,  /*value of the percolation */
        const Soilpar *soilpar  /* constant soil parameter */
        )
{

    Real vNut;
    Real concNut_mobile;
    Real ww;
    Real Nutsurf, Nutlat;

    /* determination of nitrate concentration in mobile water */
    ww = vNut = concNut_mobile = 0;



    if (w_mobile > epsilon)
    {
        ww = -w_mobile / ((1 - ANION_EXCL) * soilpar->wsats[l]);  /*Neitsch et al. 2005: Eq 4:2.1.2 */

        vNut = (*nutrient) * (1 - exp(ww));
        concNut_mobile = max(vNut/w_mobile, 0);

        //empirical fix
        concNut_mobile *=0.015;
    }

    /* nitrate movement with percolation */
    /* nitrate percolating from overlying layer */
    *nutrient += *Nutperc_ly;
    *Nutperc_ly = 0;
    /* calculate nitrate in surface runoff */
    if(l==0)
    {
        Nutsurf = NPERCO * concNut_mobile * srunoff; /* Eq. 4:2.1.5 */
        Nutsurf = min(Nutsurf, *nutrient);
        *nutrient -= Nutsurf;
    }
    else{
        Nutsurf=0;
    }
    if (l==0){
        Nutlat = NPERCO * concNut_mobile * runoff; /* Eq. 4:2.1.6 *//* use runoff instead of grunoff, which is the sum of the 2 grunoff computed above */
    }
    else{
        Nutlat = concNut_mobile * runoff; /* Eq. 4:2.1.7 *//* use runoff instead of grunoff, which is the sum of the 2 grunoff computed above */
    }
    Nutlat = min(Nutlat, *nutrient);
    *nutrient -= Nutlat;

    /* nitrate percolating from this layer */
    *Nutperc_ly = concNut_mobile * perc;  /*Eq 4:2.1.8*/
    *Nutperc_ly = min(*Nutperc_ly, *nutrient);
    *nutrient -= *Nutperc_ly;


    return (Nutsurf + Nutlat);

} /* end of if(config->with_nitrogen) */


Real nit_infilNH4(
        int l,  /* soil layer index */
        const Stand *stand,
        Real *nutrient, /* pointer to the nutrient quantity at a given depth */
        Real *Nutperc_ly, /* Pointer to the percolation value from upper layer */
        Real w_mobile, /* Free water speed  */
        Real srunoff,  /* value of the surface runoff */
        Real runoff,   /* Value of the runoff */
        Real perc,  /*value of the percolation */
        const Soilpar *soilpar  /* constant soil parameter */
        )
{

    Real vNut;
    Real concNut_mobile;
    Real ww;
    Real Nutsurf, Nutlat;

    /* determination of nitrate concentration in mobile water */
    ww = vNut = concNut_mobile = 0;



    if (w_mobile > epsilon)
    {
        ww = -w_mobile / ((1 - ANION_EXCL) * soilpar->wsats[l]);  /*Neitsch et al. 2005: Eq 4:2.1.2 */

        vNut = (*nutrient) * (1 - exp(ww));
        concNut_mobile = max(vNut/w_mobile, 0);

        //mouginot empirical fix
        concNut_mobile *=0.004;
    }

    /* nitrate movement with percolation */
    /* nitrate percolating from overlying layer */
    *nutrient += *Nutperc_ly;
    *Nutperc_ly = 0;
    /* calculate nitrate in surface runoff */
    if(l==0)
    {
        Nutsurf = NPERCO * concNut_mobile * srunoff; /* Eq. 4:2.1.5 */
        Nutsurf = min(Nutsurf, *nutrient);
        *nutrient -= Nutsurf;
    }
    else{
        Nutsurf=0;
    }
    if (l==0){
        Nutlat = NPERCO * concNut_mobile * runoff; /* Eq. 4:2.1.6 *//* use runoff instead of grunoff, which is the sum of the 2 grunoff computed above */
    }
    else{
        Nutlat = concNut_mobile * runoff; /* Eq. 4:2.1.7 *//* use runoff instead of grunoff, which is the sum of the 2 grunoff computed above */
    }
    Nutlat = min(Nutlat, *nutrient);
    *nutrient -= Nutlat;
    /* nitrate percolating from this layer */
    *Nutperc_ly = concNut_mobile * perc;  /*Eq 4:2.1.8*/
    *Nutperc_ly = min(*Nutperc_ly, *nutrient);
    *nutrient -= *Nutperc_ly;

    return (Nutsurf + Nutlat);

} /* end of if(config->with_nitrogen) */

Real nit_infilPO4(
        int l,  /* soil layer index */
        const Stand *stand,
        Real *nutrient, /* pointer to the nutrient quantity at a given depth */
        Real srunoff  /* value of the surface runoff */
        )
{

    Real Plost;
    Real PHOSKP;
    Plost=0;
    PHOSKP=100;


    //from the SWAT model PO4 only leaches through runoff
    if(l==0)
    {
        // bulkdens=kg/m3, soildepth=mm PHOSKP=m3/Mg (megagramme)
        Plost = *nutrient* srunoff/ (stand->soil.par->bulkdens[0]/1000 * soildepth[0] *PHOSKP);
        //mouginot empirical fix
        Plost *=0.01;
       *nutrient -= Plost*5;//mouginot add loss from sedimentation not in the model yet   cf. 
    }

    return Plost;

} /* end of if(config->with_nitrogen) */

