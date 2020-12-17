/***************************************************************************/
/**                                                                       **/
/**                        o  u  t  p  u  t  .  h                         **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     Header file for output datatype and functions                     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer. 22 juil. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef OUTPUT_H
#define OUTPUT_H

/* Definition of datatypes */

#define DEFAULT_PORT 2222 /* default port for socket connection */

typedef struct
{
  Real lai;
  Real phen;
  Real cleaf;
  Real croot;
  Real cso;
  Real cpool;
  Real wdf;
  Real growingday;
  Real pvd;
  Real phu;
  Real fphu;
  Real laimaxad;
  Real lainppdeficit;
  Real husum;
  Real vdsum;
  Real wscal;
  Real npp;
  Real gpp;
  Real rd;
  Real rroot;
  Real rso;
  Real rpool;
  Real gresp;
  Real trans;
  Real evap;
  Real prec;
  Real perc;
  Real irrig;
  Real w0;
  Real w1;
  Real wevap;
  Real hi;
  Real fhiopt;
  Real himind;
  Real froot;
  Real temp;
  Real sun;
  Real par;
  Real daylength;
  Real swe; /* snow water equivalent*/
  Real discharge; 
  Real runoff;
  Real rh;
  Real interc;
  Real assim;
  Real rot_mode;
  Real pet;
  int cft;
  Bool irrigation;
} Daily_outputs;

