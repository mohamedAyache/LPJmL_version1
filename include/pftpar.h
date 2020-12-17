/***************************************************************************/
/**                                                                       **/
/**                    p  f  t  p  a  r  .  h                             **/
/**                                                                       **/
/**     Pft parameter header, should be included in data file             **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 25.01.2006 by S.Jachner, M.Gumpenberger              **/
/**     Last change: $Date::                                            $ **/
/**     By         : $Author::                                 $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef PFTPAR_H /* Already included? */
#define PFTPAR_H

/* Definition of constant for pft parameter */

#define EVERGREEN 0
#define RAINGREEN 1
#define SUMMERGREEN 2
#define ANY 3
#define CROPGREEN 4
#define NOPATHWAY 0
#define C3 1
#define C4 2
#define BROADLEAVED 0
#define NEEDLELEAVED 1
#define ANYLEAVED 2
#define GRASS 0
#define TREE 1
#define CROP 2
#define NONE 0
#define BIOMASS 1
#define ANNUAL_CROP 2
#define ANNUAL_TREE 3

/* CROP must always last id because of ntypes, see also lpjml.c */
#define TROPICAL_BROADLEAVED_EVERGREEN_TREE 0
#define TROPICAL_BROADLEAVED_RAINGREEN_TREE 1
#define TEMPERATE_NEEDLELEAVED_EVERGREEN_TREE 2
#define TEMPERATE_BROADLEAVED_EVERGREEN_TREE 3
#define TEMPERATE_BROADLEAVED_SUMMERGREEN_TREE 4
#define BOREAL_NEEDLELEAVED_EVERGREEN_TREE 5
#define BOREAL_BROADLEAVED_SUMMERGREEN_TREE 6
#define C3_PERENNIAL_GRASS 7
#define C4_PERENNIAL_GRASS 8
#define BIOENERGY_TROPICAL_TREE 9
#define BIOENERGY_TEMPERATE_TREE 10
#define BIOENERGY_C4_GRASS 11
#define CITRUS 12
#define FRUITS 13
#define DATE_PALM 14
#define OLIVES 15
#define NUTS 16
#define GRAPES 17
#define VEGETABLES 18
#define COTTON 19
#define FODDER_GRASS 20
#define TEMPERATE_CEREALS 21
#define RICE 22
#define MAIZE 23
#define TROPICAL_CEREALS 24
#define PULSES 25
#define TEMPERATE_ROOTS 26
#define POTATOES 27
#define TROPICAL_ROOTS 28
#define OIL_CROPS_SUNFLOWER 29
#define OIL_CROPS_SOYBEAN 30
#define OIL_CROPS_GROUNDNUT 31
#define OIL_CROPS_RAPESEED 32
#define SUGARCANE 33

#define NO_CALC_SDATE 0
#define PREC_CALC_SDATE 1
#define TEMP_WTYP_CALC_SDATE 2
#define TEMP_STYP_CALC_SDATE 3
#define TEMP_PREC_CALC_SDATE 4
#define MULTICROP 5

#define NGRASS 2 /* OTHERS + MANAGED GRASSLAND */
#define NBIOMASSTYPE 2 /* TREE + GRASS */
#define WIRRIG 2

#define ALLNATURAL -1 /* ensures summing daily outputs for all natural vegetation pfts */

#endif
