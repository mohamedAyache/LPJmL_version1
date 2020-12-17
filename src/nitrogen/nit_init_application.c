
#include "lpj.h"

/* Initializes the application arrays for each CFT (both irrigated and not
 * irrigated). If application[i] = 1, then we must apply the ith application of manure/fert.
 * Else, the ith application has already been done.
 *
 * WARNING: application only exists with crop and agricultural trees.
 *
 * Called in iterateyear_river.c
 *
 * @param Cell cell: pointer to the current cell
 * @param int ncft: Number of cfts
 * @param int nagtree: Number of agricultural trees
 *
 * \author Nicolas Barrier
 */
void nit_init_application(Cell *cell, int ncft, int nagtree)
{
    int i;

    // loop over all the application elements.
    for (i=0; i<2*(ncft+nagtree); i++)
    {
        cell->ml.application[i].ferti[0] = 1;
        cell->ml.application[i].ferti[1] = 0;
        cell->ml.application[i].ferti[2] = 0;

        cell->ml.application[i].manure[0] = 1;
        cell->ml.application[i].manure[1] = 0;
        cell->ml.application[i].manure[2] = 0;
    }
}

