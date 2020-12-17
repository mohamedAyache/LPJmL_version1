/***************************************************************************/
/**                                                                       **/
/**                  p  a  r  a  m  .  h                                  **/
/**                                                                       **/
/**     Header file for LPJmL parameters                                  **/
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

#ifndef PARAM_H
#define PARAM_H

/* Definition of datatypes */

typedef struct
{
    Real k_litter10;
    Pool k_soil10;
    Real maxsnowpack; /* max. snow pack (mm) */
    Real soildepth_evap; /* depth of sublayer at top of upper soil layer (mm) */
    Real co2_p;       /* pre-industrial CO2 concentration (ppm) */
    Real k;
    Real theta;      /* co-limitation (shape) parameter */
    Real k_beer;     /* Extinction coefficient */
    Real alphac3;    /* intrinsic quantum efficiency of CO2 uptake in C4 plants */
    Real alphac4;    /* intrinsic quantum efficiency of CO2 uptake in C3 plants */
    Real bc3;        /* leaf respiration as fraction of Vmax for C3 plants */
    Real bc4;        /* leaf respiration as fraction of vmax for C4 plants */
    Real r_growth;     /* growth respiration */
    Real GM;           /* empirical parameter in demand function */
    Real ALPHAM;       /* Priestley-Taylor coefficient (Demand,maximum-value) */
    Real ko25;         /* Michaelis constant for O2 (Pa) at 25 deg C */
    Real kc25;          /* Michaelis constant for CO2 (Pa) at 25 deg C */
    Real atmfrac;      /* fraction of decomposed litter emitted as CO2 to the atmosphere */
    Real fastfrac;     /* fraction of soil-bound decomposed litter entering the intermediate soil carbon pool */
    Real k_mort_max;   /* asymptotic maximum mortality rate (1/year)*/
    Real k_mort;       /* coefficient of growth efficiency in mortality equation (k_mort2) */
    /* IRRIGATION */
    Real aprec_lim;    /* annual precipitation limit for C3 irrigation threshold */
    Real irrig_threshold_c3_dry; /* soil moisture irrigation threshold for C3 crops, annual precip < aprec_lim */
    Real irrig_threshold_c3_humid; /* soil moisture irrigation threshold for C3 crops, annual precip >= aprec_lim */
    Real irrig_threshold_c4; /* soil moisture irrigation threshold for C4 crops*/
    Real irrig_threshold_rice; /* soil moisture irrigation threshold for RICE */
    Real irrigation_soilfrac; /* fraction of soil filled with water during irrigation event */
    Real ec_canal[3];     /* conveyance efficiency for open canal irrigation systems for soil types sand, loam, clay */
    Real ec_pipe;      /* conveyance efficiency for pressurized irrigation systems */
    Real sat_level[4];   /* saturation level for the three irrigation systems (surf,sprink,drip) */
    Real drip_evap;        /* reduction of drip soil evap */
    Real laimax;     /* maximum LAI */
    Bool intercrop;
    Bool remove_residuals;
    int sdate_fixyear;    /* year in which sowing dates shall be fixed */
    int landuse_year_const; /* year landuse is fixed for LANDUSE_CONST case */
    Real esoil_reduction; /* reduction of soil evaporation */
    Real rw_buffer_max;   /* size of rainwater harvesting tank */
    Real frac_ro_stored;  /* fraction of surface runoff stored in tank */
    Real rw_irrig_thres;  /* threshold to apply rw_irrigation */
    Real soil_infil;      /* soil infiltration improvement */
    Real yield_gap_bridge; /* yield-gap bridge: factor by which laimax value is closed (7 - country-value)*factor */
} Param;

/* Declaration of global variable */

extern Param param;

/* Declaration of functions */

extern Bool fscanparam(FILE *,const Config *);

#endif
