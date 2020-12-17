/***************************************************************************/
/**                                                                       **/
/**                   g  r  a  s  s  .  h                                 **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     Header file for grass PFTs                                        **/
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

#ifndef GRASS_H /* Already included? */
#define GRASS_H


/* Definition of datatypes */

typedef struct
{
  Real leaf,root;
} Grassphys;

typedef struct
{
  Grassphys turnover;	/* turnover period (years) (9,11,12) */
  Grassphys cn_ratio;	/* C:N mass ratio (13-15) */
  Grassphys sapl;       /* sapling */
  Real sapling_C;	/* sapling C (gC/m2) */
  Real reprod_cost;     /* reproduction cost */
} Pftgrasspar;

typedef struct
{
  Grassphys turn;
  Grassphys ind;
  Grassphys uptake;  /*barrier.n: saving of C uptake */
  Grassphys annual_uptake;//added to save the annual uptake in order to split it in daily uptake
  Grassphys residue;  /*barrier.n: saving of C residues */
#ifdef NEW_GRASS
  Real max_leaf;
#endif
} Pftgrass;

/* Declaration of functions */

extern void new_grass(Pft *);
extern Real npp_grass(Pft *,Real,Real,Real);
extern Real fpc_grass(Pft *);
extern Real fpar_grass(const Pft *);
extern Real alphaa_grass(const Pft *);
extern void litter_update_grass(Litter *, Pft*,Real);
#ifdef NEW_GRASS
extern void output_daily_grass(Daily_outputs *,const Pft *,Real, Real);
extern Harvest harvest_stand_grass(Output *,Stand *,int);
#else
extern void output_daily_grass(Daily_outputs *,const Pft *);
#endif
extern Bool allocation_grass(Litter *,Pft *,Real *);
extern Real lai_grass(const Pft *);
extern Real actual_lai_grass(const Pft *);
extern Real turnover_grass(Litter *,Pft *,Real);
extern void phenology_grass(Pft *,Real,int);
extern Bool fwrite_grass(FILE *,const Pft *);
extern void fprint_grass(FILE *,const Pft *);
extern Bool fread_grass(FILE *,Pft *,Bool);
extern Bool fscanpft_grass(FILE *,Pftpar *,Bool);
extern Real establishment_grass(Pft *,Real,Real,int);
extern Real vegc_sum_grass(const Pft *);
extern void mix_veg_grass(Pft *,Real);
extern Real fire_grass(Pft *,Real *);
extern void init_grass(Pft *);
extern void free_grass(Pft *);
extern void light_grass(Litter *,Pft *,Real);
extern void reduce_grass(Litter *,Pft *,Real);
extern void fprintpar_grass(FILE *,const Pftpar *);
extern Real livefuel_consum_grass(Litter *,Pft *,const Fuel *,Livefuel *,
                                  Bool *,Real, Real);
extern Bool annual_grass(struct stand *,Pft *,Real *);
extern Harvest harvest_stand(Output *,Stand *,int,Real);
extern void turnover_monthly_grass(Litter *,Pft *);
extern Real albedo_grass(const Pft *,Real,Real);

/* Definition of macros */

#define isgrass(pft) (getpftpar(pft,type)==GRASS)
#define fprintgrassphys(file,phys) fprintf(file,"%.2f %.2f (gC/m2)",phys.leaf,phys.root)
#define phys_sum_grass(ind) (ind.leaf+ind.root)

#endif
