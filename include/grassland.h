/***************************************************************************/
/**                                                                       **/
/**                     g  r  a  s  s  l  a  n  d  .  h                   **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Declaration of grassland stand                                    **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer. 22 juil. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef GRASSLAND_H
#define GRASSLAND_H

#ifdef NEW_GRASS
/* Definition of settings */

#define DEMAND_COW_INT 4000.0         // [g.C.day-1] same for extensive and intensive grazing;
#define DEMAND_COW_EXT 4000.0         // corresponds to 8.9 kg DW which is in between the observed values of 6 and 12.

#define STUBBLE_HEIGHT_MOWING 25      // [g.C.m-2] equals 5 cm. The leaf biomass remaining on paddock after mowing.

#define STOCKING_DENSITY_EXT 0.5      // [LSU.ha-1] (LiveStockUnit). Stocking densities for
#define STOCKING_DENSITY_INT 1.2      // both grazing systems are rough estimates.

#define MANURE 0.25                   // [-] 25 % portion of grazed biomass going to the fast soil litter pool; literature value

#define MAX_PADDOCKS 16               // Rough estimate from Faustzahlen.

#define MAX_ROTATION_LENGTH 50        // [days] Rough estimate when usually rotations take 20 to 35 days
#define MIN_ROTATION_LENGTH 5         // [days] Rough estimate

#define GRAZING_STUBLE 5              // [g.C.m-2] Grazing stops when less than this threshold is left in the leaves

#define STUBBLE_HEIGHT_GRAZING_EXT 5  // [g.C.m-2] Minimal threshold to start extensive grazing
#define STUBBLE_HEIGHT_GRAZING_INT 40 // [g.C.m-2] equals 7-8 cm. Threshold to start intensive grazing
extern Bool isMowingDay(int);
#endif

extern Standtype grassland_stand;

extern Real daily_grassland(Stand *,Real, Dailyclimate, int,
                            Real,const Real [],
                            Real ,Real,Real,Real,Real,Real,
                            Real ,int, int, int,  Bool,Bool,const Config *,int);
extern Bool annual_grassland(Stand *,int,int,Real,int,Bool,const Config *);
extern void output_gbw_grassland(Output *,const Stand *,Real,Real,Real,Real,
                                 const Real [LASTLAYER],const Real [LASTLAYER],Real,Real,int,int,
                                 int,Bool);

#endif
