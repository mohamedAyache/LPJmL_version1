/***************************************************************************/
/**                                                                       **/
/**                        m  a  n  a  g  e  .  h                         **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 03.05.2007, Marlies Gumpenberger                     **/
/**     Last change: 23.10.2008, J. Heinke                                **/
/**     Last change: $Date:: 2015-07-24 11:22:32 +0200 (ven. 24 juil. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef MANAGE_H /* Already included? */
#define MANAGE_H

#include "managepar.h"

/* Declaration of datatypes */

typedef struct
{
  short country,region;
} Code;

typedef struct
{
  int id;     /* country id (0-196)*/
  char *name; /* country name */
  int default_irrig_system; /* default irrig system at country-level (SURF=1,SPRINK=2,DRIP=3), used for ALLCROPS or ALL_IRRIGATION */
  Real laimax_tempcer;  /*laimax for temperate cereals*/
  Real laimax_maize;    /*laimax for maize*/
  /* parameters defined for each country */
  Real *laimax_cft;  /* pointer to crop-specific laimax */
  Real *k_est; 
} Countrypar;


typedef struct
{
  int id;     /* region id (0-432)*/
  char *name; /* region name */
  Real bifratio;   /* Yevich's data/rharvest von LPJ; for all crops (no rharvest for grass)*/
  Real fuelratio;  /* Yevich's data (withoutdung)/rharvest von LPJ; for all crops (no rharvest for grass)*/
  Real woodconsum; /* domestic firewood consumption [kgDM/capita/day]*/
} Regionpar; /* parameters defined for each region */


typedef struct
{
  const Countrypar *par;    /* pointer to country-specific parameter */
  const Regionpar *regpar;  /* pointer to region-specific parameter */
  Real *laimax;             /* maximum crop specific LAI */
  Real *k_est;
} Manage;

/* Declaration of functions */

extern int fscancountrypar(FILE *,Countrypar **,Bool,Bool,Bool);
extern int fscanregionpar(FILE *,Regionpar **,Bool);
extern int fscantreedens(FILE *,Countrypar *,int, int, Bool);
extern void fprintcountrypar(FILE *,const Countrypar *,int,int);
extern void fprintregionpar(FILE *,const Regionpar[],int);
extern void initmanage(Manage *, const Countrypar *, const Regionpar *,int,
                       int,int,Bool);
extern void freemanage(Manage *,int);
extern FILE *opencountrycode(const Filename *,Bool *,String,int *,Bool);
extern FILE *createcountrycode(const char *,int,int);
extern Bool readcountrycode(FILE *,Code *,Bool);
extern void freecountrypar(Countrypar [],int);
extern void freeregionpar(Regionpar [],int);


/* Definitions of macros */

#define fwritecountrycode(file,code,n) fwrite(code,sizeof(Code),n,file)
#define seekcountrycode(file,offset,name,version) fseek(file,headersize(name,version)+(offset)*sizeof(Code),SEEK_SET)

#endif
