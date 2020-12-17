/***************************************************************************/
/**                                                                       **/
/**                 l  a  n  d  u  s  e  .  h                             **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2016-02-11 11:10:20 +0100 (jeu. 11 févr. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef LANDUSE_H /* Already included? */
#define LANDUSE_H

/* Definitions of datatypes */

typedef enum {NATURAL,SETASIDE_RF,SETASIDE_IR,AGRICULTURE,MANAGEDFOREST,
              GRASSLAND,BIOMASS_TREE,BIOMASS_GRASS,AGRICULTURE_TREE,AGRICULTURE_GRASS,KILL} Landusetype;

typedef struct landuse *Landuse;

typedef struct
{
  Real *crop;          /* array of crop fractions */
  Real grass[NGRASS];  /* pasture and other fraction */
  Real biomass_grass;  /* biomass grass fraction */
  Real biomass_tree;   /* biomass tree fraction */
  Real *ag_tree;       /* array of agriculture tree fractions */
} Landfrac;

typedef struct
{
  int *crop;
  int *ag_tree;
  int grass[NGRASS];
  int biomass_grass;
  int biomass_tree;
} Irrig_system;

typedef enum {NO_SEASONALITY, PREC, PRECTEMP, TEMP, TEMPPREC} Seasonality;
#ifdef NEW_GRASS
typedef enum {GS_DEFAULT, GS_MOWING, GS_GRAZING_EXT, GS_GRAZING_INT, GS_NONE} GrassScenarioType;
typedef enum {RM_UNDEFINED, RM_GRAZING, RM_RECOVERY} RotationModeType;

typedef struct
{
  int grazing_days;
  int recovery_days;
  int paddocks;
  RotationModeType rotation_mode;
} Rotation;
#endif

typedef struct
{
  Landfrac *landfrac;     /* land use fractions */
  Irrig_system *irrig_system; /* irrigation system type (SURF=1,SPRINK=2,DRIP=3) */
  Manage manage;
  Cropdates *cropdates;
  Real cropfrac_rf;       /* rain-fed crop fraction (0..1) */
  Real cropfrac_ir;       /* irrigated crop fraction (0..1) */
  int *sowing_month;      /* sowing month (index of month, 1..12), rainfed, irrigated*/
  int *gs;                /* length of growing season (number of consecutive months, 0..11)*/
  Seasonality seasonality_type;  /* seasonality type (0..4) 0:no seasonality, 1 and
                                    2:precipitation seasonality, 3 and 4:temperature
                                    seasonality*/
  int *sdate_fixed;       /* array to store fixed or prescribed sowing dates */
  Resdata *resdata;       /* Reservoir data */
  Real *fraction;
  Real reservoirfrac;     /* reservoir fraction (0..1) */
  Real mdemand;           /* monthly irrigation demand */
  Bool dam;               /* dam inside cell (TRUE/FALSE) */
#ifdef NEW_GRASS
  int fixed_grass_pft;              /* fix C3 or C4 for GRASS pft */
  GrassScenarioType grass_scenario; /* 0=default, 1=mowing, 2=ext.grazing, 3=int.grazing */
  Real nr_of_lsus_ext;              /* nr of livestock units for extensive grazing */
  Real nr_of_lsus_int;              /* nr of livestock units for intensive grazing */
  Rotation rotation;                /* rotation mode and parameters for intensive grazing */
#endif
#ifdef IMAGE
  Image_data *image_data; /* pointer to IMAGE data structure */
#endif
} Managed_land;

/* Definitions of macros */

#define mixpool(pool1,pool2,frac1,frac2) pool1=(pool1*frac1+pool2*frac2)/(frac1+frac2)
/*#define getcelllanduse(landuse,cell) landuse->landfrac+(cell)*landuse->nbands*/
#define rothers(ncft) ncft
#define rmgrass(ncft) (ncft+1)
#define rbgrass(ncft) (ncft+2)
#define rbtree(ncft) (ncft+3)
#define agtree(ncft) (ncft+4)

/* Declaration of functions */

extern Landuse initlanduse(int,const Config *);
extern void freelanduse(Landuse,Bool);
extern Bool getintercrop(const Landuse);
extern void newlandfrac(Landfrac [2],int,int);
extern void initlandfrac(Landfrac [2],int,int);
extern void scalelandfrac(Landfrac [2],int,int,Real);
extern void freelandfrac(Landfrac [2]);
extern Bool fwritelandfrac(FILE *,const Landfrac [2],int,int);
extern Bool freadlandfrac(FILE *,Landfrac [2],int,int,Bool);
extern Real landfrac_sum(const Landfrac [2],int,int,Bool);
extern Real crop_sum_frac(Landfrac *,int,int,Real,Bool);
extern Real cultivate(Cell *,const Pftpar *,int,Real,Bool,int,Bool,Stand *,
                      Bool,int,int,int);
extern void reclaim_land(const Stand *, Stand *,Cell *,Bool,int);
extern Bool getlanduse(Landuse,Cell *,int,int,const Config *);
extern void landusechange(Cell *,const Pftpar[],int,int,int,int,Bool,Bool,int,Bool);
extern Bool setaside(Cell *,Stand *,const Pftpar[],Bool,int,Bool);
extern Bool newlanduse(Cell *,int,const Pftpar *,int,int,int,const Landuse,Bool);
extern Real sowing_season(Cell *,int,int,int,Real,const Config *);
extern Real sowing_prescribe(Cell *,int,int,int,const Config *);
extern void deforest(Cell *,Real,const Pftpar [],Bool,int,Bool,Bool,Bool,int,Real);
extern Real woodconsum(Stand*,Real);
extern void irrigation_stand(Stand *,Real,Real*,Real);
extern Real rw_irrigation(Stand *,Real,Real*,Real);
extern void irrig_amount_river(Cell *,const Config *);
extern void irrig_amount(Stand *,Bool,int,int,int);
extern void calc_wateruse(Cell *);
extern void mixsetaside(Stand *,Stand *,Bool);
extern void set_irrigsystem(Stand *,int,int,int,int,Bool);
extern Irrig_system *new_irrigsystem(int,int);
extern void free_irrigsystem(Irrig_system *);

#endif
