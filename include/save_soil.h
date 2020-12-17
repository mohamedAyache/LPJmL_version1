/***************************************************************************/
/**                                                                       **/
/**                         s  o  i  l  .  h                              **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**     Header file for soil functions                                    **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 14.01.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#ifndef SOIL_H /* Already included? */
#define SOIL_H

/* Definition of constants */

#define NSOILLAYER 6 /* Number of soil layers */
#define LASTLAYER (NSOILLAYER-1)
#define FRACGLAYER NSOILLAYER
#define BOTTOMLAYER (NSOILLAYER-1)
#define NFUELCLASS 4 /* Number of fuel classes */
#define TOPLAYER 0

#define SNOWLAYER NSOILLAYER

#define veg_equil_year 990
#define soil_equil_year veg_equil_year+1320
/*#define atmfrac 0.7
#define soilfrac (1-atmfrac)
#define fastfrac 0.98*/
#define snow_skin_depth 40.0 /* snow skin layer depth (mm water equivalent)*/
#define c_water 4.2e6 /* J/m3/K */
#define c_ice   2.1e6 /* J/m3/K */
#define c_snow  2.1e6 /* snow heat capacity (J/ton/K) */
#define m_heat  4.0e4  /*J/gC microb. heating*/
#define lambda_snow 0.2
#define th_diff_snow (lambda_snow/snowheatcap) /* thermal diffusivity of snow [m2/s]*/
#define snow_density     0.3   /*ton/m3*/
#define snowheatcap     6.3e5  /*c_snow*snow_density [J/m3/K]*/
#define c_soilheatcapdry 1.2e6/*1.2e6   J/m3/K */
#define c_alblit         0.12 /* Albedo of litter (0-1) */
#define c_albsnow        0.70 /* Albedo of snow (0-1) */
#define c_albsoil        0.40 /* Albedo of bare soil (0-1). Should be soil and soil moisture dependant? */
#define c_snowcanopyfrac  0.40 /* Maximum snow cover fraction in the canopy (with leaves) */
#define c_watertosnow    6.70 /* Conversion factor from water to snowdepth, i.e. 1 cm water equals 6.7 cm of snow. Taken from RAKMO and personal communication with KNMI. */
#define c_mineral 1.9259e6 /* [J/m3/K] after deVries 1963*/
#define c_water2ice 0.3e9 /* the energy that is needed/released during water/ice conversion (J/m3)*/
#define tsnow 0.0
/* assuming lambda_air=0.025 and c_air = 1300; admit_air=sqrt(lambda_air*c_air)*/
#define admit_air 5.7009
#define T_zero 0   /* [deg C] */
#define maxheatsteps 100
#define NSTEP_DAILY 1
#define LAG_CONV (NDAYYEAR*0.5*M_1_PI)  /* conversion factor for oscillation
                                lag from angular units to days (=365/(2*PI))*/
#define MINERALDENS 2700 /* mineral density in kg/m3*/
#define ORGDENS 1400     /* density of organic soil substances [kg/m3]*/
#define PRIESTLEY_TAYLOR 1.32 /* Priestley-Taylor coefficient */
#define SOILDEPTH_IRRIG 500 /*size of layer considered for calculation of irrifgation ammount*/

/* Declaration of variables */

extern Real soildepth[NSOILLAYER];
extern Real layerbound[NSOILLAYER];
extern Real midlayer[NSOILLAYER];
extern Real logmidlayer[NSOILLAYER];
extern Real fbd_fac[NFUELCLASS];

#include "soilpar.h"

/* Definition of datatypes */

typedef enum {NOSTATE,BELOW_T_ZERO,AT_T_ZERO,ABOVE_T_ZERO,FREEZING,
              THAWING} Soilstate;

typedef struct
{
  Real fast;       /* fast-decomposing component */
  Real slow;       /* slow-decomposing component */
} Pool;

