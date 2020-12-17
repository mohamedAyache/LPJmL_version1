/***************************************************************************/
/**                                                                       **/
/**                         c  r  o  p  .  h                              **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     Header file for crop PFTs                                         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 03.05.2007  by S.Jachner, M.Gumpenberger             **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer. 22 juil. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef CROP_H /* Already included? */
#define CROP_H

/* Definition of constants */

/*#define LAMBERTBEER_K 0.5*/
#define RESIDUES_FIRE FALSE   /* use parameters for agricultural fires */
#define RESIDUES_IN_SOIL 0.1  /* minimum residues in soil*/

#define MINLGP 4              /* minimum length of growing period, used in calc_seasonality */
#define DEFAULT_MONTH 1       /* default setting if no sowing month can be found in calc_seasonality*/
#define MIN_PREC 0.1          /* minimum daily precipitation (mm) at a wet day - definition from CRU*/

/* Declaration of datatypes */


typedef struct
{
  int sdatenh,sdatesh;
} Initdate;

typedef struct
{
  Real root,so,pool;
} Cropphys;

typedef struct
{
  Real root,so,pool,leaf;
} Cropphys2;

typedef struct
{
  int calcmethod_sdate;     /* different methods for determining the crop dates */
  Initdate initdate;        /* init sowing date for n/shemisphere */
  int hlimit;               /* maximum length of crop cycle */
  int fallow_days;          /* length of fallow period between crop cycles */
  Real temp_fall;           /* threshold for decreasing temperature to determine the crop date */
  Real temp_spring;         /* threshold for increasing temperature to determine the crop date */
  Real temp_vern;           /* threshold for increasing temperature to determine the crop date */
  Limit trg;                /* temperature under which vernalization is possible (deg C) */
  Real pvd;                 /* number of vernalization days required */
  Real psens;               /* sensitivity to the photoperiod effect [0-1] (1 means no sensitivity) */
  Real pb;                  /* basal photoperiod (h) (pb<ps for longer days plants) */
  Real ps;                  /* saturating photoperiod (h) (ps<pb for shorter days plants) */
  Limit phus;               /* minimum/maximum potential heat units required for plant maturity (deg C), winter types*/
  Limit phuw;               /* minimum/maximum potential heat units required for plant maturity (deg C), summer types*/
  Real phu_par;             /* parameter for determining the variability of phu */
  Limit basetemp;           /* minimum/maximum base temperature */
  Real fphuc;               /* fraction of growing season 1 [0-1] */
  Real flaimaxc;            /* fraction of plant maximal LAI 1 [0-1] */
  Real fphuk;               /* fraction of growing season 2 [0-1] */
  Real flaimaxk;            /* fraction of plant maximal LAI 2 [0-1] */
  Real fphusen;             /* fraction of growing period at which LAI starts decreasing [0-1] */
  Real flaimaxharvest;      /* fraction of plant maximal LAI still present at harvest [0-1]*/
  Real laimax;              /* plant maximal LAI (m2leaf/m2)*/
  Real laimin;              /* plant minimal LAI (m2leaf/m2)*/
  Real hiopt;               /* optimum harvest index HI reached at harvest*/
  Real himin;               /* minimum harvest index HI reached at harvest*/
  Real shapesenescencenorm; /* parameter for calculating the fraction of maximal LAI */
  Cropphys cn_ratio;        /* C:N mass ratio for root, storage organ, and pool */
} Pftcroppar;


typedef struct
{
  Bool wtype;               /* distinguish between winter and summer crop */
  int growingdays;          /* counter for the days of the crop cycle */
  Real pvd;                 /* vernalization days */
  Real phu;                 /* phenological heat unit */
  Real basetemp;            /* base temperature */
  Bool senescence;          /* current senescence period */
  Bool senescence0;         /* senescence period of yesterday */
  Real husum;               /* sum of heat units */
  Real vdsum;               /* sum of vernalization days */
  Real fphu;                /* fraction of phenological heat unit */
  Cropphys2 ind;
  Cropphys2 uptake;         /*barrier.n: saving of C uptake */
  Real lai;                 /* current leaf area index */
  Real lai000;              /* leaf area index of yesterday */
  Real laimax_adjusted;     /* adjusted maximum lai */
  Real lai_nppdeficit;      /* LAI reduction due to insufficient NPP */
  Real demandsum;
  Real supplysum;
} Pftcrop;

/* Declaration of functions */

extern void new_crop(Pft *);
extern void allocation_daily_crop(Pft *,Real, Real,Daily_outputs *);
extern Real npp_crop(Pft *,Real,Real,Real,Bool *,Real,Daily_outputs *);
extern Real fpc_crop(Pft *);
extern Real fpar_crop(const Pft *);
extern Real alphaa_crop(const Pft *);
extern void litter_update_crop(Litter *,Pft *,Real);
extern Real lai_crop(const Pft *);
extern Real actual_lai_crop(const Pft *);
extern Bool phenology_crop(Pft *,Real,Real);
extern void laimax_manage(Manage *,const Pftpar [],int,int,int);
extern Bool fwrite_crop(FILE *,const Pft *);
extern void fprint_crop(FILE *,const Pft *);
extern Bool fread_crop(FILE *,Pft *,Bool);
extern Bool fscanpft_crop(FILE *,Pftpar *,Bool);
extern Real establishment_crop(Pft *,Real,Real,int);
extern void init_crop(Pft *);
extern Real vegc_sum_crop(const Pft *);
extern void free_crop(Pft *);
extern void phen_variety(Pft *,int,Real,int,Bool);
extern void harvest_crop(Output *,Stand *,Pft *,int,int,int,Bool);
extern void adapt_crop_type(Real [],Real,const Pftpar [],int,int,int);
extern Real wdf_crop(Pft *,Real,Real);
extern void fprintpar_crop(FILE *,const Pftpar *);
extern void output_daily_crop(Daily_outputs *,const Pft *,Real,Real);
extern void calc_seasonality(Cell *,int,int,const Config *);

/* Definitions of macros */

#define iscrop(pft) (getpftpar(pft,type)==CROP)
#define phys_sum_crop(ind) (ind.leaf+ind.root+ind.so+ind.pool)
#define fprintcropphys2(file,phys,nind) fprintf(file,"%.2f %.2f %.2f %.2f (gC/m2)",phys.leaf*nind,phys.so*nind,phys.pool*nind,phys.root*nind)

#endif
