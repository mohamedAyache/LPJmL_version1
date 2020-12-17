
#include "lpj.h"
#include "agriculture.h"

/* Function that updates the preccheck array, which determines
 * whether rain did occur during the last three days (used for manure
 * and fertilizers)
 *
 * Called in "iterateyear_river"
 *
 * @param Cell cell: pointer to the current cell
 * @param Real dprec: daily precipitation 
 *
 * \author Nicolas Barrier
 * 
 */ 
void nit_update_preccheck(Cell *cell, Real dprec){

    cell->preccheck[0] = cell->preccheck[1];
    cell->preccheck[1] = cell->preccheck[2];
    cell->preccheck[2] = dprec > 0 ? 1 : 0;   /* the last element is updated */

    /* the last element is the sum of the three: if equals to 3, then
     * the last three days were rainy */
    cell->preccheck[3] = cell->preccheck[0] +  cell->preccheck[1] + cell->preccheck[2];
}
