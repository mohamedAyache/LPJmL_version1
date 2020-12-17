/***************************************************************************/
/**                                                                       **/
/**                      b  i  o  m  e  s  .  h                           **/
/**                                                                       **/
/**     extension of LPJ to classify biomes based on FPC values only      **/
/**                                                                       **/
/**     written by Christoph Mueller and Bart Strengers                   **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 07.01.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#ifndef BIOMES_H /* already included? */
#define BIOMES_H

/* definition of constants for biomes */

#define BOREAL_DECIDUOUS_FOREST 1
#define BOREAL_EVERGREEN_FOREST 2
#define TEMPERATE_BOREAL_MIXED_FOREST 3
#define TEMPERATE_CONIFEROUS_FOREST 4
#define TEMPERATE_DECIDUOUS_FOREST 5
#define TEMPERATE_BROADLEAVED_EVERGREEN_FOREST 6
#define TEMPERATE_MIXED_FOREST 7
#define TROPICAL_SEASONAL_FOREST 8
#define TROPICAL_RAIN_FOREST 9
#define TROPCIAL_DECIDUOUS_FOREST 10
#define MOIST_SAVANNAH 11
#define DRY_SAVANNAH 12
#define TROPICAL_GRASS 13
#define SHRUB 14
#define ARID_SHRUB_STEPPE 15
#define WOOD_TUNDRA 16
#define TUNDRA 17
#define COOL_GRASS 18
#define DESERT 19
#ifdef IMAGE
#define ARCTIC_TUNDRA TUNDRA /* no differentiation for IMAGE */
#define XERIC_SHRUBS SHRUB /* no differentiation for IMAGE */ 
#define TALL_GRASSLAND 0
#define SHORT_GRASSLAND 0
#else
#define ARCTIC_TUNDRA 20
#define XERIC_SHRUBS 21
#define TALL_GRASSLAND 22
#define SHORT_GRASSLAND 23
#endif
#define MANAGED_LAND 24

/* Declaration of functions */

extern int biome_classification(Real,const  Stand *,int);

#endif /*BIOMES_H*/
