/***************************************************************************/
/**                                                                       **/
/**                     d  r  a  i  n  .  c                               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Mathematical description of the river routing algorithm can be    **/
/**     found in:                                                         **/
/**     Rost, S., Gerten, D., Bondeau, A., Lucht, W., Rohwer, J.,         **/
/**     Schaphoff, S.: Agricultural green and blue water consumption and  **/
/**     its influence on the global water system. Water Resources         **/
/**     Research (in press).                                              **/
/**                                                                       **/
/**     Function performs river routing using the parallel Pnet library   **/
/**                                                                       **/
/**     Description of pnet is given in:                                  **/
/**     W. von Bloh, 2008. Sequential and Parallel Implementation of      **/
/**     Networks. In P. beim Graben, C. Zhou, M. Thiel, and J. Kurths     **/
/**     (eds.), Lectures in Supercomputational Neuroscience, Dynamics in  **/
/**     Complex Brain Networks, Springer, New York, 279-318.              **/
/**                                                                       **/
/**     written by Stefanie Rost, Werner von Bloh, Sibyll Schaphoff       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-14 09:09:50 +0200 (mar., 14 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

/**
 * Updates the nutrient concentration in a lake reservoir as a function of what leaves the
 * reservoir.
 * @param finwat volume that leaves the lake reservoir (dm3)
 * @param *NO3lake pointer to the nutrient concentration within the lake reservoir (g)
 * @param concNO3 nutrient concentration within the reservoir (g/dm3)
 * @return the nutrient quantity that leaves the the reservoir (g)
 *
 * \author Nicolas Barrier
 *
*/
static Real update_drain(Real finwat, Real *NO3lake, const Real concNO3)
{
    Real finno3;

    // computes the nutrient that leaves the reservoir
    finno3 = finwat * concNO3 ;

    // insure that the NO3lake variable is never negative
    //finno3 = min(finno3, *NO3lake);

    // update of the nutrient quantity within the reservoir
    *NO3lake -= finno3;


    return finno3;
}

/* Fills a reservoir with water. It returns the remaining
 * water (i.e. the water volume that does not fit into the
 * reservoir).
 *
 * @param Real *mass: water mass content of the reservoir
 * @param Real in: water mass added to the reservoir
 * @param Real mass_max: reservoir capacity
 * @return: The water content that does not fit into the reservoir
 */
static Real fillreservoir(Real *mass, Real in, Real mass_max)
{
    *mass+=in;
    if(*mass>mass_max)
    {
        in=*mass-mass_max;
        *mass=mass_max;
    }
    else
        in=0;
    return in;
} /* of 'fillreservoir' */


