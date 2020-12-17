
#include "lpj.h"

/* Updates the nitrogen concentration in the river queue. It loops over the ncoeff elements
 * of the nitrogen queues, and for each iteration:
 *  - recovers the values into local variables
 *  - apply denitrification in water (update local variables)
 *  - apply nitrification in water (update local variables)
 *  - update the values in the queue (from local variables).
 *
 *  Called in "drain" (i.e daily time steps)
 *
 *  @param Cell cell: pointer to the current cell
 *
 *  \author Nicolas Barrier
 */
void nit_update_river(Cell *cell, const Config *config, int cellnumber )

{
    int ncoeff;
    int i;
    Real no3, po4,po4ads, nh4, phyto, wat;
    Real dec, res;

    ncoeff = queuesize(cell->discharge.queue);

    for(i=0; i<ncoeff; i++)
    {

        /* we recover the data associated with the queues */
        no3 = getqueue(cell->discharge.NO3Queue, i);
        nh4 = getqueue(cell->discharge.NH4Queue, i);
        po4 = getqueue(cell->discharge.PO4Queue, i);
        po4ads =getqueue(cell->discharge.PO4adsQueue,i);
        phyto = getqueue(cell->discharge.PHYQueue, i);
        res = getqueue(cell->discharge.RESQueue, i);
        dec = getqueue(cell->discharge.DECCQueue, i);
        wat = getqueue(cell->discharge.queue, i) * 1.0e-3; // conversion from dm3 into m3

        /* update of the real values according to the
         * phytoplankton processes */

        if(config->init_phyto == TRUE)
        {
            nit_update_phyto(cell, &phyto, &no3, &nh4, &po4, wat);

            /* immobilisation in water */
            nit_immo_water(cell, phyto, &res, &dec, &no3, &nh4, &po4, wat);
        }
        /* update through water denitrification */
        nit_watdenit(cell, &no3, wat);

        /* update through nitrification in water */
        nit_nitrification_water(cell, wat, &no3, &nh4);
        /*adsorption in water */

        nit_wat_adsorption(cell, wat, &po4, &po4ads,cellnumber);
        /* update of the values within the queues

         * (up_queue coded in the tools/queue.c function) */

        up_queue(cell->discharge.NO3Queue, no3, i);
        up_queue(cell->discharge.PO4Queue, po4, i);
        up_queue(cell->discharge.PO4adsQueue, po4ads, i);
        up_queue(cell->discharge.NH4Queue, nh4, i);
        up_queue(cell->discharge.PHYQueue, phyto, i);
        up_queue(cell->discharge.RESQueue, res, i);
        up_queue(cell->discharge.DECCQueue, dec, i);

    }
}

/* Updates the nitrogen concentration in the lake reservoirs.
 *  - apply denitrification in water
 *  - apply nitrification in water
 *  - update the values in the queue.
 *  Here, lake variables are updated directly
 *
 *  Called in "drain" (i.e daily time steps)
 *
 *  @param Cell cell: pointer to the current cell
 *
 *  \author Nicolas Barrier
 */
void nit_update_lakes(Cell *cell, int cellnumber)
{

    // update of the real values according to the
    // phytoplankton processes
    nit_update_phyto(cell, &cell->discharge.PHYlake, &cell->discharge.NO3lake,
            &cell->discharge.NH4lake, &cell->discharge.PO4lake,
            cell->discharge.dmass_lake*1.0e-3);

    /*immobilisation in water */
    nit_immo_water(cell, cell->discharge.PHYlake, &cell->discharge.RESlake,
            &cell->discharge.DECClake, &cell->discharge.NO3lake,
            &cell->discharge.NH4lake, &cell->discharge.PO4lake, cell->discharge.dmass_lake*1.0e-3);

    // update through water denitrification
    nit_watdenit(cell, &cell->discharge.NO3lake, cell->discharge.dmass_lake*1.0e-3);

    // update through nitrification in water
    nit_nitrification_water(cell, cell->discharge.dmass_lake*1.0e-3, &cell->discharge.NO3lake, &cell->discharge.NH4lake);

    /*adsorption in water */
    nit_wat_adsorption(cell, cell->discharge.dmass_lake*1.0e-3, &cell->discharge.PO4lake, &cell->discharge.PO4adslake, cellnumber);

}
