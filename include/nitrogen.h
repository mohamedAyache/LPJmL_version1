/*! Header file associated with the nitrogen module
 * \author Nicolas Barrier
 */

#ifndef NITROGEN_H /* Already included? */
#define NITROGEN_H

#define FRAC_NO3_FERTN 0.5  // fraction of fertilizers that go to NO3 (remaining go to H4

/* variables for adsorption */
#define A 98.61E-3  // fixed parameter corresponding to the amount of P adsorbed for a concentration of 1 g/m3 (gP/kg soil)
#define B 50.76E-3  // fixed parameter corresponding to the buffer capacity for a concentration of 1 g/m3 (m3/kg soil)
#define CSUSP_FOR 50 // concentration of suspended solids in the water from forest (g/m3).
#define CSUSP_GRASS 70 // concentration of suspended solids in the water from forest (g/m3).
#define CSUSP_CROP 350 // concentration of suspended solids in the water from forest (g/m3).
#define CSUSP_URB 500 // concentration of suspended solids in the water from forest (g/m3).
#define VMAX_P_SUSP 0.0055  // maximal adsorption rate of P to suspended solids (g/g)
#define KWATER_ADS 0.7   // half saturation constant for P adsorption (g/m3)

/* variables defined for soil denitrification
 * (nitrogen branch) */
#define NEGATIVE_SOIL_NO3_ERR 34
#define NEGATIVE_SOIL_NH4_ERR 35
#define NEGATIVE_SOIL_PO4_ERR 36


#define RATIO_CN_ROOT 85.  // C:N ratio of roots part
#define RATIO_CN_SAP 300. // C:N ratio of sapwood part
#define RATIO_CN_LEAF 65. // C:N ratio of leaf part
#define RATIO_CN_STOR 65. // C:N ratio of storing organs part
#define RATIO_CP_ROOT 1600. // C:P ratio of roots part
#define RATIO_CP_SAP 5500. // C:P ratio of sapwood part
#define RATIO_CP_LEAF 1200. // C:P ratio of leaf part
#define RATIO_CP_STOR 1200. // C:P ratio of storing organs part

#define RATIO_CN_DEC 5. // C:N ratio of soil decomposers (gC/gN)
#define RATIO_CP_DEC 16. // C:P ratio of soil decomposers (gC/gP)
#define IMMO_N 0.1  // maximum fraction of N immobilized daily by soil decomposers
#define IMMO_P 0.1  // maximum fraction of P immobilized daily by soil decomposers
#define BETA_RES_SOIL 0.01  // maximum fraction of carbon residues consumed daily by soil decomposers
#define BETA_RES_WATER 0.06  // maximum fraction of residues consumed daily by water decomposers
#define MDEC 0.5  // daily mortality of decomposers
#define SED 0.024   // fraction of residues entering the sediments in rivers or reservoirs

#define VIT_WAT_DEN 9.6E-3 /* uptake velocity for denitrification in water (m/d) */
#define LOAD_WAT 86400.  /* hydraulic load (m/d) */

#define VMAX_WAT_NITR 0.72   // maximal nitrification rate in thewater (g/m3/d)
#define K_WAT_NITR 1.4    //  half-saturation constant for water nitrification (g/m3)

/* nitrogen parameters for phytoplankton */
#define M_PHYTO 0.3  // daily mortality of phytoplankton, including grazing (nu)
#define CN_RATIO_PHYTO 9//10.    // C:N ratio of phytoplankton (gC/gN)
#define CP_RATIO_PHYTO 120//250.   // C:P ratio of phytoplankton (gC/gP)
#define MU_PHYTO 2.9  // maximal daily growth of phytoplankton
#define KP_PHYTO 0.06  // half-saturation constant for phytoplankton growth on P (g/m3)
#define KN_PHYTO 0.07  // half-saturation constant for phytoplankton growth on N (g/m3)

/* nitrogen parameters for wastewater */
#define FRAC_SEW_IN 0.5   //  fraction of inorganic nutrient in wastewater.
#define FRAC_SEW_DIN_NO3 0.5  //  fraction of nitrate in the wastewater inorganic N.

