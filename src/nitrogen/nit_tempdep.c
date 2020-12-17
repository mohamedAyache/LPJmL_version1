#include "lpj.h"

/* Computes the dependence of nitrification in water as a function of 
 * water temperature and input parameters (gamma and topt).
 * Used in the nitrification and phytoplankton modules.
 *
 * Cf 09, section 02, equation 2.
 * 
 * @param Real twater: water temperature (seasonal cycle used here)
 * @param Real topt:  optimal temperature for the process considered (C)
 * @param Real gamma: sigmoid range (C)
 *
 * \author Nicolas Barrier
 */ 

Real nit_tempdep(
        Real twater, /* water temperature (C) */ 
        Real topt, /* optimal temperature for the process considered (C) */
        Real gamma /* sigmoid range (C) */
        )

{
    /* Computes the dependence of nitrification in water as a function of 
     * water temperature and input parameters (gamma and topt).
     * Used in the nitrification and phytoplankton modules.
     *
     * Cf 09, section 02, equation 2.
     *
     * \author Nicolas Barrier
     */ 

    Real output;
    output = exp(-pow(twater-topt, 2.0)/pow(gamma, 2.0));

    return output;
}