typedef struct
{
  Real mnpp;             /* Monthly NPP (gC/m2) */
  Real mgpp;             /* Monthly GPP (gC/m2) */
  Real mrh;              /* monthly heterotrophic respiration (gC/m2) */
  Real mfpar;            /* Monthly fpar */
  Real mtransp;          /* Monthly transpiration (mm) */
  Real mtransp_b;        /* Monthly transpired irrigation water (mm) */
  Real atransp;          /* Yearly transpiration (mm) */
  Real mrunoff;          /* Monthly runoff (mm) */
  Real mdischarge;       /* Monthly discharge (1.000.000 m3/day) */
  Real mwateramount;     /* Mean wateramount in month (1.000.000 m3) */
  Real mevap;            /* Monthly evaporation (mm) */
  Real mevap_b;          /* Monthly evaporation of irrigation water (mm) */
  Real aevap;            /* Yearly evaporation (mm) */
  Real minterc;          /* Monthly interception (mm) */
  Real minterc_b;        /* Monthly blue interception (mm) */
  Real ainterc;          /* Yearly interception (mm) */
  Real mpet;             /* Monthly PET (mm) */
  Real mswc[NSOILLAYER]; /* monthly soil water content*/
  Real mrootmoist;       /* monthly plant available water for evapotranspiration fractional*/
  Real firec;            /* annual fire carbon emissions (gC/m2)*/
  Real mfirec;           /* monthly fire carbon emissions (gC/m2)*/
  Real mnfire;           /* monthly number of fires */
  Real mfiredi;          /* monthly fire danger index */
  Real mfireemission;    /* monthly fire emissions */
  Real mburntarea;       /* monthly burnt area */
  Real mprec_image;      /* monthly precipitation received from IMAGE [mm/month]*/
  Real mtemp_image;      /* monthly temperature received from IMAGE [K] */
  Real msun_image;       /* monthly cloudiness received from IMAGE [% sunshine = 100-%cloud]*/
  Real mwet_image;       /* monthly wet days received from IMAGE [days/month]*/
  Real firef;            /* fire frequency */
  Real flux_estab;       /* establishment flux (gC/m2) */
  Real flux_harvest;     /* harvest flux (gC/m2) */
  Real flux_rharvest_burnt; /* crop residuals burnt outside of field (gC/m2)*/
  Real flux_rharvest_burnt_in_field; /*crop residuals burnt in field (gC/m2)*/
  Real mirrig;           /* Monthly irrigation (mm) */
  Real airrig;           /* Yearly irrigation (mm) */
  Real mwd_unsustainable; /* Monthly withdrawal from unsustainable source (mm) */
  Real awd_unsustainable; /* Yearly withdrawal from unsustainable source (mm) */
  Real munmet_demand;     /* monthly unavailable requested irrigation water (mm) */
  Real *cft_airrig;      /* Yearly irrigation per cft (mm) */
  int *sdate;            /* sowing date */
  int *hdate;            /* Harvest date */
  Real *pft_npp;         /* Pft specific NPP (gC/m2) */
  Harvest *pft_harvest;
  Real *fpc;             /* foliar projective cover (FPC) */
  Real *pft_gcgp;
  Real *gcgp_count;
  Real mevap_lake;       /* Monthly lake evaporation (mm) */
  Real aevap_lake;       /* Yearly lake evaporation (mm) */
  Real mevap_res;        /* Monthly reservoir evaporation (mm) */
  Real aevap_res;        /* Yearly reservoir evaporation (mm) */
  Real mprec_res;        /* Monthly reservoir precipitation (mm) */
  Real mres_storage;     /* Monthly mean reservoir storage (million m3) */
  Real mres_demand;      /* Monthly reservoir demand (million m3) */
  Real mtarget_release;  /* Monthly target release (1.000.000 m3/day) */
  Real mres_cap;         /* monthly output of reservoir capacity (in million m3) */
  Real mwd_local;        /*  monthly local withdrawal (mm) */
  Real mwd_neighb;       /* monthly neighbour withdrawal (mm) */
  Real mwd_res;          /* monthly reservoir withdrawal (mm) */
  Real mwd_return;       /* monthly withdrawals returned to river (mm) */
  Real aconv_loss_evap;  /* Yearly evaporative conveyance loss of irrigation water withdrawals (mm) */
  Real aconv_loss_drain; /* Yearly drainage conveyance loss (mm) */
  Real mconv_loss_evap;  /* Monthly evaporative conveyance loss (mm) */
  Real mconv_loss_drain; /* Monthly drainage conveyance loss (mm) */
  Real mstor_return;     /* Monthly water amount returned from irrig stor into river system*/
  Real mprec;            /* Monthly precipitation (mm)*/
  Real mrain;            /* Monthly precipitation fallen as rain (minus snowfall) (mm)*/
  Real msnowf;           /* Monthly precipitation fallen as snow */
  Real mmelt;            /* Monthly snowmelt (mm)*/
  Real msnowrunoff;      /* Monthly runoff from snow above snowpack maximum */
  Real mswe;             /* Monthly average snow water equivalent (mm)*/
  Real awateruse_hil;    /* yearly waterwithdrawal of household, industry and
                          * livestock */
  Real mreturn_flow_b;   /* monthly blue water return flow of runoff */
  Real input_lake;       /* yearly precipitation input to lakes (mm) */
  Real adischarge;       /* Annual discharge (1.000.000 m3/year) */
  Real surface_storage;  /* Water stored in the suface storages by the end of year (dm3) */
  Real soil_storage;     /* Water stored in the soil column by the end of year (dm3) */
  Real *cftfrac;         /* cft fraction */
  Real *cft_consump_water_g; /* CFT specific green water consumption (mm) */
  Real *cft_consump_water_b; /* CFT specific blue water consumption (mm) */
  Real *growing_period;    /* length of growing period in days */
  Real *cft_pet;           /* cft PET */
  Real *cft_transp;        /* cft specific transpiration (mm) */
  Real *cft_transp_b;      /* cft specific transpiration (mm) */
  Real *cft_evap;          /* cft specific soil evaporation (mm) */
  Real *cft_evap_b;        /* cft specific blue soil evaporation (mm) */
  Real *cft_interc;        /* cft specific interception (mm) */
  Real *cft_interc_b;      /* cft specific blue interception (mm) */
  Real *cft_return_flow_b; /* cft specific irrigation return flows from surface runoff, lateral runoff and percolation (mm) */
  Real *cft_nir;           /* cft specific net irrigation requirement (mm) */
  Real *cft_fpar;          /* cft specific fpar */
  Real *cft_temp;          /* cft specific temperature sum (day degC) */
  Real *cft_prec;          /* cft specific precipitation (mm) */  
  Real *cft_srad;          /* cft specific short-wave radiation (W/m2) */
  Real *cft_aboveground_biomass; /* above ground biomass for crops before harvest (for grass before last harvest of year)*/
  Real *cft_conv_loss_evap; /* cft specific evaporative conveyance losses (mm) */
  Real *cft_conv_loss_drain; /* cft specific drainage conveyance losses (mm) */
  int  *cft_irrig_events;  /* number of irrigation days within growing season */
  Real prod_turnover;      /* carbon emissions from product turnover [gC/m2/a] in IMAGE coupling */
  Real deforest_emissions; /* carbon emissions from deforested wood burnt [gC/m2/a] in IMAGE coupling */
  Real trad_biofuel;       /* carbon emissions from traditional biofuel burnt [gC/m2/a] in IMAGE coupling */
  Real flux_firewood;      /* carbon emissions from domestic wood use [gC/m2/a]*/
  Real fburn;              /* fraction of deforested wood burnt [0-1]*/
  Real ftimber;            /* fraction of deforested wood harvested as timber [0-1]*/
  Real timber_harvest;     /* carbon harvested as timber [gC/m2/a] */
  Real product_pool_fast;  /* carbon in the fast product pool */
  Real product_pool_slow;  /* carbon in the slow product pool */
  Real *cft_luc_image;     /* LUC data received by IMAGE [0-1], CFT specific */
  Real msoiltemp1;         /* monthly soil temperature in deg C first layer*/
  Real msoiltemp2;         /* monthly soil temperature in deg C*/
  Real msoiltemp3;         /* monthly soil temperature in deg C*/
  Real msoiltemp4;         /* monthly soil temperature in deg C*/
  Real msoiltemp5;         /* monthly soil temperature in deg C*/
  Real msoiltemp6;         /* monthly soil temperature in deg C*/
  Real mrunoff_surf;       /* monthly surface runoff in mm*/
  Real mrunoff_lat;        /* monthly lateral runoff in mm*/
  Real mseepage;           /* monthly seepage water in mm*/
  Real mgcons_rf;          /* monthly green water consumption on rainfed stands */
  Real mgcons_irr;         /* monthly green water consumption on irrigated stands */
  Real mbcons_irr;         /* monthly blue water consumption on irrigated stands */
  Real dcflux;             /* daily carbon flux from LPJ to atmosphere (gC/m2/day) */
  Real mirrig_rw;          /* monthly supplementary rain water irrigation in mm */
  Daily_outputs daily;     /* structure for daily outputs */
} Output;

