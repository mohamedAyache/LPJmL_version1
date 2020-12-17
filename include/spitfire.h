/***************************************************************************/
/**                                                                       **/
/**                 s  p i  t  f  i  r  e  .  h                           **/
/**                                                                       **/
/**     Header file for Spitfire fire model                               **/
/**                                                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 03.03.07 Ben Poulter                                 **/
/**     Last change: 12.03.09 Werner von Bloh                             **/
/**                                                                       **/
/***************************************************************************/

#ifndef SPITFIRE_H /* Already included? */
#define SPITFIRE_H

/* Declaration of datatypes */

typedef struct popdens *Popdens;

typedef struct
{
  Real human;
  Real nesterov_accum;   
} Ignition;

/* Declaration of functions */

extern Real area_burnt(Real,Real,Real,Real, int,const Pftlist *);
extern Real deadfuel_consumption(const Litter *,Fuel *, Real);
extern Real fuel_consum_total(const Litter *,const Fuel *);
extern Real firedangerindex(Real,Real,Real);
extern Real fire_emissions(Real);
extern Real firemortality_tree(Pft *,const Fuel *,Livefuel *,Real,Real);
extern void fraction_of_consumption(Fuel *);
extern Real fuel_consumption_1hr(Real, Real);
extern void fuelload(Stand*,Fuel*,Livefuel *,Real);
extern void initfuel(Fuel *);
extern void freepopdens(Popdens,Bool);
extern Real getpopdens(const Popdens,int);
extern Bool readpopdens(Popdens,int,const Cell *,const Config *);
extern Real humanignition(Real,Ignition *);
extern Bool initignition(Cell *,const Config *);
extern Popdens initpopdens(const Config *);          
extern Real litter_update_fire(Litter *,const Fuel *);
extern Real rateofspread(Real,Fuel *);
extern Real surface_fire_intensity(Real, Real, Real);
extern void update_fbd_tree(Litter*,Real,Real,int);
extern void update_fbd_grass(Litter*,Real,Real);
extern Real wildfire_ignitions(Real,Real,Real);
extern Real windspeed_fpc(Real,const Pftlist *);
extern void dailyfire(Stand *,Livefuel *,Real,Dailyclimate,int);

/* Definition of constants */

#define MINER_TOT 0.055

/* Definition of macros */

#define nesterovindex(tmin,tmax) ((tmax)*(tmax-((tmin)-4)))

#endif
