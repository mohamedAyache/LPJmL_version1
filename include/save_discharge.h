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
  Real dmass_lake;
  Real dmass_lake_max;
  Real dmass_river;
  Real dmass_sum;         /* sum of dmass_lake, dmass_new, dmass_queue (m3) */
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
  Real withdrawal;        /* amount of water withdrawn from river */
  Real irrig_unmet;       /* amount of irrigation water not met by local sources or neighbor cell */
  Real wateruse;          /* water consumption for industry,household and livestock, read from input file if specified in lpjml.conf */
  Queue queue;            /* Delay queue */
  Real *tfunct;           /* pointer to the factors of the cell's transfer function */
  int next;               /* index to outflow cell */
} Discharge;

typedef struct wateruse *Wateruse;

/* Declaration of functions */

extern void wateruse(Cell *,int,int,const Config *);
extern void withdrawal_demand(Cell *,const Config *);
extern void distribute_water(Cell *,int,int,int,int,int);
extern Real *transfer_function(Real,int *);
extern Bool getroute(FILE *,Routing *,Bool);

#endif