typedef struct
{
  Real nep;      /* Total NEP (gC) */
  Real fire;     /* Total fire (gC) */
  Real estab;    /* Total extablishment flux (gC) */
  Real harvest;  /* Total harvested carbon (gC) */
  Real transp;   /* Total transpiration (dm3) */
  Real evap;     /* Total evaporation (dm3) */
  Real interc;   /* Total interception (dm3) */
  Real wd;       /* Total irrigation water withdrawal (dm3) */
  Real wd_unsustainable;      /* Total irrigation water withdrawal from unsustainable source (dm3) */
  Real wateruse;              /* Total wateruse for household industry and livestock */
  Real discharge;             /* Total discharge (dm3) */
  Real evap_lake;             /* Total evaporation from lakes (dm3) */
  Real evap_res;              /* Total evaporation from reservoirs (dm3) */
  Real irrig;                 /* Total field irrigation (dm3) */
  Real conv_loss_evap;        /* Total evaporation during conveyance (dm3) */
  Real prec;                  /* Total precipitation (dm3) */
  Real delta_surface_storage; /* Total change of surface storage (dm3), increase positive */
  Real delta_soil_storage;    /* Total change of soil storage (dm3), increase positive */
  Real area;                  /* Total area (m2) */
  Real total_reservoir_out;   /* Total water extracted from reservoirs for irrigation */
  Real total_irrig_from_reservoir; /*Total water added to fields from reservoirs */
} Flux;

typedef enum {LPJ_FILES,LPJ_MPI2,LPJ_GATHER,LPJ_SOCKET} Outputmethod;