// stuff that is taken from the nitrogen branch
#define CDN 1.2  // ?
#define NPERCO 0.4
#define f_temp(soiltemp) exp(-(soiltemp-18.79)*(soiltemp-18.79)/(2*5.26*5.26)) /* Parton et al 2001*/
#define DENIT_RATE  6
#define A_DENIT  -7.35
#define B_DENIT  0.4804
#define ANION_EXCL  0.4
#define NITVOL_TEMP_FACTOR 0.41
#define VOL_CATION_EXCHANGE  0.15
#define DENIT_WATER_THRESHOLD   2
#define A_NIT  0.60
#define B_NIT  1.27
#define C_NIT  0.0012
#define D_NIT  2.84
#define C_CORR  10
#define BD   1.25
#define CN_RATIO  15
#define Z_NIT (D_NIT*(B_NIT-A_NIT)/(A_NIT-C_NIT))
#define N_NIT (A_NIT-B_NIT)
#define M_NIT (A_NIT-C_NIT)
#define K_MAX 0.10      /* k_max, maximum fraction of soil->NH4 assumed to be nitrified Parton, 2001*/
#define K_2 0.02      /* k_2, fraction of nitrified N lost as N20 flux Parton, 2001*/

#define Nmolarmass 14 //g/mol
#define Pmolarmass 31 //g/mol

/* structure that contains the arrays for
 * pulse crop, no pulse crop and grass fractions
 * (data per country per yer) */
typedef struct CountryFrac_
{
    int **countrypulsefrac;
    int **countrynopulsefrac;
    int **countrygrassfrac;
    int firstyear;
    int lastyear;
} CountryFrac;

/* structure that contains country parameters associated with
 * wastewater */
typedef struct CountryWasteData_ {
    Real *sewp; /* daily point sources of P from sewer systems (g) */
    Real *sewn; /* daily point sources of N from sewer systems (g) */
    Real *sewc; /* daily point sources of DOC from sewer systems (g) */
} CountryWasteData;

/* Structure that contains the manure data
 * (sedac N/P ratio, annual N manure for crop and pasture) */
typedef struct Manuredata_ {
    Real *man_sedac;
    Real *man_ann_crop_n;
    Real *man_ann_past_n;
} Manuredata;

/* Structure that contains the fertilizers data
 * (N and P fertilizers) */
typedef struct Fertdata_ {
    Real *fert_n;
    Real *fert_p;
} Fertdata;

/* Structure that contains the variables that will be exchanged among
 * the different cells through the river routing (Runoff is a bit misleading) */
typedef struct Runoff_
{
    Real wat;   // water exchange
    Real no3;
    Real po4;
    Real po4ads;
    Real nh4;
    Real doc;
    Real don;
    Real dop;
    Real phy;
    Real decc;
    Real res;
} Runoff;

/* Definitions of all the nitrogen related functions */
extern void nit_soil_adsorption(Stand *);
extern void nit_wat_adsorption(const Cell *, const Real, Real *, Real *,int);
extern void nit_ads_upd_po4(Real, const Real, Real *);
extern void nit_udpdate_frac_for_ads(const Config *, Cell *);

extern void nit_init_countfrac(const Config *);
extern void nit_update_country_cropfrac(const Config *, const Cell[], Real *, Real *);
extern void nit_reset_country_cropfrac(const Config *, const Cell[], Real *, Real *);

extern void nit_deforest_updatesoil(Stand *, Stand *, Real);

extern void nit_watdenit(const Cell *, Real *, const Real);
extern void nit_denitrification(Stand *);

extern Bool nit_update_fert_file(const Config *, Cell grid [], int);
extern Bool nit_init_fert(const Config *);
extern void nit_fert(const Config *, Cell *, int, Real, Real, int, int);

extern Bool nit_fscannitconfig(FILE *, Config *);

extern void nit_immo_soil(Soil *, int landusetype, char * name);

extern void nit_immo_water(const Cell *, const Real, Real *, Real *, Real *, Real *, Real *, const Real);

