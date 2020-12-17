#include "lpj.h"

/* Module for the saving of nutrient related variables.
 *
 * \author Nicolas Barrier
 * */

/* Write the soil nutrient variables.
 *
 * Called in "fwritesoil".
 *
 * \author Nicolas Barrier
 * */
Bool nit_write_restarts_soil(FILE *file, const Soil *soil, int ntotpft)
{
    fwriten(soil->NO3, sizeof(Real), LASTLAYER, file);
    fwriten(soil->PO4, sizeof(Real), LASTLAYER, file);
    fwriten(soil->PO4adsorbed, sizeof(Real), LASTLAYER, file);
    fwriten(soil->NH4, sizeof(Real), LASTLAYER, file);
    fwriten(soil->DOC, sizeof(Real), LASTLAYER, file);
    fwriten(soil->DOP, sizeof(Real), LASTLAYER, file);
    fwriten(soil->DON, sizeof(Real), LASTLAYER, file);
    fwriten(soil->RESC, sizeof(Real), LASTLAYER, file);
    fwriten(soil->RESN, sizeof(Real), LASTLAYER, file);
    fwriten(soil->RESP, sizeof(Real), LASTLAYER, file);
    fwriten(soil->DECC, sizeof(Real), LASTLAYER, file);
    return FALSE;
}

/* Writes water nutrient variables
 *
 * Called in "fwritecell" if river routing.
 *
 * \author Nicolas Barrier
 *
 * */
Bool nit_write_restarts_water(FILE *file, const Cell cell)
{

    /* Writting of discharge related variables */
    if(fwritequeue(file,cell.discharge.NO3Queue))
        return TRUE;
    if(fwritequeue(file,cell.discharge.PO4Queue))
        return TRUE;
    if(fwritequeue(file,cell.discharge.PO4adsQueue))
        return TRUE;
    if(fwritequeue(file,cell.discharge.NH4Queue))
        return TRUE;
    if(fwritequeue(file,cell.discharge.PHYQueue))
        return TRUE;
    if(fwritequeue(file,cell.discharge.DOCQueue))
        return TRUE;
    if(fwritequeue(file,cell.discharge.DONQueue))
        return TRUE;
    if(fwritequeue(file,cell.discharge.DOPQueue))
        return TRUE;
    if(fwritequeue(file,cell.discharge.DECCQueue))
        return TRUE;
    if(fwritequeue(file,cell.discharge.RESQueue))
        return TRUE;

    if(fwrite(&cell.discharge.NO3riv,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.NH4riv,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.PO4riv,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.PO4adsriv,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.DOPriv,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.DONriv,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.DOCriv,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.DECCriv,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.RESriv,sizeof(Real),1,file)!=1)
        return TRUE;

    if(fwrite(&cell.discharge.NO3lake,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.NH4lake,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.PO4lake,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.PO4adslake,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.DOPlake,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.DONlake,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.DOClake,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.DECClake,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.RESlake,sizeof(Real),1,file)!=1)
        return TRUE;

    if(fwrite(&cell.discharge.PHYriv,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.PHYlake,sizeof(Real),1,file)!=1)
        return TRUE;

    if(fwrite(&cell.discharge.NO3runoff,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.NH4runoff,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.PO4runoff,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.PO4adsrunoff,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.DOCrunoff,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.DONrunoff,sizeof(Real),1,file)!=1)
        return TRUE;
    if(fwrite(&cell.discharge.DOPrunoff,sizeof(Real),1,file)!=1)
        return TRUE;

    return FALSE;
}