typedef struct
{
  float *mnpp;           /* monthly NPP (g C/m2) */
  float *mgpp;           /* monthly GPP (g C/m2) */
  float *mrh;            /* monthly heterotrophic respiration (gC/m2) */
  float *mfpar;          /* monthly fpar */
  float *mrunoff;        /* monthly runoff (mm) */
  float *mdischarge;     /* monthly discharge */
  float *mwateramount;   /* mean monthly wateramount */
  float *mtransp;        /* monthly transpiration (mm) */
  float *mtransp_b;      /* monthly transpired irrigation water (mm) */
  float *mevap;          /* monthly evaporation (mm) */
  float *mevap_b;        /* monthly evaporation of irrigation water (mm) */
  float *minterc;        /* monthly interception (mm) */
  float *minterc_b;      /* monthly blue interception (mm) */
  float *mpet;           /* monthly PET (mm) */
  float *mswc1;          /* monthly fractional saturation of soil water content of the upper soil layer */
  float *mswc2;          /* monthly fractional saturation of soil water content of the second soil layer */
  float *mswc3;          /* monthly fractional saturation of soil water content of the third soil layer  */
  float *mswc4;          /* monthly fractional saturation of soil water content of the fourth soil layer */
  float *mswc5;          /* monthly fractional saturation of soil water content of the fifth soil layer */
  float  *mrootmoist;    /* monthly plant available water for evapotranspiration fractional*/
  float *mirrig;         /* monthly irrigation (mm) */
  float *mreturn_flow_b; /* monthly blue water return flow of runoff (mm) */
  float *mevap_lake;     /* monthly lake evaporation (mm) */
  float *mevap_res;      /* monthly reservoir evaporation (mm) */
  float *mprec_res;      /* monthly reservoir precipitation (mm) */
  float *mres_storage;   /* Monthly mean reservoir storage (million m3) */
  float *mres_demand;    /* Monthly reservoir demand (million m3) */
  float *mtarget_release;  /* Monthly target release (1.000.000 m3/day) */
  float *mres_cap;       /* monthly output of reservoir capacity (in million m3) */
  float *mwd_local;      /* monthly local withdrawal (mm) */
  float *mwd_neighb;     /* monthly neighbour withdrawal (mm) */
  float *mwd_res;        /* monthly reservoir withdrawal (mm) */
  float *mwd_return;     /* monthly withdrawals returned to river (mm) */
  float *mconv_loss_evap;  /* Monthly evaporative conveyance loss (mm) */
  float *mconv_loss_drain; /* Monthly drainage conveyance loss (mm) */
  float *mstor_return;     /* Monthly water amount returned from irrig stor into river system*/
  float *mprec;          /* Monthly precipitation (mm)*/
  float *mrain;          /* Monthly precipitation fallen as rain (minus snowfall) (mm)*/
  float *msnowf;         /* Monthly precipitation fallen as snow (mm)*/
  float *mmelt;          /* Monthly snowmelt (mm)*/
  float *msnowrunoff;    /* Monthly runoff from snow above set maximum snowpack */
  float *mswe;           /* Monthly average snow water equivalent (mm)*/
  float *mfirec;         /* monthly fire carbon emissions (gC/m2)*/
  float *mnfire;         /* monthly number of fires */
  float *mfiredi;        /* monthly fire danger index */
  float *mfireemission;  /* monthly fire emissions */
  float *mprec_image;    /* monthly precipitation received from IMAGE [mm/month]*/
  float *mtemp_image;    /* monthly temperature received from IMAGE [K] */
  float *msun_image;     /* monthly cloudiness received from IMAGE [% sunshine = 100-%cloud]*/
  float *mwet_image;     /* monthly wet days received from IMAGE [days/month]*/
  float *msoiltemp1;     /* monthly soil temperature in deg C*/
  float *msoiltemp2;     /* monthly soil temperature in deg C*/
  float *msoiltemp3;     /* monthly soil temperature in deg C*/
  float *msoiltemp4;     /* monthly soil temperature in deg C*/
  float *msoiltemp5;     /* monthly soil temperature in deg C*/
  float *msoiltemp6;     /* monthly soil temperature in deg C*/
  float *mrunoff_surf;   /* monthly surface runoff in mm*/
  float *mrunoff_lat;    /* monthly lateral runoff in mm*/
  float *mseepage;       /* monthly seepage water in mm*/
  float *mburntarea;     /* monthly burnt area */
  float *mgcons_rf;      /* monthly green water consumption on rainfed stands */
  float *mgcons_irr;     /* monthly green water consumption on irrigated stands */
  float *mbcons_irr;     /* monthly blue water consumption on irrigated stands */
  float *mirrig_rw;          /* monthly supplementary rain water irrigation in mm */
  float *munmet_demand;      /* monthly unavailable requested irrigation water in mm*/
  int count;
} Outputmonth; /* Output for monthly data */

typedef struct
{
  float **data;
  int count;
} Outputday;

typedef struct
{
  char *name;  /* variable name */
  char *var;   /* data name in NetCDF file */
  char *descr; /* description */
  char *unit;  /* units */
  float scale;
} Variable;

/* Declaration of variables */

/* Declaration of functions */

extern void initoutput(Output *,int,Bool,int,int,int,int);
extern void initoutput_annual(Output *,int,int,int,int);
extern void initoutput_monthly(Output *);
extern void initoutput_daily(Daily_outputs *);
extern void freeoutput(Output *);
extern void freeoutputmonthly(Outputmonth *);
extern void freeoutputdaily(Outputday *);
extern void update_outputmonthly(Outputmonth *,const Output *,int,int);
extern void update_outputdaily(Outputday *,const Real [],int,int);
extern int outputsize(int,int,int,int,int);
extern Type getoutputtype(int);

#ifdef USE_MPI
extern int mpi_write(FILE *,void *,MPI_Datatype,int,int *,
                     int *,int,MPI_Comm);
extern int mpi_write_txt(FILE *,void *,MPI_Datatype,int,int *,
                         int *,int,MPI_Comm);
#endif
extern Bool ismonthlyoutput(int);

/* Definition of macros */

#define isopen(output,index) output->files[index].isopen
#define output_flux(output,flux) writedouble_socket(output->socket,(Real *)&flux,sizeof(Flux)/sizeof(Real))
#define isdailyoutput(index) (index>=D_LAI && index<=D_PET)

#endif
