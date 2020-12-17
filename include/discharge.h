/***************************************************************************/
/**                                                                       **/
/**                 d  i  s  c  h  a  r  g  e  .  h                       **/
/**                                                                       **/
/**     C implementation of river routing in LPJ                          **/
/**                                                                       **/
/**     LPJ header file for river routing                                 **/
/**                                                                       **/
/**     written by Stefanie Rost, Werner von Bloh                         **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer. 22 juil. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef DISCHARGE_H /* Already included? */
#define DISCHARGE_H

#define v 86400.0 /* velocity (m/d) */
#define kr 0.001 /* lake outflow coefficient (1/d) */
#define H 5 /* maximum active lake storage depth (m) */
#define TSTEP (1.0/8.0)   /* time step in drain (d) */
#define SEGLEN 10000       /* length of river segment treated as a single storage in m */
#define TAU (SEGLEN/v)     /* time constant for a single storage in d */


/* Definition of datatypes */

typedef struct
{
    int index; /* index to outflow cell */
    int len;   /* river length (m) */
} Routing;

typedef struct
{
    Real dmass_lake;        //dm3
    Real dmass_lake_max;
    Real dmass_river;
    Real dmass_sum;         /* sum of dmass_lake, dmass_new, dmass_queue (dm3) */
    Real drunoff;           /* daily runoff (mm) */
    Real dfout;
    Real fout;
    Real mfin;              /* pool for all incoming water fluxes into cell, used to close cell water balance in update_monthly and check_fluxes */
    Real mfout;             /* pool for all water fluxes leaving the cell, used to close cell water balance in update_monthly and check_fluxes */
    Real waterdeficit;
    Real gir;               /* gross irrigation requirements, including net irrigation + distribution requirements and conveyance losses */
    Real act_irrig_amount_from_reservoir;
    Real wd_demand;         /* sum of daily withdrawal demand from irrigation and hil */
    Real wd_deficit;        /* withdrawal demand that can not be fulfilled locally */
    Real wd_neighbour;      /* sum of withdrawal requests from neighbour cells */
    Real withdrawal;        /* amount of water withdrawn from river and lake */
    Real irrig_unmet;       /* amount of irrigation water not met by local sources or neighbor cell */
    Real wateruse;          /* water consumption for industry,household and livestock, read from input file if specified in lpjml.conf */
    Queue queue;            /* Delay queue */
    Real *tfunct;           /* pointer to the factors of the cell's transfer function */
    int next;               /* index to outflow cell */

    Real NO3out;            /* discharge of NO3 g*/
    Real NH4out;            /* discharge of NH4 g*/
    Real PO4out;            /* discharge of PO4 g*/
    Real PO4adsout;            /* discharge of adsorbed PO4 g*/
    Real DOCout;            /* discharge of DOC g*/
    Real DONout;            /* discharge of DON g*/
    Real DOPout;            /* discharge of DOP g*/
    Real PHYout;            /* discharge of phytoplankton */
    Real RESout;            /* discharge of water residues */
    Real DECCout;            /* discharge of water decomposers */

    Real NO3riv;            /* NO3 in rivers g*/
    Real NH4riv;            /* NH4 in rivers g*/
    Real PO4riv;            /* PO4 in rivers g*/
    Real PO4adsriv;            /* PO4 adsorbed in rivers g*/
    Real DOPriv;            /*DOP in rivers g*/
    Real DONriv;          /* DON in rivers g*/
    Real DOCriv;          /* DOC in rivers g*/
    Real DECCriv;         /* Decomposers in rivers */
    Real RESriv;          /* Residues in rivers */
    Real PHYriv;          /* Phyto in rivers */

    Real NO3lake;            /* NO3 in lake g*/
    Real NH4lake;            /* NH4 in lake g */
    Real PO4lake;            /* PO4 in lake g*/
    Real PO4adslake;         /* PO4 adsorbed in lake g*/
    Real DOPlake;           /*DOP in lakes g*/
    Real DONlake;           /*DON in lakes g*/
    Real DOClake;           /*DOC in lakes g*/
    Real DECClake;           /*DECC in lakes */
    Real RESlake;           /*RES in lakes */
    Real PHYlake;         /* Phyto in lakes */

    Real NO3runoff;    /*NO3 runoff  gN.m^{-2} */
    Real NH4runoff;    /*NH4 runoff gN.m^{-2}*/
    Real PO4runoff;    /*PO4 runoff gN.m^{-2}*/
    Real PO4adsrunoff;    /*PO4 adsorbed runoff gN.m^{-2}*/
    Real DOPrunoff;    /*DOP runoff gN.m^{-2}*/
    Real DOCrunoff;    /*DOC runoff gN.m^{-2}*/
    Real DONrunoff;    /*DON runoff gN.m^{-2}*/

    Real NO3was;   /* NO3 quantity into wastewaters g*/
    Real NH4was;   /* NH4 quantity into wastewaters g*/
    Real PO4was;   /* PO4 quantity into wastewaters g*/
    Real DOPwas;   /* DOP quantity into wastewaters g*/
    Real DOCwas;   /* DOC quantity into wastewaters g*/
    Real DONwas;   /* DON quantity into wastewaters g*/

    Real NO3withdraw; /*NO3 quantity in irrigation water g*/

    /* barrier.n: according to Werner, these are "First In, First Out queues",
     * because "river water may flow at the poles through more than one cell"
     */
    Queue NO3Queue;
    Queue NH4Queue;
    Queue PO4Queue;
    Queue PO4adsQueue;
    Queue DOPQueue;
    Queue DONQueue;
    Queue DOCQueue;
    Queue PHYQueue;
    Queue DECCQueue;
    Queue RESQueue;

} Discharge;

typedef struct wateruse *Wateruse;

/* Declaration of functions */

extern void wateruse(Cell *,int,int,const Config *);
extern void withdrawal_demand(Cell *,const Config *);
extern void distribute_water(Cell *,int,int,int,int,int);
extern Real *transfer_function(Real,int *);
extern Bool getroute(FILE *,Routing *,Bool);

#endif
