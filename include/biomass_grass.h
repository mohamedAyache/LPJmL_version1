/***************************************************************************/
/**                                                                       **/
/**                     b  i  o  m  a  s  s  _  g  r  a  s  s  .  h       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Declaration of biomass  stand                                     **/
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

#ifndef BIOMASS_GRASS_H
#define BIOMASS_GRASS_H

extern Standtype biomass_grass_stand;

extern Real daily_biomass_grass(Stand *,Real, Dailyclimate, int,
                            Real,const Real [],
                            Real ,Real,Real,Real,Real,Real,
                            Real ,int, int, int, Bool,Bool,const Config *,int);
extern Bool annual_biomass_grass(Stand *,int, int,Real,int,Bool,const Config *);
extern void output_gbw_biomass_grass(Output *,const Stand *,Real,Real,Real,Real,
                                 const Real [LASTLAYER],const Real [LASTLAYER],Real,Real,int,int,
                                 int,Bool);

#endif
