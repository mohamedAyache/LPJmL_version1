/***************************************************************************/
/**                                                                       **/
/**                     p  f  t  .  h                                     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Declaration of plant functional type (PFT) datatype               **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 19.08.2009  by Christoph Mueller & Elke Stehfest     **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer. 22 juil. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef PFT_H /* Already included? */
#define PFT_H

#include "pftpar.h"

/* Definition of constants */

#define NO_WDF -9
#define PREC_MAX 1e38
/*#define GM 3.26       empirical parameter in demand function */
/*#define ALPHAM 1.391  Priestley-Taylor coefficient (Demand,maximum-value)*/
#define LAMBDA_OPT 0.8  /* optimal Ci/Ca ratio */
#define NHSG 4 /* number of hydrological soil groups */

/* Definitions of datatypes */

typedef struct
{
  Real char_moist_factor;
  Real char_alpha_fuel;    /* parameter to calculate fuel moisture */
  Real char_net_fuel;
  Real char_dens_fuel_ave; /* average fbd */
  Real cf;
  Real daily_litter_moist; /* fuel moisture */
  Real deadfuel_consum[NFUELCLASS+1];
  Real gamma;
  Real moist_1hr;
  Real moist_10_100hr;
  Real mw_weight;
  Real sigma;
} Fuel;

typedef struct
{
  Real disturb;
  Real dlm_livegrass;
  Real non_combust;
  Real pot_fc_lg_c3; /* Biomass of C3 grass in g/m2 */
  Real pot_fc_lg_c4; /* Biomass of C4 grass in g/m2 */
} Livefuel;

typedef struct
{
  Real co2;
  Real co;
  Real ch4;
  Real voc;
  Real tpm;
  Real nox;
} Tracegas;

typedef struct
{
  Real low,high; /* lower and upper tolerance limits */
} Limit;

struct stand; /* forward declaration of datatype Stand */

typedef struct Pft
{
  const struct Pftpar
  {
    int id;                     /* unique PFT identifier */
    int type;                   /* type --> whether CROP or TREE or GRASS*/
    int cultivation_type;       /* cultivation_type----> NONE, BIOMASS, ANNUAL_CROP*/
    char *name;                 /* Pft name */
    Real cn[NHSG];              /* pft specific curve number for each hydr. soil group */
    Real beta_root;        /* root distribution parameter */
    Real rootdist[LASTLAYER];/* fraction of roots in upper soil layer par1*/
    Real minwscal;              /* water scalar value at which leaves shed by
                                 drought deciduous PFT par3 */
    Real gmin;                  /* canopy conductance component (4) */
    Real respcoeff;             /* maintenance respiration coefficient (5) */
    Real nmax;                  /* maximum foliar N content (mg/g) (7) */
    Real resist;                /* fire resistance index (8) */
    Real longevity;             /* leaf longevity (10) */
    Real lmro_ratio;            /* leaf to root ratio under non-water stressed
                                   conditions (18) */
    Real ramp;                  /* number of GDDs to attain full leaf cover
                                   (par19) */
    Real gdd5min;               /* PFT-specific minimum GDD(30) */
    Real twmax;                 /* (31) */
    Real gddbase;/* GDD base  (33) */
    Real min_temprange;         /*  (34) */
    Real emax;                  /* emax (mm/day)(35) */
    Real intc;                  /* Interception storage parameter (36) */
    Real alphaa;                /* alphaa, fraction of PAR assimilated at ecosystem level, relative to leaf level */
    Real lai_sapl;
    int phenology;              /* par17 */
    int path;                   /* par2 */
    Real sla;
    Real k1,k2,k3;
    Real soc_k;                 /* shape-factor for vertical distribution function of soil organic carbon, following Jobbagy et al. 2000*/
    Limit temp_co2;             /* temperature limit for CO2 uptake (24,27) */
    Limit temp_photos;          /* range of temperature optimum for
                                   photosynthesis(25,26) */
    Limit temp;                 /* bioclimatic limits (28,29) */
    Real aprec_min;             /* minimum annual precipitation (mm) */
    Real flam;
    Traitpar k_litter10;
    Real windspeed;             /* windspeed dampening */
    Real roughness;             /* roughness length */
    Real fuelbulkdensity;       /* fuel bulk density*/
    Tracegas emissionfactor;    /* trace gas emission factors */
    void *data;                 /* pointer for PFT specific extensions */

    /* list of pointers for PFT specific functions */
    /* (Virtual functions in C++)                  */

    void (*newpft)(struct Pft *);
    void (*init)(struct Pft *);
    Real (*wdf)(struct Pft *,Real,Real);
    Real (*npp)(struct Pft*,Real,Real,Real);
    Real (*fpar) (const struct Pft*);
    Real (*alphaa_manage) (const struct Pft*);
    void (*leaf_phenology)(struct Pft *,Real,int);
    Bool (*fwrite)(FILE *,const struct Pft *);
    Bool (*fread)(FILE *,struct Pft *,Bool);
    void (*fprint)(FILE *,const struct Pft *);
    void (*litter_update)(Litter *,struct Pft *,Real);
    Real (*establishment)(struct Pft *,Real,Real,int);
    Real (*fire)(struct Pft *,Real *);
    Real (*actual_lai)(const struct Pft *);
    void (*adjust)(Litter *,struct Pft *,Real);
    void (*reduce)(Litter *,struct Pft *,Real);
    void (*free)(struct Pft *);
    Real (*vegc_sum)(const struct Pft *);
    void (*mix_veg)(struct Pft *,Real);
    void (*fprintpar)(FILE *,const struct Pftpar *);
    void (*output_daily)(Daily_outputs *,const struct Pft *);
    void (*turnover_monthly)(Litter *,struct Pft *);
    Real (*livefuel_consumption)(Litter *,struct Pft *,const Fuel *,
                                 Livefuel *,Bool *,Real,Real);
    Bool (*annual)(struct stand *,struct Pft *,Real *);
    Real (*albedo)(const struct Pft *,Real,Real);
  } *par;                /* PFT parameters */
  Real fpc;              /* foliar projective cover (FPC) under full leaf
                            cover as fraction of modelled area */
  Real nind;             /* individual density (indiv/m2) */
  Real gdd;              /* current-year growing degree days */
  Real bm_inc;           /* annual biomass increment (gC/m2) */
  Real wscal;            /* mean daily water scalar (among leaf-on days) */
  Real wscal_mean;
  Real phen,aphen;
  Real flaimax;          /* fraction of maximum lai */
#ifdef DAILY_ESTABLISHMENT
  Bool established;
#endif
  int litter;            /* index of above-ground litter pool */
  struct stand *stand;   /* pointer to stand */
  void *data;            /* pointer for PFT specific extensions */
} Pft;