extern Real nit_infilNO3(int,const Stand *, Real*,Real*,Real,Real,Real,Real,const Soilpar*);
extern Real nit_infilNH4(int,const Stand *, Real*,Real*,Real,Real,Real,Real,const Soilpar*);
extern Real nit_infilPO4(int,const Stand *, Real*,Real);
extern void nit_init_application(Cell *, int , int );
extern void nit_init_soil_nutrients(Stand *, const Soilpar *);

extern void nit_init_uptake_daily(Stand *);
extern void nit_init_uptake_annual(Stand *);
extern void nit_init_residue_daily(Stand *);
extern void nit_init_residue_annual(Stand *);

extern Bool nit_init_manure_n(const Config *);
extern Bool nit_update_manure_file(const Config *, Cell [], int);
extern Bool nit_init_manure_sedac(const Config *, Cell []);
extern void nit_manure(const Config *, Cell *, int, Real, Real, Real, int,int);

extern void nit_mix_setaside(Stand *, const Stand *);

extern void nit_nitrification_water(const Cell *, const Real, Real *, Real *);
extern void nit_nitrification(Stand *);

extern Bool nit_openfiles(Climatefile *, const Filename *, const char *, size_t, const Config *);

extern void nit_update_phyto(const Cell *, Real *, Real *, Real *, Real *, const Real);

extern Bool nit_readfiles(Climatefile *, Real [], Real, Real, const Cell [], int, const Config *);

extern Bool nit_read_restarts_soil(FILE *, Soil *, int, Bool);
extern Bool nit_read_restarts_water (FILE *, Cell *, Bool);

extern void nit_remin_allpft(Stand *);
extern void nit_remin_crop(Pft *,Real * , Real *);
extern void nit_remin_grass(Pft *,Real * , Real *);
extern void nit_remin_tree(Pft *,Real * , Real *);

extern Real nit_tempdep(Real, Real, Real);
extern void nit_update_outputs(Cell *, int);
extern void nit_update_preccheck(Cell *, Real);

extern void nit_update_river(Cell *,const Config *config, int);
extern void nit_update_lakes(Cell *,int);

extern void nit_uptake_allpft(Stand *, int);
extern void nit_save_annual_uptake_allpft(Stand *);
extern void nit_uptake_crop(Pft *,int,Real * , Real * );
extern void nit_save_annual_uptake_crop(Pft *);
extern void nit_uptake_grass(Pft *,int ,Real * , Real * );
extern void nit_save_annual_uptake_grass(Pft *);
extern void nit_uptake_tree(Pft *,int,Real * , Real *  );
extern void nit_save_annual_uptake_tree(Pft *);

extern int nit_init_wastewater_country_par(const Config *);
extern void nit_update_waste_file(const Config *, const Cell [], int);
extern void nit_update_daily_wastewater(Cell [], int, const Config *);

extern Real nit_grassfrac(int, int);
extern Real nit_allfrac_nopulse(int, int);
extern Real nit_allfrac(int, int);
extern Real nit_cropfrac(int, int);

extern Bool nit_write_restarts_soil(FILE *, const Soil *, int);
extern Bool nit_write_restarts_water(FILE *, const Cell);

extern Climatefile *fwaste_n, *fwaste_p, *fwaste_c;
extern Climatefile *manure_n_crop, *manure_n_past;

extern Manuredata *manuredata;
extern Fertdata *fertdata;
extern CountryWasteData *countrywastedata;

extern void nit_init_phyto(Cell *);

extern void nit_printlog(const Config *);

// arrays associated (area of crop and grass for each country), used in fert. and manure.
extern CountryFrac *countryfrac;

#ifdef USE_MPI
/* When using MPI, we define a MPI_datatype for exchange among the
 * cells */
extern MPI_Datatype runoff_type;
#endif

// macro to recover the crop fraction of a coutry at a given year
// count is the country index, idyear is the year index
#define getfrac(countrycropfrac, count, idyear) *(*(countrycropfrac + count) + idyear)

#endif /* NITROGEN_H */
