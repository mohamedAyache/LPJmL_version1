/***************************************************************************/
/**                                                                       **/
/**                   c  o  n  f  i  g  .  h                              **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-11-06 11:17:06 +0100 (ven. 06 nov. 20#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef CONFIG_H /* Already included? */
#define CONFIG_H

/* Definition of datatypes */

typedef struct
{
  Filename filename; /* Filename of output file */
  int id;
  Bool oneyear;
} Outputvar;

typedef struct
{
  Real lon_min;
  Real lon_max;
  Real lat_min;
  Real lat_max;
} Extension;

typedef struct
{
  const char *filename;       /* LPJ configuration filename */
  char *inputdir;             /* input directory */
  char *outputdir;            /* output directory */
  char *restartdir;           /* restart directory */
  Filename temp_filename;
  Filename prec_filename;
  Filename cloud_filename;
  Filename wet_filename;
  Filename wind_filename;
  Filename tamp_filename;
  Filename tmax_filename;
  Filename lightning_filename;
  Filename lwnet_filename;
  Filename swdown_filename;
  Filename popdens_filename;
  Filename human_ignition_filename;
  Filename co2_filename;
  Filename drainage_filename;
  Filename neighb_irrig_filename;
  Filename coord_filename;
  Filename soil_filename;
  Filename drainclass_filename;
  Filename river_filename;
  Filename countrycode_filename;
  Filename regioncode_filename;
  Filename landuse_filename;
  char *restart_filename;
  Filename lakes_filename;
  Filename wateruse_filename;
  Filename elevation_filename;
  Filename reservoir_filename;
  Filename sdate_filename;
#ifdef NEW_GRASS
  Filename grassfix_filename;
  Filename grassharvest_filename;
#endif
#ifdef IMAGE
  Filename temp_var_filename; /* file with CRU-derived temperature variation (K) */
  Filename prec_var_filename; /* file with CRU-derived precipitation variation (%) */
  Filename prodpool_init_filename; /* file with initial product pool sizes */
  char *image_host;       /* hostname for computer running the IMAGE model */
  int image_inport;       /* port numbert for ingoing data */
  int image_outport;      /* port number for outgoing data */
  int wait_image;         /* time to wait for image connection (sec) */
#endif
  char *sim_name;         /* Desciption of LPJ simulation */
  int sim_id;             /* Simulation type */
  int *npft;              /* number of PFTs in each PFT class */
  int nbiomass;           /* number of biomass PFTs */
  int nagtree;            /* number of agricultural tree PFts */
  int nsoil;              /* number of soil types */
  Soilpar *soilpar;       /* Soil parameter array */
  int ncountries;         /* number of countries */
  Countrypar *countrypar; /* country parameter array */
  int nregions;           /* number of regions */
  Regionpar *regionpar;
  Outputvar *outputvars;
  int n_out;     /* number of output files */
  Bool laimax_interpolate;
  int crop_index;
  Bool crop_irrigation;
  Bool landuse_restart;   /* land use enabled in restart file */
  int sdate_option_restart; /* sdate option in restart file */
  Bool param_out;               /* print LPJmL parameter */
  int compress;           /* compress NetCDF output (0: no compression) */
  float missing_value;    /* Missing value in NetCDF files */
  Variable *outnames;
  Outputmethod outputmethod;
  char *hostname;               /* hostname to send data */
  int port;                     /* port of socket connection */
#ifdef USE_MPI
  MPI_Comm comm; /* MPI communicator */
  int offset;
#endif
  char *write_restart_filename; /* filename of restart file */
  Pftpar *pftpar;         /* PFT parameter array */
  int restartyear; /* year restart file is written */
  int ntypes;    /* number of PFT classes */
  int ngridcell; /* number of grid cells */
  int startgrid; /* index of first local grid cell */
  int firstgrid; /* index of first grid cell */
  int nspinup;   /* number of spinup years */
  int nspinyear; /* cycle length during spinup (yr) */
  int lastyear;  /* last simulation year (AD) */
  int firstyear; /* first simulation year (AD) */
  int total;     /* total number of grid cells with valid soilcode */
  int nall;      /* total number of grid cells */
  int rank;      /* my rank */
  int ntask;     /* number of parallel tasks */
  int count;     /* number of grid cells with valid soilcode */
  int fire;      /* fire disturbance enabled */
  int firewood;  /* fire wood usage */
  int seed;      /* initial seed for random number generator */
  Coord resolution;    /* size of grid cell (deg) */
  Bool ispopulation;
  Bool river_routing;  /* river routing enabled */
  Bool permafrost;     /* permafrost module enabled */
  Bool from_restart;   /* reading from restart */
  int sdate_option;    /* sowing date option (computed internally: 0, fixed: 1, prescribed: 2)*/
  Bool isconstlai;     /* constant LAI max */
  Bool initsoiltemp;
  Pnet *route;         /* river routing network */
  Pnet *irrig_neighbour; /* irrigation neighbour network */
  Pnet *irrig_back;      /* back irrigation network */
  Pnet *irrig_res;
  Pnet *irrig_res_back;
  int withlanduse;
  Bool reservoir;
  int irrig_scenario; /* irrigation scenario (NO:0, LIM:1, POT:2, ALL:3, IRRIG on RAINFED: 4) */
  Bool rw_manage;     /* rain-water management enabled: reduced soil evaporation + rain-water harvesting */
  Bool pft_output_scaled; /* PFT output grid scaled */
  int with_radiation; /* input of radiation components (CLOUDINESS, RADIATION, RADIATION_SWONLY, RADIATION_LWDOWN) */
#ifdef IMAGE
  int start_imagecoupling; /* year in which coupling to IMAGE starts
                              (e.g. 1970), set to 9999 if IMAGE is not used */
  Socket *in;  /* socket for ingoing data */
  Socket *out; /* socket for outgoing data */
#endif

} Config; /* LPJ configuration */

/* Declaration of functions */

#ifdef USE_MPI
extern void initmpiconfig(Config *,MPI_Comm);
#endif
extern void initconfig(Config *);
extern FILE* openconfig(Config *,const char *,int *,char***,const char*);
extern void freeconfig(Config *);
extern void fprintconfig(FILE *,const Config *,int,int);
extern Bool filesexist(Config,int,int,Bool);
extern long long outputbuffersize(int,int,const Config *);
extern Variable *fscanoutputvar(FILE *,int,Bool);
extern void freeoutputvar(Variable *,int);
extern Bool fscanoutput(FILE *,Config *,int);
extern Bool readconfig(Config *,const char *,Fscanpftparfcn [],int,int,int *,
                       char ***,const char *);
extern Bool fscanconfig(Config *,FILE *,Fscanpftparfcn [],int,int);
extern void fprintparam(FILE *,int,int,const Config *);
extern Bool readfilename(FILE *,Filename *,const char *,Bool,Bool);
extern void fprintfiles(FILE *,Bool,const Config *);

/* Definition of macros */

#define printconfig(config,npft,ncft) fprintconfig(stdout,config,npft,ncft)
#define closeconfig(fp) pclose(fp)
#define iswriterestart(config) ((config)->write_restart_filename!=NULL)
#define isreadrestart(config) ((config)->restart_filename!=NULL)
#ifdef USE_MPI
#define isroot(config) ((config).rank==0)
#else
#define isroot(config) TRUE
#endif
#define printparam(config,npft,ncft) fprintparam(stdout,config,npft,ncft)
#define printfiles(isinput,config) fprintfiles(stdout,isinput,config)

#endif /* CONFIG_H */