typedef struct
{
  Real leaf;       /* leaf litter (gC/m2)  */
  Real wood;       /* woody litter (gC/m2) */
} Traitpar;

typedef struct
{
  Real leaf;             /* leaf litter (gC/m2)  */
  Real wood[NFUELCLASS]; /* woody litter (gC/m2) */
} Trait;

typedef struct
{
  const struct Pftpar *pft; /* PFT id for litter */
  Trait trait;              /* leaf and wood litter */
} Litteritem;

typedef struct
{
  Real avg_fbd[NFUELCLASS+1];
  Litteritem *ag; /* above ground litter list for PFTs (gC/m2) */
  Real *bg;        /* below ground litter (gC/m2) */
  int n;          /* Number of above ground litter pools */
  Real decomC;  /*litter decomposed*/
} Litter;

typedef struct
{
  Real harvest,residual,residuals_burnt,residuals_burntinfield;
} Harvest;

typedef struct
{
  int type;   /* unique soil type id */
  char *name; /* soil name */
  Real Ks;    /*hydraulic conductivity (mm/h)*/
  Real Sf;    /*Suction head (mm)*/
  Real wpwp;  /* relative water content at wilting point */
  Real wfc;   /* relative water content at field capacity */
  Real wsat;  /* relative water content at saturation */
  Real beta_soil;
  Real whcs_all;
  Real whc[NSOILLAYER];   /* water holding capacity (fraction), whc = wfc - wpwp */
  Real wsats[NSOILLAYER]; /* absolute water content at saturation (mm), wsats = wsat * soildepth*/
  Real whcs[NSOILLAYER];  /* absolute water holding capacity (mm), whcs = whc * soildepth */
  Real wpwps[NSOILLAYER]; /*water at permanent wilting point in mm, depends on soildepth*/
  Real bulkdens[NSOILLAYER]; /* bulk density of soil [kg/m3]*/
  Real k_dry[NSOILLAYER]; /*thermal conductivity of dry soil */
  int hsg;        /* hydrological soil group for CN */
  Real tdiff_0;   /* thermal diffusivity (mm^2/s) at wilting point (0% whc) */
  Real tdiff_15;  /* thermal diffusivity (mm^2/s) at 15% whc */
  Real tdiff_100; /* thermal diffusivity (mm^2/s) at field capacity (100% whc) */
  Real tcond_pwp, tcond_100, tcond_100_ice; /* thermal conductivity [W/m/K]*/
  Real k1, k2;
} Soilpar;  /* soil parameters */

typedef struct
{
  const Soilpar *par; /* pointer to soil parameters */
  Pool cpool[LASTLAYER];         /* fast and slow carbon pool for all layers*/
  Pool k_mean[LASTLAYER];        /* fast and slow decay constant */
  Real *c_shift_fast[LASTLAYER];       /* shifting rate of carbon matter to the different layer*/
  Real *c_shift_slow[LASTLAYER];       /* shifting rate of carbon matter to the different layer*/
  Real w[NSOILLAYER],            /* fraction of whc*/
    w_fw[NSOILLAYER];            /* mm */
  Real w_evap;                   /* soil moisture content which is not transpired and can evaporate? correct? */
  Real perc_energy[NSOILLAYER]; /*energy transfer by percolation*/
  Real micro_heating[NSOILLAYER]; /*energy of microbiological heating*/
  Real alag,amp,meanw1;
  Real snowpack;
  Real temp[NSOILLAYER+1];      /*[deg C]; last layer=snow*/
  Real ice_depth[NSOILLAYER];   /* mm */
  Real ice_fw[NSOILLAYER];      /* mm */
  Real freeze_depth[NSOILLAYER]; /* mm */
  Real ice_pwp[NSOILLAYER];      /*fraction of water below pwp frozen */
  short state[NSOILLAYER];
  Real maxthaw_depth;
  Real mean_maxthaw;
  short drainage_class;     /* 1-7 (1 is well drained) */
  Real layer_exists[NSOILLAYER]; /* allows variable soil depth */
  Real decomp_litter_mean;
  int count;
  Real YEDOMA;          /* g/m2 */
  Litter litter;      /* Litter pool */
  Real decomC[NSOILLAYER];
  Real rw_buffer;             /* available rain water amount in buffer (mm) */
} Soil;

