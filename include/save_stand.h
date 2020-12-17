/***************************************************************************/
/**                                                                       **/
/**                    s  t  a  n  d  .  h                                **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     Header file for implementation of stands in LPJ                   **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer. 22 juil. #$  **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef STAND_H  /* Already included? */
#define STAND_H

/* Definition of datatypes */

typedef struct
{
  Landusetype landusetype;
  char *name;
  void (*newstand)(Stand *);
  void (*freestand)(Stand *);
  Bool (*fwrite)(FILE *,const Stand *);
  Bool (*fread)(FILE *,Stand *,Bool);
  void (*fprint)(FILE *,const Stand *);
  Real (*daily)(Stand *,Real,Dailyclimate,int,Real,
                const Real [],Real,Real,Real,Real,Real,
                Real,Real,int,int,int,Bool,Bool,const Config *);
  Bool (*annual)(Stand *,int,int,
                 Real,int,Bool,const Config *);
  void (*dailyfire)(Stand *,Livefuel *,Real,Dailyclimate,int);
} Standtype;

struct stand
{
  const Standtype *type;
  struct cell *cell;          /* pointer to cell */
  Pftlist pftlist;            /* List of establishd PFTs */
  Soil soil;                  /* soil specific variables */
  Real fire_sum;
  Real frac;                  /* Stand fraction (0..1) */
  Real frac_g[NSOILLAYER];    /* fraction of green water in total available soil water, including free water */
  int growing_time;           /*for TREES years since harvest*/
  int growing_days;           /*for GRASS days since harvest*/
  int age;                    /*PLANTATION AGE*/
  int pft_id;
  void *data;                 /* stand-specific extensions */
};

typedef List *Standlist;

/* Declaration of functions */

extern Bool fwritestand(FILE *,const Stand *,int);
extern void fprintstand(FILE *,const Stand *);
extern int fwritestandlist(FILE *,const Standlist,int);
extern void fprintstandlist(FILE *,const Standlist);

extern Stand *freadstand(FILE *,struct cell *,const Pftpar[],int,
                         const Soilpar *,const Standtype [],int,Bool);

extern Standlist freadstandlist(FILE *,struct cell *,const Pftpar [],int,
                                const Soilpar *,const Standtype [],int,Bool);

extern int addstand(const Standtype *,struct cell *);
extern void initstand (Stand *);
extern void freestand(Stand *);
extern int delstand(Standlist,int);
extern void freestandlist(Standlist);
extern void mixsoil(Stand *,const Stand *);
extern Bool check_lu(const Standlist ,Real,int,Bool);
extern void check_stand_fracs(const Standlist,Real);
extern int findstand(const Standlist, Landusetype, Bool);
extern int findstandpft(const Standlist,int, Bool);
extern int findlandusetype(const Standlist,Landusetype);
extern void allocation_today(Stand *, int);
extern void light(Stand *,int,const Real[]);
extern Real establishmentpft(Stand *,const Pftpar[],int,int,Real,int);
extern Real standcarbon(const Stand *);
extern void cutpfts(Stand *);
extern Harvest harvest_grass(Stand *,Real);
extern Real roughnesslength(const Standlist);

extern void waterbalance(Stand *,Real [BOTTOMLAYER],Real [BOTTOMLAYER],Real *,Real *,Real,Real,
                         Real,Real *,Bool);
extern Real infil_perc_irr(Stand *,Real,Real *,Bool);
extern Real infil_perc_rain(Stand *,Real,Real *,Bool);

/* Definition of macros */

#define getstand(list,index) ((Stand *)getlistitem(list,index))
#define foreachstand(stand,i,list) for(i=0;i<getlistlen(list) && (stand=getstand(list,i));i++)
#define printstandlist(standlist,pftlist) fprintstandlist(stdout,standlist,pftlist)

/*
 * The following macros allow to call the stand-specific functions like virtual
 * functions in C++
 */

#define daily_stand(stand,co2,climate,day,daylength,gp_pft,gtemp_air,gtemp_soil,gp_stand,gp_stand_leafon,pet,par,melt,npft,ncft,year,withdaily,intercrop,config) stand->type->daily(stand,co2,climate,day,daylength,gp_pft,gtemp_air,gtemp_soil,gp_stand,gp_stand_leafon,pet,par,melt,npft,ncft,year,withdaily,intercrop,config)
#define annual_stand(stand,npft,ncft,popdens,year,intercrop,config) stand->type->annual(stand,npft,ncft,popdens,year,intercrop,config)
#define dailyfire_stand(stand,livefuel,popdens,climate,ntypes) if(stand->type->dailyfire!=NULL) stand->type->dailyfire(stand,livefuel,popdens,climate,ntypes)

#endif
