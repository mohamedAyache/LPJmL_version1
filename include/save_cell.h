/***************************************************************************/
/**                                                                       **/
/**                      c  e  l  l  .  h                                 **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     Declaration of cell datatype                                      **/
/**                                                                       **/
/**     Hierarchy:                                                        **/
/**                                                                       **/
/**     --cell                                                            **/
/**       +-Coord                                                         **/
/**       +-Managed_land                                                  **/
/**       | +-Landfrac                                                    **/
/**       | +-Resdata                                                     **/
/**       | | +-Reservoir                                                 **/
/**       | +-Cropdates                                                   **/
/**       | +-Manage                                                      **/
/**       | +-Image_data (only for version with -DIMAGE set)              **/
/**       |   +-Pool                                                      **/
/**       +-Ignition                                                      **/
/**       +-Discharge                                                     **/
/**       | +-Queue                                                       **/
/**       +-Output                                                        **/
/**       +-Climbuf                                                       **/
/**       | +-Buffer                                                      **/
/**       +-Balance                                                       **/
/**       +-Standlist                                                     **/
/**         +-Stand                                                       **/
/**           +-Pftlist                                                   **/
/**           | +-Pft                                                     **/
/**           +-Soil                                                      **/
/**             +-Pool                                                    **/
/**             +-Litter                                                  **/
/**               +-Litteritem                                            **/
/**                 +-Trait                                               **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     See file AUTHORS for list of authors contributing to this code    **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-24 11:22:32 +0200 (ven. 24 juil. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef CELL_H /* Already included? */
#define CELL_H

/* Definition of datatypes */


typedef struct
{
  Real nep;                 /* annual NEP (gC/m2) */
  Real awater_flux;         /* annual water flux (mm) */
  Real aprec;               /* annual precipitation (mm) */
  Real totc;                /* total carbon (gC/m2) */
  Real biomass_yield;       /* harvested biomass (gC/m2)*standfrac*/
  Real estab_storage_tree[2];  /* carbon taken out from annual NPP to satisfy tree establishment rate */
  Real estab_storage_grass[2]; /* carbon taken out from annual NPP to satisfy grass establishment rate */
  Real totw;                /* total water (mm) */
  Real surface_storage;     /* total water in surface storages (dm3) */
  Real soil_storage;        /* total water in soil storages (dm3) */
  Real total_reservoir_out; /* total water extracted from reservoirs (dm3) */
  Real total_irrig_from_reservoir; /* total water added to fields from reservoirs (dm3)*/
} Balance;

typedef struct celldata *Celldata;

struct cell
{
  Coord coord;              /* Cell coordinate and area */
  Standlist standlist;      /* Stand list */
  Climbuf climbuf;
  Ignition ignition;
  Real afire_frac;          /* fraction of grid cell burnt this year */
  Real *gdd;                /* Growing degree days array */
  Real lakefrac;            /* lake fraction (0..1) */
  Real k_perc;              /* percolation parameter */
  Bool skip;                /* Invalid soil code in cell (TRUE/FALSE) */
  Managed_land ml;          /* Managed land */
  Output output;            /* Output data */
  Discharge discharge;
  int elevation;            /* cell elevation (m) */
  Balance balance;          /* balance checks */
};

/* Declaration of functions */

extern void freegrid(Cell [],int,const Config *);
extern void freecell(Cell *,int,Bool);
extern void update_daily(Cell *,Real,Real,Dailyclimate,int,
                         int,int,int,int, Bool,Bool,const Config *);
extern void update_annual(Cell *,int,int,
                          Real,int,Bool,const Config *);
extern void update_monthly(Cell *,Real,Real,int);
extern void init_annual(Cell *,int,int,int,int);
extern int fwritecell(FILE *,long long [],const Cell [],int,int,int,int,int,Bool);
extern void fprintcell(FILE *,const Cell [],int,int,Bool,Bool);

extern Bool freadcell(FILE *,Cell *,const Pftpar[],int,int,int,const Soilpar *,
                      const Standtype [],int,Bool,int,int,Bool,Bool);

extern int writecoords(Outputfile *,int,const Cell [],const Config *);
extern int writecountrycode(Outputfile *,int,const Cell [],const Config *);
extern int writeregioncode(Outputfile *,int,const Cell [],const Config *);
extern int iterate(Outputfile *,Cell [],Input,
                   int,int,const Config *);
extern void iterateyear_river(Outputfile *,Cell [],Input,
                              Real,int,int,int,const Config *);
extern void iterateyear(Outputfile *,Cell [],Input,Real,
                        int,int,int,const Config *);
extern void fwriteoutput_annual(Outputfile *,const Cell [],int,const Config *);
extern void fwriteoutput_monthly(Outputfile *,const Cell [],int,int,const Config *);
extern void fwriteoutput_daily(Outputfile *,const Cell [],int,int,const Config *);
extern void fwriteoutput_pft(Outputfile *,Cell [],int,int,int,const Config *);
extern void equilsom(Cell *,int, const Pftpar []);
extern void equilveg(Cell *);
extern void check_fluxes(Cell *,int,int,const Config *);
extern void check_balance(Flux,int,const Config *);
extern Bool initdrain(Cell [],Config *);
extern void drain(Cell [],const Config *,int);
extern Real nep_sum(const Cell [],const Config *);
extern Real cflux_sum(const Cell [],const Config *);
extern Real flux_sum(Flux *,Cell [],const Config *);
extern Bool getwateruse(Wateruse, Cell [],int,const Config *);
extern Wateruse initwateruse(const Config *);
extern void freewateruse(Wateruse,Bool);
extern void killstand(Cell *,const Pftpar [],int,Bool);
extern Bool initsoiltemp(Climate *, Cell*,const Config *);
extern Celldata opencelldata(Config *);
extern Bool seekcelldata(Celldata,int,int);

extern Bool readcelldata(Celldata,Coord *,int *,Real *,int,Config *);

extern void closecelldata(Celldata);
extern Real albedo(const Cell *);

/* Definition of macros */

#define printcell(cell,n,cft,landuse,river_routing) fprintcell(stdout,cell,n,cft,landuse,river_routing)

#endif