void drain(Cell grid[],       /* Cell array */
        const Config *config, /* Grid configuration */
        int month          /* month (0..11) */
        )
{

    /* The function has been updated to allow for nutrient in rivers.
     * It works as follows. The objet used in the transfer among processes is a runoff object,
     * containing several variables that will be exchanged (water, no3, etc, see "nitrogen.h").
     *
     * First, we determine the water runoff that enter the lake and river reservoirs.
     * If water runoff exceeds the maximum lake volume, then we split nutrients into the lake and river reservoirs.
     * If "volume" is the quantity that should enter the lake (after filling dams) and "dmass_lake" the water content already in the lake.
     * Therefore, the water quantity that enters the lake is "dmass_max - dmass", while the volume
     * that enters the river is "volume + dmass - dmass_max".
     * We add nutrients in water reservoirs using proportionality: if Wat and Nut are the runoff quantity of water and nutriments, respectively
     * and Riv/Lake the runoff water that enters the river and lake, therefore, Nutriv = Riv * Nut / Wat and Nutlake = Lake * Nut / Wat
     *
     * If all the runoff water fills the lake, then nothing goes into the river and everything goes into the lake.
     *
     * If waste_water are included, we add the Nutwas water to the Nutrunoff variable (assume that all wastewater go into rivers, maybe to be improved)
     *
     * At the end of this first step, the Nutrunoff variable (in g/m2) has been overwritten and is the quantity that should enter the river reservoir.
     *
     * Then, at the first step of the iter loop, we add to the queue and to the Riv reservoir this Nutrunoff variable (using the
     * integer variable fac, which equals 1 when iter==0).
     *
     * \author Nicolas Barrier
     * \Updat Mohamed Ayache
     *
     */


    int count,cell,i,j,iter,t,ncoeff;

    Runoff fin, *out, *in;

    Real fout_lake, irrig_to_river;
    Real volume; /* barrier.n */
    Real concNO3, concPO4, concPO4ads, concNH4, concDOC, concDON, concDOP, concPHY; // barrier.n
    Real concRES, concDECC;

    int rank;

#ifdef USE_MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

    count = (int)(1.0/TSTEP); /* calculate number of iterations */

    /* out is the structure containing the variables that will be output (wat, no3, etc) */
    /* in is the structure containing the variables that will be input (wat, no3, etc) */
    out = (Runoff *)pnet_output(config->route);
    in = (Runoff *)pnet_input(config->route);

    for(cell=0;cell<config->ngridcell;cell++)
    {

        grid[cell].discharge.dfout=0.0;

        /* add runoff to reservoir or lake */
        if(!grid[cell].skip)
        {
            grid[cell].discharge.mfin+=grid[cell].discharge.drunoff*grid[cell].coord.area;

            if(grid[cell].ml.dam)
            {
                //todo add nutrient
                /* volume = quantity of runoff water that goes into the lake, ie
                 * it is the part of runoff water that excess the dam capacity */
                volume = fillreservoir(&grid[cell].ml.resdata->dmass,
                        grid[cell].discharge.drunoff*grid[cell].coord.area,
                        grid[cell].ml.resdata->reservoir.capacity);
            }
            else
            {
                // if no dam, all the runoff water goes into the lake reservoir
                volume = grid[cell].discharge.drunoff*grid[cell].coord.area;
            }

            // puts all the nutrient runoff in the lake reservoir (as water is put in the lake reservoir)
            // we scale depending on the effective water runoff that enters the lake
            // Units:
            // NO3lake = [g]
            // volume = [dm3]
            // NO3runoff = [gN.m^{-2}]
            // drunoff = [mm] = [10^{-3} m]
            // hence, [dm3] * [gN.m^{-2}] * [10^3 m^{-1}] = [dm3] * [gN] * [10^3 m-3] = [dm3] * [gN] * [dm-3] = [gN] => ok
            if(grid[cell].discharge.drunoff > epsilon) {//todo rework that put only what remains from above
                grid[cell].discharge.NO3lake += volume*(grid[cell].discharge.NO3runoff / grid[cell].discharge.drunoff);// == grid[cell].discharge.NO3runoff*grid[cell].coord.area

                grid[cell].discharge.PO4lake += volume*(grid[cell].discharge.PO4runoff / grid[cell].discharge.drunoff);
                grid[cell].discharge.PO4adslake += volume*(grid[cell].discharge.PO4adsrunoff / grid[cell].discharge.drunoff);
                grid[cell].discharge.NH4lake += volume*(grid[cell].discharge.NH4runoff / grid[cell].discharge.drunoff);
                grid[cell].discharge.DOClake += volume*(grid[cell].discharge.DOCrunoff / grid[cell].discharge.drunoff);
                grid[cell].discharge.DONlake += volume*(grid[cell].discharge.DONrunoff / grid[cell].discharge.drunoff);
                grid[cell].discharge.DOPlake += volume*(grid[cell].discharge.DOPrunoff / grid[cell].discharge.drunoff);
            }
            // add the water volume into the lake reservoir
            grid[cell].discharge.dmass_lake += volume;
            grid[cell].discharge.mfin += fin.wat;

        }

        /* release from reservoirs*/
        /* barrier.n: WARNING!!!!!!!! release from reservoirs do not impact yet the nutrient todo dfoutnutrient + dfout_irrigation_daily_nutrient
         * pools in rivers and lakes */
        if(grid[cell].ml.dam)
        {
            grid[cell].ml.resdata->dfout_res=0.0; /* dfout_res = total outflow of reservoir [dm3] */

            if(grid[cell].discharge.next>=0)
                grid[cell].ml.resdata->dfout_res=outflow_reservoir(grid[cell].ml.resdata,month);

            /* irrigation reservoirs*/
            if(grid[cell].ml.resdata->reservoir.purpose[0]==2 || grid[cell].ml.resdata->reservoir.purpose[1]==1)
            {
                /* the water that was reserved for irrigation 5 days ago, but still not used is released to the river*/
                irrig_to_river=grid[cell].ml.resdata->dfout_irrigation_daily[0];
                for(i=0;i<(NIRRIGDAYS-1);i++)
                    grid[cell].ml.resdata->dfout_irrigation_daily[i]=grid[cell].ml.resdata->dfout_irrigation_daily[i+1];

                grid[cell].ml.resdata->dfout_irrigation_daily[(NIRRIGDAYS-1)]=0.0;

                /* part of todays outflow goes directly to the river
                 * (environmental flow) and part can be used for irrigation
                 */

                if(grid[cell].ml.resdata->dfout_res>ENV_FLOW*grid[cell].ml.resdata->mean_inflow_month[month]/ndaymonth[month])
                {
                    grid[cell].ml.resdata->dfout_irrigation_daily[(NIRRIGDAYS-1)]=grid[cell].ml.resdata->dfout_res
                        -ENV_FLOW*grid[cell].ml.resdata->mean_inflow_month[month]/ndaymonth[month];
                    grid[cell].ml.resdata->dfout_res-=grid[cell].ml.resdata->dfout_irrigation_daily[(NIRRIGDAYS-1)];
                }
                grid[cell].ml.resdata->dfout_res+=irrig_to_river;
            } /* end of irrigation reservoirs */

            grid[cell].discharge.dmass_lake+=grid[cell].ml.resdata->dfout_res;
        }

    } /* of 'for(cell=...)' */


    grid-=config->startgrid-config->firstgrid; /* adjust first index of grid
                                                  array needed for pnet library */

    /* this corresponds to the algo2 of the Bloh et al. (2010) paper. */
    /* according to Werner, this iteration loop, with count=8 (number of adjacent cells)
     * is due to the fact that water can flow to the poles through more than one cell */
    for(iter=0;iter<count;iter++)
    {
        for(i=pnet_lo(config->route);i<=pnet_hi(config->route);i++)
        {
            /* calculate outflow */
            grid[i].discharge.fout=0.0;
            grid[i].discharge.NO3out=0.0;
            grid[i].discharge.PO4out=0.0;
            grid[i].discharge.PO4adsout=0.0;
            grid[i].discharge.NH4out=0.0;
            grid[i].discharge.PHYout=0.0;
            grid[i].discharge.DOCout=0.0;
            grid[i].discharge.DONout=0.0;
            grid[i].discharge.DOPout=0.0;
            grid[i].discharge.RESout=0.0;
            grid[i].discharge.DECCout=0.0;
            ncoeff=queuesize(grid[i].discharge.queue);

            for(t=0;t<ncoeff;t++)
            {
                grid[i].discharge.fout += getqueue(grid[i].discharge.queue,t)*grid[i].discharge.tfunct[t];
                grid[i].discharge.NO3out += getqueue(grid[i].discharge.NO3Queue,t)*grid[i].discharge.tfunct[t];
                grid[i].discharge.PO4out += getqueue(grid[i].discharge.PO4Queue,t)*grid[i].discharge.tfunct[t];
                grid[i].discharge.PO4adsout += getqueue(grid[i].discharge.PO4adsQueue,t)*grid[i].discharge.tfunct[t];
                grid[i].discharge.NH4out += getqueue(grid[i].discharge.NH4Queue,t)*grid[i].discharge.tfunct[t];
                grid[i].discharge.PHYout += getqueue(grid[i].discharge.PHYQueue,t)*grid[i].discharge.tfunct[t];
                grid[i].discharge.DOCout += getqueue(grid[i].discharge.DOCQueue,t)*grid[i].discharge.tfunct[t];
                grid[i].discharge.DONout += getqueue(grid[i].discharge.DONQueue,t)*grid[i].discharge.tfunct[t];
                grid[i].discharge.DOPout += getqueue(grid[i].discharge.DOPQueue,t)*grid[i].discharge.tfunct[t];
                grid[i].discharge.RESout += getqueue(grid[i].discharge.RESQueue,t)*grid[i].discharge.tfunct[t];
                grid[i].discharge.DECCout += getqueue(grid[i].discharge.DECCQueue,t)*grid[i].discharge.tfunct[t];
            }

            grid[i].discharge.dmass_river-=grid[i].discharge.fout;
            grid[i].discharge.dfout+=grid[i].discharge.fout;
            grid[i].output.mdischarge+=grid[i].discharge.fout;
            grid[i].output.daily.discharge+=grid[i].discharge.fout*1e-3*dayseconds1; /*in m3 per second */
            grid[i].discharge.mfout+=grid[i].discharge.fout;

            grid[i].discharge.NO3riv -= grid[i].discharge.NO3out;
            grid[i].output.mNO3discharge += grid[i].discharge.NO3out;

            grid[i].discharge.PO4riv -= grid[i].discharge.PO4out;
            grid[i].output.mPO4discharge += grid[i].discharge.PO4out;

            grid[i].discharge.PO4adsriv -= grid[i].discharge.PO4adsout;
            grid[i].output.mPO4adsdischarge += grid[i].discharge.PO4adsout;

            grid[i].discharge.NH4riv -= grid[i].discharge.NH4out;
            grid[i].output.mNH4discharge += grid[i].discharge.NH4out;

            grid[i].discharge.PHYriv -= grid[i].discharge.PHYout;
            grid[i].output.mPhytoCdischarge += grid[i].discharge.PHYout;

            grid[i].discharge.DOCriv -= grid[i].discharge.DOCout;
            grid[i].output.mDOCdischarge += grid[i].discharge.DOCout;

            grid[i].discharge.DONriv -= grid[i].discharge.DONout;
            grid[i].output.mDONdischarge += grid[i].discharge.DONout;

            grid[i].discharge.DOPriv -= grid[i].discharge.DOPout;
            grid[i].output.mDOPdischarge += grid[i].discharge.DOPout;

            grid[i].discharge.RESriv -= grid[i].discharge.RESout;
            grid[i].discharge.DECCriv -= grid[i].discharge.DECCout;

        }

        /* fill output buffer */
        /* Step 1 of the algorithm 2 */
        for(i=0;i<pnet_outlen(config->route);i++)
        {
            out[i].wat=grid[pnet_outindex(config->route,i)].discharge.fout;
            out[i].no3=grid[pnet_outindex(config->route,i)].discharge.NO3out;
            out[i].nh4=grid[pnet_outindex(config->route,i)].discharge.NH4out;
            out[i].po4=grid[pnet_outindex(config->route,i)].discharge.PO4out;
            out[i].po4ads = grid[pnet_outindex(config->route,i)].discharge.PO4adsout;
            out[i].phy=grid[pnet_outindex(config->route,i)].discharge.PHYout;
            out[i].doc=grid[pnet_outindex(config->route,i)].discharge.DOCout;
            out[i].don=grid[pnet_outindex(config->route,i)].discharge.DONout;
            out[i].dop=grid[pnet_outindex(config->route,i)].discharge.DOPout;
            out[i].res=grid[pnet_outindex(config->route,i)].discharge.RESout;
            out[i].decc=grid[pnet_outindex(config->route,i)].discharge.DECCout;

        }

        /* communication function fills input buffer: puts "out" in "in"*/
        pnet_exchg(config->route);

        for(i=pnet_lo(config->route);i<=pnet_hi(config->route);i++)
        {
            /* sum up all inflows from other cells */
            fin.wat = 0.0;
            fin.no3 = 0.0;
            fin.po4 = 0.0;
            fin.po4ads = 0.0;
            fin.nh4 = 0.0;
            fin.phy = 0.0;
            fin.doc = 0.0;
            fin.don = 0.0;
            fin.dop = 0.0;
            fin.res = 0.0;
            fin.decc = 0.0;

            for(j=0;j<pnet_inlen(config->route,i);j++)
            {

                fin.wat += in[pnet_inindex(config->route,i,j)].wat;
                fin.no3 += in[pnet_inindex(config->route,i,j)].no3;
                fin.po4 += in[pnet_inindex(config->route,i,j)].po4;
                fin.po4ads += in[pnet_inindex(config->route,i,j)].po4ads;
                fin.nh4 += in[pnet_inindex(config->route,i,j)].nh4;
                fin.phy += in[pnet_inindex(config->route,i,j)].phy;
                fin.doc += in[pnet_inindex(config->route,i,j)].doc;
                fin.don += in[pnet_inindex(config->route,i,j)].don;
                fin.dop += in[pnet_inindex(config->route,i,j)].dop;
                fin.res += in[pnet_inindex(config->route,i,j)].res;
                fin.decc += in[pnet_inindex(config->route,i,j)].decc;

            }

            grid[i].discharge.mfin += fin.wat;

            // all nitrogen is put into the lake reservoir
            // therefore, so far the lake reservoir contains

            // both runoff (step one) and river inputs
            grid[i].discharge.NO3lake += fin.no3;
            grid[i].discharge.PO4lake += fin.po4;
            grid[i].discharge.PO4adslake += fin.po4ads;
            grid[i].discharge.NH4lake += fin.nh4;
            grid[i].discharge.DONlake += fin.don;
            grid[i].discharge.DOClake += fin.doc;
            grid[i].discharge.DOPlake += fin.dop;
            grid[i].discharge.PHYlake += fin.phy;
            grid[i].discharge.RESlake += fin.res;
            grid[i].discharge.DECClake += fin.decc;

            // volume is the total water associated with the lake reservoir
            // (for having the concentration). At step one, volume contains
            // the (dmass + runoff + river) volume, at other steps it
            // contains only (dmass + river)
            // therefore, volume=0 assumes that we have no runoff and also no lake
            // therefore, nothing to do
            volume = (fin.wat + grid[i].discharge.dmass_lake);
            if(volume < epsilon ){
                // avoids division by 0 and therefore NaNs.
                concNO3 =  0.0;
                concPO4 =  0.0;
                concPO4ads = 0.0;
                concNH4 =  0.0;
                concDOC =  0.0;
                concDON =  0.0;
                concDOP =  0.0;
                concPHY =  0.0;
                concRES =  0.0;
                concDECC =  0.0;
            }
            else{
                concNO3 = grid[i].discharge.NO3lake / volume;
                concPO4 = grid[i].discharge.PO4lake / volume;
                concPO4ads = grid[i].discharge.PO4adslake / volume;
                concNH4 = grid[i].discharge.NH4lake / volume;
                concDOC = grid[i].discharge.DOClake / volume;
                concDON = grid[i].discharge.DONlake / volume;
                concDOP = grid[i].discharge.DOPlake / volume;
                concPHY = grid[i].discharge.PHYlake / volume;
                concRES = grid[i].discharge.RESlake / volume;
                concDECC = grid[i].discharge.DECClake / volume;
            }

            // reinitialises the input variable to 0
            // this is what will be put into the river
            fin.no3 = 0.0;
            fin.po4 = 0.0;
            fin.nh4 = 0.0;
            fin.phy = 0.0;
            fin.doc = 0.0;
            fin.don = 0.0;
            fin.dop = 0.0;
            fin.decc = 0.0;
            fin.res = 0.0;


            /* filling the dam reservoir */
            // fin.wat is the remaining river water after filling the dam //todo change what is done above put mutrient in reservoir first
            if(grid[i].ml.dam)
            {
                fin.wat=fillreservoir(&grid[i].ml.resdata->dmass,fin.wat,
                        grid[i].ml.resdata->reservoir.capacity);
            }

            // fin.wat is the river water that does not fit into the lake reservoir.
            // accordingly, we extract part of the lake nutrients and add them to
            // the river reservoir.
            // at step 1, the dmass_lake can exceed the reservoir (since the runoff were added into the lake
            // reservoir without using the fillreservoir function.
            fin.wat = fillreservoir(&grid[i].discharge.dmass_lake, fin.wat, grid[i].discharge.dmass_lake_max);

            // if the dmass_lake_max is 0, then all the nutrients should be removed from the lakes and moved to
            // the river reservoir. However, presumably due to rounding errors in the calculation of concentration,
            // we may have a residual. So a special case is treated by hand: if all the water should be removed from the lake,
            // then we all the nutrients to leave the lake

            if(fin.wat == volume)
            {

                fin.no3 += grid[i].discharge.NO3lake;
                grid[i].discharge.NO3lake = 0.0;
                fin.po4 += grid[i].discharge.PO4lake;
                grid[i].discharge.PO4lake = 0.0;
                fin.po4ads += grid[i].discharge.PO4adslake;
                grid[i].discharge.PO4adslake = 0.0;
                fin.nh4 += grid[i].discharge.NH4lake;
                grid[i].discharge.NH4lake = 0.0;
                fin.doc += grid[i].discharge.DOClake;
                grid[i].discharge.DOClake = 0.0;
                fin.don += grid[i].discharge.DONlake;
                grid[i].discharge.DONlake = 0.0;
                fin.dop += grid[i].discharge.DOPlake;
                grid[i].discharge.DOPlake = 0.0;
                fin.phy += grid[i].discharge.PHYlake;
                grid[i].discharge.PHYlake = 0.0;
                fin.res += grid[i].discharge.RESlake;
                grid[i].discharge.RESlake = 0.0;
                fin.decc += grid[i].discharge.DECClake;
                grid[i].discharge.DECClake = 0.0;
            }
            else
            {
                // removing of the corresponding nutrient concentration from the lakes
                // and adding it to the river reservoir
                fin.no3 += update_drain(fin.wat, &grid[i].discharge.NO3lake, concNO3);
                fin.po4 += update_drain(fin.wat, &grid[i].discharge.PO4lake, concPO4);
                fin.po4ads += update_drain(fin.wat, &grid[i].discharge.PO4adslake, concPO4ads);
                fin.nh4 += update_drain(fin.wat, &grid[i].discharge.NH4lake, concNH4);
                fin.doc += update_drain(fin.wat, &grid[i].discharge.DOClake, concDOC);
                fin.don += update_drain(fin.wat, &grid[i].discharge.DONlake, concDON);
                fin.dop += update_drain(fin.wat, &grid[i].discharge.DOPlake, concDOP);
                fin.phy += update_drain(fin.wat, &grid[i].discharge.PHYlake, concPHY);
                fin.res += update_drain(fin.wat, &grid[i].discharge.RESlake, concRES);
                fin.decc += update_drain(fin.wat, &grid[i].discharge.DECClake, concDECC);

            }

            /* lake outflow
             * we therefore remove the corresponding nutrients from the lake and add it to
             * the river reservoir */
            if(grid[i].discharge.dmass_lake>0.0 && grid[i].discharge.next>=0)
            {
                fout_lake=kr/count*grid[i].discharge.dmass_lake*pow(grid[i].discharge.dmass_lake/grid[i].discharge.dmass_lake_max,1.5);
                grid[i].discharge.dmass_lake -= fout_lake;
                fin.wat += fout_lake;
                fin.no3 += update_drain(fout_lake, &grid[i].discharge.NO3lake, concNO3);
                fin.po4 += update_drain(fout_lake, &grid[i].discharge.PO4lake, concPO4);
                fin.po4ads += update_drain(fout_lake, &grid[i].discharge.PO4adslake, concPO4ads);
                fin.nh4 += update_drain(fout_lake, &grid[i].discharge.NH4lake, concNH4);
                fin.doc += update_drain(fout_lake, &grid[i].discharge.DOClake, concDOC);
                fin.don += update_drain(fout_lake, &grid[i].discharge.DONlake, concDON);
                fin.dop += update_drain(fout_lake, &grid[i].discharge.DOPlake, concDOP);
                fin.phy += update_drain(fout_lake, &grid[i].discharge.PHYlake, concPHY);
                fin.res += update_drain(fout_lake, &grid[i].discharge.RESlake, concRES);
                fin.decc += update_drain(fout_lake, &grid[i].discharge.DECClake, concDECC);

            }

            /* water withdrawal from the outflow from lake and reservoir*/
            if(fin.wat > grid[i].discharge.wd_demand/count)
            {
                grid[i].discharge.withdrawal+=grid[i].discharge.wd_demand/count;
                grid[i].discharge.mfout+=grid[i].discharge.wd_demand/count;

                //move the nutrient taken with the water withdraw
                grid[i].discharge.NO3withdraw += (grid[i].discharge.wd_demand/count)/fin.wat*fin.no3;

                fin.no3 -= (grid[i].discharge.wd_demand/count)/fin.wat*fin.no3;

                fin.wat -= grid[i].discharge.wd_demand/count;

            }
            else
            {
                grid[i].discharge.withdrawal+=fin.wat;
                grid[i].discharge.mfout+=fin.wat;

                //move the nutrient taken with the water withdraw
                grid[i].discharge.NO3withdraw += fin.no3;

                fin.no3 = 0.0;
                fin.wat=0.0;

            }



            /* the remainder enters the river system */
            grid[i].discharge.dmass_river += fin.wat;
            putqueue(grid[i].discharge.queue, fin.wat);
            grid[i].discharge.dmass_sum += grid[i].discharge.dmass_river + grid[i].discharge.dmass_lake + sumqueue(grid[i].discharge.queue);

            //add all the wastewater to the river. As PO4was, NH4was ... are daily variables we only add them at the first iteration
            if(config->with_wastewater && iter==0)
            {
                fin.no3 +=  grid[i].discharge.NO3was;
                grid[i].output.mNWastewater += grid[i].discharge.NO3was +grid[i].discharge.NH4was;

                fin.po4 +=  grid[i].discharge.PO4was;
                grid[i].output.mPO4Wastewater += grid[i].discharge.PO4was;

                fin.nh4 +=  grid[i].discharge.NH4was;
                fin.don +=  grid[i].discharge.DONwas;
                fin.dop +=  grid[i].discharge.DOPwas;

            }

            grid[i].discharge.NO3riv += fin.no3;

            putqueue(grid[i].discharge.NO3Queue, fin.no3);

            grid[i].discharge.PO4riv += fin.po4;
            putqueue(grid[i].discharge.PO4Queue, fin.po4);

            grid[i].discharge.PO4adsriv += fin.po4ads;
            putqueue(grid[i].discharge.PO4adsQueue, fin.po4ads);

            grid[i].discharge.NH4riv += fin.nh4;
            putqueue(grid[i].discharge.NH4Queue, fin.nh4);

            grid[i].discharge.DOCriv += fin.doc;
            putqueue(grid[i].discharge.DOCQueue, fin.doc);

            grid[i].discharge.DONriv += fin.don;
            putqueue(grid[i].discharge.DONQueue, fin.don);

            grid[i].discharge.DOPriv += fin.dop;
            putqueue(grid[i].discharge.DOPQueue, fin.dop);

            grid[i].discharge.PHYriv += fin.phy;
            putqueue(grid[i].discharge.PHYQueue, fin.phy);

            grid[i].discharge.DECCriv += fin.decc;
            putqueue(grid[i].discharge.DECCQueue, fin.decc);

            grid[i].discharge.RESriv += fin.res;
            putqueue(grid[i].discharge.RESQueue, fin.res);

        } /* of 'for(i=...)' */

    } /* of 'for(iter=...)' */

    grid += config->startgrid-config->firstgrid; /* re-adjust first index of grid array */


    /* water withdrawal from the lake */
    for(cell=0;cell<config->ngridcell;cell++)
    {
        grid[cell].output.mwateramount+=grid[cell].discharge.dmass_sum/count;
        grid[cell].discharge.dmass_sum=0.0;
        /* if the water withdrawn from the outflow of lake and reservoir isn't enough we take in the lake*/
        if(grid[cell].discharge.withdrawal<grid[cell].discharge.wd_demand)
        {
            if(grid[cell].discharge.dmass_lake>grid[cell].discharge.wd_demand-grid[cell].discharge.withdrawal)
            {
                grid[cell].discharge.NO3withdraw += update_drain(grid[cell].discharge.wd_demand-grid[cell].discharge.withdrawal, &grid[cell].discharge.NO3lake, grid[cell].discharge.NO3lake/grid[cell].discharge.dmass_lake);
                grid[cell].discharge.dmass_lake-=grid[cell].discharge.wd_demand-grid[cell].discharge.withdrawal;
                grid[cell].discharge.mfout+=grid[cell].discharge.wd_demand-grid[cell].discharge.withdrawal;


                grid[cell].discharge.withdrawal=grid[cell].discharge.wd_demand;
            }
            else
            {
                grid[cell].discharge.mfout+=grid[cell].discharge.dmass_lake;
                grid[cell].discharge.withdrawal+=grid[cell].discharge.dmass_lake;

                grid[cell].discharge.NO3withdraw += grid[cell].discharge.NO3lake;

                grid[cell].discharge.NO3lake =0.0;

                grid[cell].discharge.dmass_lake=0.0;
            }
        }
        grid[cell].output.mwd_local+=grid[cell].discharge.withdrawal/grid[cell].coord.area; /* withdrawal in local cell */
    }

    /* barrier.n: update of nutrients in lakes and rivers */
    for (cell=0; cell<config->ngridcell; cell++)
    {
        //todo add nit update reservoir


        nit_update_river(grid + cell, config, cell);
        nit_update_lakes(grid + cell,cell);

    }



}  /* of 'drain' */
