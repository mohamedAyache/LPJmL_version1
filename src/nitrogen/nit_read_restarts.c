/* Module for the loading of nutrient related variables
 *
 * \author Nicolas Barrier
 * */

#include "lpj.h"

/* Read the soil nutrient variables.
 *
 * Called in "freadsoil"
 *
 * \author Nicolas Barrier
 * \update Mohamed Ayache
 */
Bool nit_read_restarts_soil(FILE *file, Soil *soil, int ntotpft, Bool swap)
{

    freadreal(soil->NO3, LASTLAYER, swap, file);
    freadreal(soil->PO4, LASTLAYER, swap, file);
    freadreal(soil->PO4adsorbed, LASTLAYER, swap, file);
    freadreal(soil->NH4, LASTLAYER, swap, file);
    freadreal(soil->DOC, LASTLAYER, swap, file);
    freadreal(soil->DOP, LASTLAYER, swap, file);
    freadreal(soil->DON, LASTLAYER, swap, file);
    freadreal(soil->RESC, LASTLAYER, swap, file);
    freadreal(soil->RESN, LASTLAYER, swap, file);
    freadreal(soil->RESP, LASTLAYER, swap, file);
    freadreal(soil->DECC, LASTLAYER, swap, file);

    return FALSE;


}

/* Read water nutrient variables.
 *
 * Called in "freadcell" if river routing
 *
 * \author Nicolas Barrier
 *
 * */
Bool nit_read_restarts_water (FILE *file, Cell *cell, Bool swap)
{

    /* Reading discharge related variables */
    cell->discharge.NO3Queue = freadqueue(file, swap);
    cell->discharge.PO4Queue = freadqueue(file, swap);
    cell->discharge.PO4adsQueue = freadqueue(file, swap);
    cell->discharge.NH4Queue = freadqueue(file, swap);
    cell->discharge.PHYQueue = freadqueue(file, swap);
    cell->discharge.DOCQueue = freadqueue(file, swap);
    cell->discharge.DONQueue = freadqueue(file, swap);
    cell->discharge.DOPQueue = freadqueue(file, swap);
    cell->discharge.DECCQueue = freadqueue(file, swap);
    cell->discharge.RESQueue = freadqueue(file, swap);

    freadreal1(&cell->discharge.NO3riv,swap,file);
    freadreal1(&cell->discharge.NH4riv,swap,file);
    freadreal1(&cell->discharge.PO4riv,swap,file);
    freadreal1(&cell->discharge.PO4adsriv,swap,file);
    freadreal1(&cell->discharge.DOPriv,swap,file);
    freadreal1(&cell->discharge.DONriv,swap,file);
    freadreal1(&cell->discharge.DOCriv,swap,file);
    freadreal1(&cell->discharge.DECCriv,swap,file);
    freadreal1(&cell->discharge.RESriv,swap,file);

    freadreal1(&cell->discharge.NO3lake,swap,file);
    freadreal1(&cell->discharge.NH4lake,swap,file);
    freadreal1(&cell->discharge.PO4lake,swap,file);
    freadreal1(&cell->discharge.PO4adslake,swap,file);
    freadreal1(&cell->discharge.DOPlake,swap,file);
    freadreal1(&cell->discharge.DONlake,swap,file);
    freadreal1(&cell->discharge.DOClake,swap,file);
    freadreal1(&cell->discharge.DECClake,swap,file);
    freadreal1(&cell->discharge.RESlake,swap,file);

    freadreal1(&cell->discharge.PHYriv,swap,file);
    freadreal1(&cell->discharge.PHYlake,swap,file);

    freadreal1(&cell->discharge.NO3runoff,swap,file);
    freadreal1(&cell->discharge.NH4runoff,swap,file);
    freadreal1(&cell->discharge.PO4runoff,swap,file);
    freadreal1(&cell->discharge.PO4adsrunoff,swap,file);
    freadreal1(&cell->discharge.DOCrunoff,swap,file);
    freadreal1(&cell->discharge.DONrunoff,swap,file);
    freadreal1(&cell->discharge.DOPrunoff,swap,file);

    return FALSE;
}