struct Pftpar; /* forward declaration */

/* Declaration of functions
 * NOTE: initsoiltemp() in cell.h */

extern int addlitter(Litter *,const struct Pftpar *);
extern void convert_water(Soil*,int,Real*);
extern void copysoil(Soil *,const Soil *, int);
extern int findlitter(const Litter *,const struct Pftpar *);
extern Real fire_prob(const Litter *,Real);
extern int fscansoilpar(FILE *,Soilpar **,Bool);
extern Bool freadlitter(FILE *,Litter *,const struct Pftpar *,int,Bool);

extern Bool freadsoil(FILE *,Soil *,const Soilpar *,const struct Pftpar *,int,Bool);

extern Bool freadsoilcode(FILE *,int *,Bool,int);
extern void freesoil(Soil *);
extern void freelitter(Litter *);
extern void freesoilpar(Soilpar [],int);
extern void fprintlitter(FILE *,const Litter *);
extern void fprintsoilpar(FILE *,const Soilpar [],int);
extern void fprintsoil(FILE *,const Soil *);
extern Bool freadsoilcode(FILE *,int *,Bool,int);
extern FILE *fopensoilcode(const Filename *,Bool *,int *,Bool);

extern Bool fwritesoil(FILE *,const Soil *,int);
extern Bool fwritelitter(FILE *,const Litter *);
extern void getlag(Soil *,int);
extern int getnsoilcode(const Filename *,int,Bool);
extern Soilstate getstate(Real *); /*temperature above/below/at T_zero?*/

extern void initsoil(Soil *soil,const Soilpar *, int);

extern Real litter_ag_sum(const Litter *);
extern Real litter_ag_grass(const Litter *);
extern Real litter_ag_sum_quick(const Litter *);
extern Real littersom(Soil *,Real [NSOILLAYER]);
extern Real littersum(const Litter *);
extern Real moistfactor(const Litter *);
extern void moisture2soilice(Soil *,Real *,int);
extern void newsoil(Soil *);
extern int seeksoilcode(FILE *,int,int,int);

extern Real snow(Soil *,Real *,Real *,Real, Real *,Real *);
extern Real snow_old(Real *,Real *,Real *,Real);
extern void soiltemp(Soil*, Real);
extern Real soiltemp_lag(const Soil *,const Climbuf *);
extern Real soilcarbon(const Soil *);
extern Real soilcarbon_slow(const Soil *);
extern Real soilwater(const Soil *);
extern Real soilconduct(const Soil *,int);
extern Real soilheatcap(const Soil *,int);
extern void soilice2moisture(Soil *, Real *,int);
extern Real temp_response(Real);
extern Real litter_ag_tree(const Litter *,int);


/* Definition of macros */

#define getsoilpar(soil,var) (soil)->par->var
#define foreachsoillayer(l) for(l=0;l<NSOILLAYER;l++)
#define forrootmoist(l) for(l=0;l<3;l++)  /* here defined for the first 1 m*/
#define forrootsoillayer(l) for(l=0;l<LASTLAYER;l++)
#define allice(soil,l) (soil->ice_depth[l]+soil->par->wpwps[l]*soil->ice_pwp[l]+soil->ice_fw[l])
#define allwater(soil,l) (soil->w[l]*soil->par->whcs[l]+soil->par->wpwps[l]*(1-soil->ice_pwp[l])+soil->w_fw[l])
#define timestep2sec(timestep,steps) (24.0*3600.0*((timestep)/(steps))) /* convert timestep --> sec */
#define fprintpool(file,pool) fprintf(file,"%.2f %.2f",pool.slow,pool.fast)

#endif /* SOIL_H */
