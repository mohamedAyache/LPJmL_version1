
#include "lpj.h"

/* Updates, for the current cell, the monthly output
 * of soil nutrient concentration.
 *
 * Loop over all the stands of the current cell.
 *
 * @param Cell cell: pointer to the current cell
 *
 * \author Nicolas Barrier
 *
 */
void nit_update_outputs(Cell *cell, int cellnumber)

{

    int s;
    int l;
    Stand *stand;
    Real weight[LASTLAYER];

    forrootsoillayer(l){weight[l] =0;}

    foreachstand(stand,s,cell->standlist){
        forrootsoillayer(l){
            weight[l] += stand->cell->coord.area * stand->soil.par->bulkdens[l] * soildepth[l] * 1e-3 * stand->frac;
            cell->output.mNO3_SOIL[l] += stand->soil.NO3[l]*stand->cell->coord.area *stand->frac;
            cell->output.mPO4_SOIL[l] += stand->soil.PO4[l]*stand->cell->coord.area *stand->frac;
            cell->output.mNH4_SOIL[l] += stand->soil.NH4[l]*stand->cell->coord.area *stand->frac;
            cell->output.mDECC_SOIL[l] += stand->soil.DECC[l]*stand->cell->coord.area *stand->frac;
            cell->output.mRESC_SOIL[0] += stand->soil.RESC[l];
            cell->output.mRESN_SOIL[0] += stand->soil.RESN[l];
            cell->output.mRESP_SOIL[0] += stand->soil.RESP[l];
        }
    }
    forrootsoillayer(l){


    cell->output.mNO3_SOIL[l] /= weight[l] ;
    cell->output.mPO4_SOIL[l] /= weight[l] ;
    cell->output.mNH4_SOIL[l] /= weight[l] ;
    cell->output.mDECC_SOIL[l] /= weight[l];
    }


    if(cell->discharge.dmass_lake >=10e-20)
    {
        cell->output.mNO3_LAKE = cell->discharge.NO3lake/Nmolarmass*1000000/cell->discharge.dmass_lake;//change unit to micromol/L
        cell->output.mPO4_LAKE = cell->discharge.PO4lake/Pmolarmass*1000000/cell->discharge.dmass_lake;
        cell->output.mNH4_LAKE = cell->discharge.NH4lake/Nmolarmass*1000000/cell->discharge.dmass_lake;
        cell->output.mPHY_LAKE = cell->discharge.PHYlake/cell->discharge.dmass_lake;
    }
    else
    {
        cell->output.mNO3_LAKE = 0;
        cell->output.mPO4_LAKE = 0;
        cell->output.mNH4_LAKE = 0;
        cell->output.mPHY_LAKE = 0;
    }
    cell->output.mNWastewater = cell->output.mNWastewater/1000000000*12;
    cell->output.mPO4Wastewater = cell->output.mPO4Wastewater/1000000000*12;
    cell->output.mNferti = cell->output.mNferti/1000000000*12;
    cell->output.mPO4ferti = cell->output.mPO4ferti/1000000000*12;
    cell->output.mNmanure = cell->output.mNmanure/1000000000*12;
    cell->output.mPO4manure = cell->output.mPO4manure/1000000000*12;


    cell->output.mPhytoCdischarge =cell->discharge.PHYout;

    cell->output.mNO3discharge = cell->output.mNO3discharge/1000000000*12;//change unit to kt/y
    cell->output.mPO4discharge = cell->output.mPO4discharge/1000000000*12;
    cell->output.mNH4discharge = cell->output.mNH4discharge/1000000000*12;

}

