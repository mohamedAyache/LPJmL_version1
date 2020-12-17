/***************************************************************************/
/**                                                                       **/
/**                     n  a  t  u  r  a  l  .  h                         **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Declaration of natural stand                                      **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-15 12:26:32 +0200 (mer. 15 juil. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef NATURAL_H
#define NATURAL_H

extern Standtype natural_stand;

extern Real daily_natural(Stand *,Real,Dailyclimate,int,
                          Real,const Real [],
                          Real,Real,Real,Real,
                          Real,Real,Real,int,int,int,Bool,Bool,const Config *,int);
extern Bool annual_natural(Stand *,int,int,Real,int,Bool,const Config *);

extern void dailyfire_natural(Stand *,Output *,Livefuel *,Real *,Real,
                              Dailyclimate,const Pftpar *,int);
extern Real woodconsum_natural(Stand*, Pftlist *,Litter *,Real,Real,Real,int);

extern void new_natural(Stand *);
extern void free_natural(Stand *);
extern Bool fread_natural(FILE *,Stand *,Bool);
extern Bool fwrite_natural(FILE *,const Stand *);
extern void fprint_natural(FILE *,const Stand *);

#endif
