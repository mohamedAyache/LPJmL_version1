/***************************************************************************/
/**                                                                       **/
/**                        t  r  e  e  .  h                               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     Header file for tree PFTs                                         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 15:07:59 +0200 (mer. 22 juil. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef TREE_H /* Already included? */
#define TREE_H

/* Definition of constants */

#define reinickerp 1.6
#define k_latosa 4e3      /* leaf area to sapwood area */
#define wooddens 2e5      /* wood density (gC/m3) */
#define FPC_TREE_MAX 0.95 /* maximum foliage projective cover for trees */

/* Declaration of datatypes */

typedef struct
{
  Real leaf,sapwood,root;
} Treephys;

typedef struct
{
  Real leaf,sapwood,heartwood,root,debt;
} Treephys2;

typedef struct
{
  int leaftype;			    /* par16*/
  Treephys turnover;		    /* turnover period (years) (9,11,12)*/
  Treephys cn_ratio;		    /* C:N mass ratio (13-15) */
  Real crownarea_max;		    /* tree maximum crown area (m2) (20)*/
  Treephys2 sapl;                   /* sapling */
  Real sapling_C;		    /*sapling C per (m2)*/
  Real allom1,allom2,allom3,allom4; /* allometry */
  Real aphen_min,aphen_max;
  Real height_max;                  /* maximum height of tree (m) */
  Real reprod_cost;                 /* reproduction cost */
  Real scorchheight_f_param;        /* scorch height (F)    benp */
  Real crownlength;                 /* crown length (CL)    benp */
  Real barkthick_par1;              /* bark thickness par1  benp */
  Real barkthick_par2;              /* bark thickness par2  benp */
  Real crown_mort_rck;              /* crown damage (rCK)   benp */
  Real crown_mort_p;                /* crown damage (p)     benp */
  Real fuelfrac[NFUELCLASS];        /* Fuel fractions */
  Real k_est;			/* maximum overall sapling establishment rate (indiv/m2) */
  int rotation;         /* coppicing interval from short ratation woody crops */
  int max_rotation_length; /* lifetime of short rotation woody crop plantation */
  Real harvest_ratio; /* fraction of biomass incremenet put into fruits */
  Bool with_grass;  /* grass PFT allowed under agriculture tree? */
} Pfttreepar;

typedef struct
{
  Real height;            /* tree height (m) */
  Real crownarea;         /* crown area (m2) */
  Real barkthickness;     /* bark thickness Kirsten */
  Real gddtw;
  Real aphen_raingreen;
  Bool isphen;       /*only used for summergreen trees*/
  Treephys turn;
  Treephys turn_litt;
  Treephys2 ind;
  Treephys2 uptake;  /*barrier.n: saving of carbon uptake */
  Treephys2 annual_uptake;//added to save the annual uptake in order to split it in daily uptake
  Treephys2 residue;  /*barrier.n: saving of carbon residues */
  Real c_fruit;
} Pfttree;

/* Declaration of functions */

extern void new_tree(Pft *);
extern Pft *newpftage(Pftpar *,int);
extern Real npp_tree(Pft *,Real,Real,Real);
extern Bool mortality_tree(Litter *,Pft *,Real,Real);
extern Real fpc_tree(Pft *);
extern Real fpar_tree(const Pft *);
extern Real alphaa_tree(const Pft *);
extern void litter_update_tree(Litter *, Pft *,Real);
extern void allometry_tree(Pft *pft);
extern Bool allocation_tree(Litter *,Pft *,Real *);
extern Real lai_tree(const Pft *);
extern Real actual_lai_tree(const Pft *);
extern Real turnover_tree(Litter *,Pft *);
extern void phenology_tree(Pft *,Real,int);
extern Bool fwrite_tree(FILE *,const Pft *);
extern void fprint_tree(FILE *,const Pft *);
extern Bool fread_tree(FILE *,Pft *,Bool);
extern Bool fscanpft_tree(FILE *,Pftpar *,Bool);
extern Bool isneg_tree(const Pft *);
extern Real establishment_tree(Pft *,Real,Real,int);
extern void init_tree(Pft *);
extern Real fire_tree(Pft *,Real *);
extern Real vegc_sum_tree(const Pft *);
extern void mix_veg_tree(Pft *,Real);
extern void free_tree(Pft *);
extern void light_tree(Litter *,Pft *,Real);
extern void adjust_tree(Litter *,Pft *,Real);
extern void reduce_tree(Litter *,Pft *,Real);
extern void fprintpar_tree(FILE *,const Pftpar *);
extern void output_daily_tree(Daily_outputs *,const Pft *);
extern Real livefuel_consum_tree(Litter *,Pft *,const Fuel *,Livefuel *,
                                 Bool *,Real,Real);
extern Bool annual_tree(struct stand *,Pft *,Real *);
extern Real coppice_tree(Pft *);
extern Real albedo_tree(const Pft *,Real,Real);
extern void turnover_monthly_tree(Litter *,Pft *);
extern Real harvest_tree(Pft *);

/* Definitions of macros */

#define istree(pft) (getpftpar(pft,type)==TREE)
#define israingreen(pft) getpftpar(pft,phenology)==RAINGREEN
#define fprinttreephys2(file,phys,nind) fprintf(file,"%6.2f %6.2f %6.2f %6.2f %6.2f (gC/m2)",phys.leaf*nind,phys.sapwood*nind,phys.heartwood*nind,phys.root*nind,phys.debt*nind)
#define phys_sum_tree(ind) (ind.leaf+ind.root+ind.heartwood+ind.sapwood)

#endif