typedef struct Pftpar Pftpar;


/* Defines a pointer to a function that takes a pointer to FILE, Pftpar
 * and a char and returns a Bool.
 * This construction is necessary to make a function return a function
 * pointer.
 */

typedef Bool (*Fscanpftparfcn)(FILE *,Pftpar *,Bool);

/* Declaration of functions */

extern void newpft(Pft *,struct stand *,const Pftpar *);
extern void freepft(Pft *);
extern void freepftpar(Pftpar [],int);
extern int* fscanpftpar(FILE *,Pftpar **,const Fscanpftparfcn [],int,Bool);
extern Real temp_stress(const Pftpar *,Real,Real);
extern Real photosynthesis(Real *,Real *,int,Real,Real,Real,Real,Real ,Real);
extern Bool survive(const Pftpar *,const Climbuf *);
extern Real interception(Real *,const Pft *,Real,Real);
extern void initgdd(Real [],int);
extern void updategdd(Real [],const Pftpar [],int,Real);
extern Real gp(Pft *,Real,Real,Real,Real);
extern Real water_stressed(Pft *,Real [LASTLAYER],Real,Real,
                           Real,Real *,Real *,Real *,Real,Real,
                           Real,Real,Real,Real *,Bool);
extern Bool fwritepft(FILE *,const Pft *);
extern void fprintpft(FILE *,const Pft *);
extern Bool freadpft(FILE *,struct stand *,Pft *,const Pftpar[],int,Bool);
extern void noinit(Pft *);
extern Real nofire(Pft *,Real *);
extern Real nowdf(Pft *,Real,Real);
extern void noadjust(Litter *,Pft *,Real);
extern void nomix_veg(Pft *,Real);
extern Bool establish(Real,const Pftpar *,const Climbuf *);
extern Real noestablishment(Pft *,Real,Real,int);
extern Bool fscanlimit(FILE *,Limit *,Bool);
extern Bool fscanemissionfactor(FILE *,Tracegas *);
extern Real fire_sum(const Litter *,Real);
extern void fprintpftpar(FILE *,const Pftpar []);
extern void output_daily(Daily_outputs *,const Pft *,Real,Real);
extern void equilsoil(Soil *, int, const Pftpar []);
extern void noturnover_monthly(Litter *,Pft *);
extern char **createpftnames(int,int,int,int,int,const Pftpar []);
extern void freepftnames(char **,int,int,int,int,int);
extern int getncultivationtype(const Pftpar [],int,int);

/* needed for IMAGE, but can also be used otherwise */

extern Real timber_burn(const Pft *, Real,Litter *,Real);
extern Real timber_harvest(Pft *,Soil *,Pool *,Pool,Real,Real,Real *,Real *);

/* Definition of macros */

#define isphoto(tstress) (tstress>=1e-2)
#define getpftpar(pft,val) (pft)->par->val
#define newgdd(npft) newvec(Real,npft)

/*
 * The following macros allow to call the PFT specific functions like virtual
 * functions in C++
 */

#define fpar(pft) pft->par->fpar(pft)
#define turnover_monthly(litter,pft) pft->par->turnover_monthly(litter,pft)
#define alphaa(pft) pft->par->alphaa_manage(pft)
#define npp(pft,gtemp_air,gtemp_soil,assim) pft->par->npp(pft,gtemp_air,gtemp_soil,assim)
#define leaf_phenology(pft,temp,day) pft->par->leaf_phenology(pft,temp,day)
#define litter_update(litter,pft,frac) pft->par->litter_update(litter,pft,frac)
#define fire(pft,fireprob) pft->par->fire(pft,fireprob)
#define actual_lai(pft) pft->par->actual_lai(pft)
#define init(pft) pft->par->init(pft)
#define vegc_sum(pft) pft->par->vegc_sum(pft)
#define mix_veg(pft,scaler) pft->par->mix_veg(pft,scaler)
#define adjust(litter,pft,fpc) pft->par->adjust(litter,pft,fpc)
#define reduce(litter,pft,fpc) pft->par->reduce(litter,pft,fpc)
#define wdf(pft,demand,supply) pft->par->wdf(pft,demand,supply)
#define establishment(pft,fpc_total,fpc,n_est) pft->par->establishment(pft,fpc_total,fpc,n_est)
#define annualpft(stand,pft,fpc_inc) pft->par->annual(stand,pft,fpc_inc)

#endif
