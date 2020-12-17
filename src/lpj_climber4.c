/**
 * \file lpj_climber4.c
 *
 * $Id: lpj_climber4.c 3239 2016-02-11 10:10:20Z bloh $
 *
 */

/**
 * \file
 * C-side of the wrapper for coupling LPJ to Climber-4 / land_lad.
 * This wrapper replaces the LPJ main program when LPJ is
 * compiled as part of Climber-4.
 * It is not compiled for stand-alone LPJ.
 * See lpjml.c for information about the original main program.
 */

/**
 * \file
 * This block contains those functions that are called by the F90-implemented land_lad
 * resp. land_atlantes.
 * It is tested with our two main target platforms GNU gfortran with g++ and Intel Fortran
 * with Intel C++, both on Linux.
 * For other Fortran/C++ combos, different naming and argument passing conventions might
 * be needed.
 * GNU and Intel Fortran convert names of external functions to lowercase and append one
 * underscore. (This is their default behaviour, can be changed with command line options and/or
 * special directive comments in the code.)
 */

#include <time.h>
#include <math.h>
#include <assert.h>
#include "lpj.h"
#include "grass.h"
#include "tree.h"
#include "crop.h"
#include "natural.h"
#include "grassland.h"
#include "biomass_tree.h"
#include "biomass_grass.h"
#include "agriculture.h"
#include "agriculture_tree.h"
#include "agriculture_grass.h"

#ifndef DAILY_ESTABLISHMENT
#error for coupling with FMS/MOM4/Climber-4 , DAILY_ESTABLISHMENT must be #defined
#endif

#define NTYPES 3 /*< number of plant functional types: grass, tree, crop, bioenergy */
#define NSTANDTYPES 11 /*< number of stand types / land use types as defined in landuse.h */

#define dflt_conf_filename_ml "lpjml_fms.conf" /*< Default LPJ configuration file
                                              if called by lpjml */
#define dflt_conf_filename "lpj.conf" /*< Default LPJ configuration file
                                         if called by lpj */

static const char *progname;

static Outputfile *output; /*< Output file array */
static Input input;        /*< input data */
static Config config;      /*< LPJ configuration */
static Cell *grid;         /*< cell array */
static Standtype standtype[NSTANDTYPES];

static int npft;           /*< Number of natural PFT's */
static int ncft;           /*< Number of crop PFT's */

/// from iterate()
static Real co2,cflux_total;
static Flux flux;
static int year, landuse_year, wateruse_year;
#ifdef STORECLIMATE
static Climatedata store,data_save;
#endif

static int is, ie, js, je;    //< boundary indices of local compute domain
static MPI_Comm comm;         //< MPI communicator for the atmosphere domain
static int pe, root_pe, npes; //< our own MPI rank, root MPI rank, total number of atmosphere ranks


static int dt_fast; /*< fast time step [sec] */

/// Information for mapping between FMS compute domain lat-lon-arrays
/// and LPJ grid cell indices.
/// We assume that both FMS and LPJ have longitudes running 0..360 degrees (not radians, not -180..180)
static double lpj_lon_min = 100000, lpj_lon_max = -100000, lpj_lat_min = 100000, lpj_lat_max = -100000; //< cell center coordinates of bounding box
static int nlons, nlats; //< number of cells in lon/lat directions of the bounding box in the FMS domain
static int *lpj_to_fms_indices = NULL; //< array to map LPJ cell index to FMS array index, on local compute domain
static int *fms_to_lpj_indices = NULL; //< array to map FMS array index to LPJ cell index


// evaporation / transpiration is accumulated for monthly sums in the cell output structure.
// daily values are available only in local variables of some functions.
// Thus, we remember here yesterday's monthly sums, and the difference to today's values
// is the daily amount. The XXX_yesterday values must be reset to 0 every month.
static Real *mevap_yesterday = NULL, *mtransp_yesterday = NULL;
// mevap_lake and mevap_res are computed separately in update_daily.c ,
// thus we must treat those also here, and finally sum up all three evap components for passing to the coupler.
static Real *mevap_lake_yesterday = NULL, *mevap_res_yesterday = NULL;

// LPJ does not know, where runoff enters the ocean. It just stores a dead-end indicator
// in its river routing map (cell.discharge.next == -1).
// Here we store for each such cell the index of the target ocean cell,
// where its runoff enters the ocean.
static struct Runoff_map {
  int from_i, from_j; // just for debugging / visualisation
  int to; // destination index in regular grid on LPJ resolution
} *runoff_map = NULL;

// The DDM30 data set used as source for LPJ runoff does not have data for antarctica.
// Thus use runoff from land_lad for that region, if use_runoff_antarctica_from_lpj = .false.

static int use_runoff_antarctica_from_lpj = 0;

/// Compare double values for equality.
/// They are considered equal if the relative error is smaller than epsilon.
inline static Bool double_eq(const double v1, const double v2, const double e) {
  return fabs((v1-v2)/((v1+v2)/2)) < e;
}


/**
 * lpj_init_() first of all is used to check for
 * compatibility of the procedure call interface between
 * the Fortran90 and C++ modules.
 * Possible pitfalls include:
 * - size of Fortran integer vs. sizeof C++ int
 * - size of Fortran real vs. sizeof C++ float or double
 * - assumed size arrays and pointers might be passed as
 *   descriptors (alias dope vectors)
 * - alignment of elements in derived types
 *   => to avoid this problem, we try to avoid passing
 *      derived types, and use the single elements as call
 *      arguments (we shall see if this will always be possible)
 *
 * TODO: check data types for char * parameters from Fortran
 * However, non are used here yet.
 */
#ifdef __cplusplus
extern "C"
#endif
void lpj_init_
(
 /*! first some arguments for checking the F90->C++ interface */
 const int *const deadbeef, const double *const pitest,
 const int *const true_in, const int *const false_in,
 const double *const testarray3d,
 const int *const kind_i, const int *const kind_r, const int *const kind_l,

 /*! now the ``real'' arguments */
 /*! the FMS time stamps are broken out into days and seconds */
 const int *const Time_init_year, const int *const Time_init_month, const int *const Time_init_day,
 const int *const Time_init_hour, const int *const Time_init_minute, const int *const Time_init_second,
 const int *const Time_year, const int *const Time_month, const int *const Time_day,
 const int *const Time_hour, const int *const Time_minute, const int *const Time_second,
 const int *const Time_step_fast /* seconds */, const int *const Time_step_slow /* seconds */,

 /*! information about domain partitioning */
 const int *const peset_in,   /*< MPI communicator for the set of atmoshpere pes */
 const int *const pe_in,      /*< our own MPI rank == processing element number */
 const int *const root_pe_in, /*< MPI rank of atmosphere master process */
 const int *const npes_in,    /*< number of total atmosphere processes */
 /*! the boundary indices of the local compute domain */
 const int *const is_in, const int *const ie_in, const int *const js_in, const int *const je_in,

 /*! grid axis info */
 const int *const nlons_in, /*< number of cells in the FMS domain at land_lad resolution in lon/lat directions */
 const int *const nlats_in, /*< just used for testing and curiosity */
 const double *const minlonb_in, const double *const maxlonb_in,
 const double *const minlatb_in, const double *const maxlatb_in, /*< bounding box of LPJ compute domain */

 /*! Wether to skip antarctica in river runoff mapping
  * The DDM30 data set used as source for LPJ runoff does not have data for antarctica.
  * Thus use runoff from land_lad for that region, if use_runoff_antarctica_from_lpj = .false.
  */
 const int *const use_runoff_antarctica_from_lpj_in,

 /*! LPJ-specific options TODO */
 const int *const argc_in, /* char **argv,*/

 /*! Output parameters */
 int *nlons_out, int *nlats_out, /*< number of cells in lon and lat directions */
 double *reslon_out, double *reslat_out, /*< LPJ resolutions in lon and lat directions, degrees */

 /*! and finally some interface-checking args again */
   const double *const onetwothree, const int *const fse
 ) { /* begin of lpj_init_() */

  const double myonetwothree = 1234567809.0987654321L;

  int argc = *argc_in;
  char **argv = NULL;

  int i, j, k;

  printf("%s %s F90->C interface test\n", __FILE__, __FUNCTION__); fflush(stdout);
  printf("Fortran KIND(integer) %d KIND(real) %d KIND(logical) %d C++ sizeof int %d long %d float %d double %d Bool %d\n",
         *kind_i, *kind_r, *kind_l, (int)sizeof(int), (int)sizeof(long), (int)sizeof(float), (int)sizeof(double),
         (int)sizeof(Bool));
  fflush(stdout);
  if (*kind_i != sizeof(int)) { printf("Error: Fortran KIND(integer) != C++ sizeof(int)\n"); fflush(stdout); abort(); }
  if (*kind_r != sizeof(double)) { printf("Error: Fortran KIND(real) != C++ sizeof(double)\n"); fflush(stdout); abort(); }
  if (*kind_l != sizeof(int)) { printf("Error: Fortran KIND(logical) != C++ sizeof(int)\n"); fflush(stdout); abort(); }
  if (*deadbeef != 0xeadbeef /* 0xdeadbeef yields overflow error on 32bit machines */) {
    printf("Error: test1 failed: 0xdeadbeef != %x\n", *deadbeef);
    fflush(stdout);
    abort();
  }
  if (! double_eq(*pitest, M_PI, epsilon)) {
    printf("Error: test2 failed: %30.28f != %30.28f\n", M_PI, *pitest); fflush(stdout);
    abort();
  }
  if (! (*true_in)) { printf("Error: test3 failed: true_in %x is not true\n", *true_in);
    fflush(stdout);
    abort();
  }
  if (*false_in) { printf("Error: test4 failed: false_in %x is not false\n", *false_in);
    fflush(stdout);
    abort();
  }

  for (k = 1; k <= 4; k++) {
    for (j = 1; j <= 3; j++) {
      for (i = 1; i <= 2; i++) {
        const int idx = (k-1)*3*2 + (j-1)*2 + (i-1);
        const double testval = testarray3d[idx];
        const double myval = i*100.0+j+k/10.0;
        /*printf("testarray3d(%2d,%2d,%2d) = testarray3d[%2d] = %10.9g vs. %10.9g\n",
         *	 i, j, k, idx, testval, myval);
         */
        if (! double_eq(testval, myval, epsilon)) {
          printf("Error: test5 failed: testarray3d(%d,%d,%d) is %10.9g != %10.9g\n", i, j, k, testval, myval);
          fflush(stdout);
          abort();
        }
      }
    }
  }

  if (! double_eq(*onetwothree, myonetwothree, epsilon)) {
    printf("Error: test6 failed: %24.12f != %24.12f\n", myonetwothree, *onetwothree);
    fflush(stdout);
    abort();
  }
  if (*fse != 4711) { printf("Error: test7 failed: 4711 != %d\n", *fse); fflush(stdout); abort(); }
  /*printf("double HUGE_VAL is %g\n", HUGE_VAL);*/
  printf("... interface test finished\n"); fflush(stdout);

  /* Interface tests finished,
   * now look at the actual model parameters
   */
  is = *is_in; ie = *ie_in; js = *js_in; je = *je_in;
  pe = *pe_in; root_pe = *root_pe_in; npes = *npes_in;
  comm = *peset_in;
  if (pe == root_pe) {
    printf("MPI: comm %d=0x%x pe %d root_pe %d npes %d\n", comm, comm, pe, root_pe, npes);
    printf("Domain: local boundary indices: is %d ie %d js %d je %d\n", is, ie, js, je);

    printf("Time_init: %04d-%02d-%02d %02d:%02d:%02d\n",
           *Time_init_year, *Time_init_month, *Time_init_day,
           *Time_init_hour, *Time_init_minute, *Time_init_second);
    printf("Time now : %04d-%02d-%02d %02d:%02d:%02d\n",
           *Time_year, *Time_month, *Time_day,
           *Time_hour, *Time_minute, *Time_second);
    printf("Time step fast: %d seconds\n", *Time_step_fast);
    printf("Time step slow: %d seconds\n", *Time_step_slow);
  }
  dt_fast = *Time_step_fast;

  /* fake the argument vector */
  argv = (char **)malloc((argc+1) * sizeof(*argv));
  if (NULL == argv) { perror("cannot malloc argv for LPJ"); exit(1); }
  argv[argc] = NULL;
  argv[0] = strdup("lpjml"); /* or lpjml ? or something else ? */
  if (argc > 1) {
    /* TODO: fill in the other entries */
    for (i = 1; i < argc; i++) argv[i] = NULL;
  }

  if (*use_runoff_antarctica_from_lpj_in) use_runoff_antarctica_from_lpj = 1;

  /*! LPJ must begin all its runs at start of a fresh year ! */
  /*! TODO: check that here. */
  /* Also: LPJ reads its start spinup, start, and count-of-to-be-simulated years from
   * its configuration file, inside readconfig() below
   * (actually it happens in fscanconfig() section V.)
   * When coupling with  FMS, we want to use this information here that was passed from
   * FMS instead of the numbers from the lpj config file.
   * On the other hand, it might be useful to check if both dates match.
   */

  int rc;

#ifndef DAILY_ESTABLISHMENT
  fprintf(stderr, "Error: for coupling into Climber-4, LPJ must be compiled with flag -DDAILY_ESTABLISHMENT\n");
  fflush(stderr);
  abort();
#endif

  /* Create array of functions, uses the typedef of (*Fscanpftparfcn) in pft.h */
  Fscanpftparfcn scanfcn[NTYPES]={fscanpft_grass,fscanpft_tree,fscanpft_crop};

  standtype[NATURAL]=natural_stand;
  standtype[SETASIDE_RF]=setaside_rf_stand;
  standtype[SETASIDE_IR]=setaside_ir_stand;
  standtype[AGRICULTURE]=agriculture_stand;
  standtype[MANAGEDFOREST]=managedforest_stand;
  standtype[GRASSLAND]=grassland_stand,
  standtype[BIOMASS_TREE]=biomass_tree_stand,
  standtype[BIOMASS_GRASS]=biomass_grass_stand,
  standtype[AGRICULTURE_TREE]=agriculture_tree_stand,
  standtype[AGRICULTURE_GRASS]=agriculture_grass_stand,
  standtype[KILL]=kill_stand;

  /*
   * Use default communicator containing all processors. In defining your own
   * communicator it is possible to run LPJ on a subset of processors
   * HERE: use the communicator from FMS/MPP domain
   */
  initmpiconfig(&config,comm);

  /* TODO: use parameters passed from F90 part instead of argv */
  progname=strippath(argv[0]); /* strip path from program name */
  if(argc>1)
  {
    if(!strcmp(argv[1],"-h")) /* check for help option */
    {
      if(isroot(config))
      {
        help(progname,
             (strcmp(progname,"lpj")) ? dflt_conf_filename_ml : dflt_conf_filename);
      }
      MPI_Finalize();
      /*return EXIT_SUCCESS*/ exit(0);
    }
    else if(!strcmp(argv[1],"-v")) /* check for version option */
    {
      if(isroot(config))
        printflags(progname);
      MPI_Finalize();
      /*return EXIT_SUCCESS*/ exit(0);
    }
  }
  if(isroot(config))
  { /* Output only for the root task 0 */
    time_t tbegin;
    time(&tbegin);
    copyright(progname);
    printf("\nRunning for user %s on %s at %s",getuser(),gethost(),
           ctime(&tbegin));
    fflush(stdout);
  }

  /* NTYPES is set to natural vegetation must be considered
   * in light and establishment
   * crops must have last id-number */
  /* Read configuration file */
  rc=readconfig(&config,
                (strcmp(progname,"lpj")) ? dflt_conf_filename_ml :
                                           dflt_conf_filename,
                scanfcn,NTYPES,NOUT,&argc,&argv,usage);
  failonerror(&config,rc,READ_CONFIG_ERR,"Cannot read configuration");
  if(isroot(config) && argc)
    fprintf(stderr,"WARNING018: Arguments listed after configuration filename, will be ignored.\n");

  /*! check that the date passed from FMS matches the date from the LPJ configuration */
  /* alternate TODO: set the date from FMS to the LPJ configuration structure */
  if(isroot(config))
    printf("Current FMS date: %04d-%02d-%02d. Current LPJ year: %04d\n",
           *Time_year, *Time_month, *Time_day, config.firstyear);
  if (isroot(config) && 1 != *Time_month || 1 != *Time_day) {
    printf("Warning: FMS start time is not at begin of new year\n");
    /*fflush(stdout); abort();*/
  }
  if (isroot(config) && *Time_year != config.firstyear) {
    printf("Warning: FMS start year differs from LPJ start year\n");
    /*fflush(stdout); abort();*/
  }

  /* Check that river routing was enabled in the configuration
   * The original LPJ can also work (faster) wthout river routing,
   * but for coupling with FMS we always want it enabled.
   */
  if (!config.river_routing) {
    fprintf(stderr, "%s: for coupling with Climber-4, LPJ must be configured to use river routing\n",
            __FUNCTION__);
    fflush(stderr);
    abort();
  }

#ifdef IMAGE
  if(config.sim_id==LPJML_IMAGE)
  {
    rc=open_image(&config);
    failonerror(&config,rc,OPEN_IMAGE_ERR,"Cannot open IMAGE coupler");
  }
#endif
  if(isroot(config))
    printconfig(&config,config.npft[GRASS]+config.npft[TREE],
                config.npft[CROP]);
  /* Allocation and initialization of grid */
  /*! TODO: pass FMS domain bounding box info to the MPI partitioning code of LPJ
   *        so that it can distribute the LPJ cells to match the FMS domains
   */
  rc=((grid=newgrid(&config,standtype,NSTANDTYPES,config.npft[GRASS]+config.npft[TREE],config.npft[CROP]))==NULL);
  failonerror(&config,rc,INIT_GRID_ERR,"Initialization of LPJ grid failed");
  rc=initinput(&input,grid,config.npft[CROP],&config);
  failonerror(&config,rc,INIT_INPUT_ERR,
              "Initialization of input data failed");
  /* open output files */
  output=fopenoutput(grid,NOUT,&config);

  if(isopen(output,GRID))
    writecoords(output,GRID,grid,&config);
  if(isopen(output,COUNTRY) && config.withlanduse)
    writecountrycode(output,COUNTRY,grid,&config);
  if(isopen(output,REGION) && config.withlanduse)
    writeregioncode(output,REGION,grid,&config);
  if(isroot(config))
    printf("Simulation begins...\n");

  //year=iterate(output,grid,input,
  //             config.npft[GRASS]+config.npft[TREE],config.npft[CROP],
  //             &config);

  //int iterate(Outputfile *output, /* Output file data */
  //            Cell grid[],        /* cell grid array */
  //            Input input,        /* input data: climate, land use, water use */
  //            int npft,           /* Number of natural PFT's */
  //            int ncft,           /* Number of crop PFT's */
  //            Config *config   /* grid configuration data */
  //            )                    /* returns last year+1 on success */

  npft = config.npft[GRASS]+config.npft[TREE];
  ncft = config.npft[CROP];

  // from iterate()
  //Bool rc;
#ifdef STORECLIMATE
  if(config.nspinup)
  {
    //fprintf(stderr, "Error: The current LPJ configuration specifies %d spinup years, but when coupled to Climber-4, no Spinup is possible\n",
    //        config.nspinup);
    //fflush(stderr);
    //abort();

    /* climate for the first nspinyear years is stored in memory
       to avoid reading repeatedly from disk */
    rc = storeclimate(&store,input.climate,grid,input.climate->firstyear,config.nspinyear,&config);
    failonerror(&config,rc,STORE_CLIMATE_ERR,"Storage of climate failed");

    data_save=input.climate->data;
  }
#endif
  if(config.initsoiltemp)
    initsoiltemp(input.climate,grid,&config);


  /* main loop over spinup + simulation years  */
  // for(year=config.firstyear-config.nspinup;year<=config.lastyear;year++)

  /* --- end of the code that was copied from lpjml.c --- */

  /**
   * Create a mapping from Fortran90-lat-lon arrays to LPJ cell indices.
   * First compute LPJ grid cell boundaries. That is also needed to prepare
   * output parameters with information about the LPJ grid.
   * The mapping is needed for storing input data from land_lad/FMS into LPJ,
   * and to extract LPJ results vice versa.
   * The mapping functions are intended to work on LPJ's own grid resolution.
   * Interpolation from/to possibly different grid in land_lad is handled on the
   * Fortran side.
   *
   * Sigh. The official LPJ input data set uses longitudes -180 .. 180, while land_lad uses
   * 0 .. 360 .
   *
   * From /home/bloh/LPJmL/src/netcdf/createcoord_all.c
   * This code creates a bounding box around all land cells.
   * But when coupled to FMS, also ocean cells are contained
   * in the FMS domain. Thus we must embed the LPJ cells in the FMS domain.
   * <PRE>
   *   array->lon_min=array->lat_min=1000;
   *   lon_max=lat_max=-1000;
   *   for(cell=0;cell<config->nall;cell++)
   *   {
   *     if(array->lon_min>lon[cell])
   *       array->lon_min=lon[cell];
   *     if(lon_max<lon[cell])
   *       lon_max=lon[cell];
   *     if(array->lat_min>lat[cell])
   *       array->lat_min=lat[cell];
   *     if(lat_max<lat[cell])
   *       lat_max=lat[cell];
   *   }
   *   array->nlon=(int)((lon_max-array->lon_min)/config->resolution.lon)+1;
   *   array->nlat=(int)((lat_max-array->lat_min)/config->resolution.lat)+1;
   *   for(cell=0;cell<config->nall;cell++)
   *   {
   *     array->index[cell]=(int)((lon[cell]-array->lon_min)/config->resolution.lon+0.5)+
   *                      (int)((lat[cell]-array->lat_min)/config->resolution.lat+0.5)*array->nlon;
   * #ifdef SAFE
   *     if(array->index[cell]<0 || array->index[cell]>=array->nlon*array->nlat)
   *       fprintf(stderr,"Invalid index %d.\n",array->index[cell]);
   * #endif
   *   }
   * </PRE>
   *
   * This code here was tested using one single CPU.
   * TODO: It is intended to also run in parallel, but that still needs to be tested.
   */
  *reslon_out = config.resolution.lon;
  *reslat_out = config.resolution.lat;
  // Obtaining LPJ cell boundary box is just for testing.
  {
    int lpj_nlons, lpj_nlats;
    double minlonb = 1000, maxlonb = -1000, minlatb = 1000, maxlatb = -1000;
    for (i = 0; i < config.nall; i++) {
      if (lpj_lon_min > grid[i].coord.lon) lpj_lon_min = grid[i].coord.lon;
      if (lpj_lon_max < grid[i].coord.lon) lpj_lon_max = grid[i].coord.lon;
      if (lpj_lat_min > grid[i].coord.lat) lpj_lat_min = grid[i].coord.lat;
      if (lpj_lat_max < grid[i].coord.lat) lpj_lat_max = grid[i].coord.lat;
    }
    lpj_nlons = (int)((lpj_lon_max - lpj_lon_min) / (*reslon_out)) + 1;
    lpj_nlats = (int)((lpj_lat_max - lpj_lat_min) / (*reslat_out)) + 1;
    minlonb = lpj_lon_min - (*reslon_out * 0.5);
    maxlonb = lpj_lon_max + (*reslon_out * 0.5);
    minlatb = lpj_lat_min - (*reslat_out * 0.5);
    maxlatb = lpj_lat_max + (*reslat_out * 0.5);
    printf("LPJ grid: %dx%d cells, %g < lon < %g, %g < lat < %g FMS grid: %dx%d cells, %g < lon < %g, %g < lat < %g\n",
           lpj_nlons, lpj_nlats, minlonb, maxlonb, minlatb, maxlatb,
           *nlons_in, *nlats_in, *minlonb_in, *maxlonb_in, *minlatb_in, *maxlatb_in);
    //assert(minlonb >= 0.0 && minlonb <= 360.0);
    //assert(maxlonb >= 0.0 && maxlonb <= 360.0);
    //assert(minlatb >= -90.0 && minlatb <= 90.0);
    //assert(maxlatb >= -90.0 && maxlatb <= 90.0);
  }

  // What we really need to use is the FMS boundary box, but with LPJ resolution.
  {
    double nlons_lpj_in_fms = (*maxlonb_in - *minlonb_in) / config.resolution.lon;
    double nlats_lpj_in_fms = (*maxlatb_in - *minlatb_in) / config.resolution.lat;
    printf("LPJ resolution in FMS domain boundaries yields %gx%g cells\n",
           nlons_lpj_in_fms, nlats_lpj_in_fms);
    // make sure we embed integral number of LPJ cells into the FMS domain
    assert(double_eq(nlons_lpj_in_fms, (double)((int)nlons_lpj_in_fms), epsilon));
    assert(double_eq(nlats_lpj_in_fms, (double)((int)nlats_lpj_in_fms), epsilon));
    *nlons_out = nlons = (int)nlons_lpj_in_fms;
    *nlats_out = nlats = (int)nlats_lpj_in_fms;
  }
  // cell centers of border cells in FMS domain in LPJ resolution
  lpj_lon_min = *minlonb_in + config.resolution.lon * 0.5;
  lpj_lon_max = *maxlonb_in - config.resolution.lon * 0.5;
  lpj_lat_min = *minlatb_in + config.resolution.lat * 0.5;
  lpj_lat_max = *maxlatb_in - config.resolution.lat * 0.5;

  lpj_to_fms_indices = newvec(int, config.nall);
  fms_to_lpj_indices = newvec(int, nlons*nlats);
  if (NULL == lpj_to_fms_indices) { perror("malloc lpj_to_fms_indices failed"); exit(1); }
  if (NULL == fms_to_lpj_indices) { perror("malloc fms_to_lpj_indices failed"); exit(1); }
  for (i = 0; i < nlons*nlats; i++) fms_to_lpj_indices[i] = -1;
  printf("lpj_lon_min %f lpj_lat_min %f config.nall %d nlons*nlats %d\n",
         lpj_lon_min, lpj_lat_min, config.nall, nlons*nlats);
  for (i = 0; i < config.nall; i++) {
    // transform -180 .. 180 to 0 .. 360
    double lon = grid[i].coord.lon < 0 ? grid[i].coord.lon + 360.0 : grid[i].coord.lon;
    // mapping from F90 2-D-array indices [i,j] to C 1-D-index is j*imax+i
    lpj_to_fms_indices[i] = (int)((lon - lpj_lon_min) / config.resolution.lon + 0.5)
      + (int)((grid[i].coord.lat - lpj_lat_min) / config.resolution.lat + 0.5)* nlons;
    //printf("cell %6d %8.3f=%7.3f/%7.3f index %6d\n", i, grid[i].coord.lon, lon, grid[i].coord.lat, lpj_to_fms_indices[i]);
    if (lpj_to_fms_indices[i] < 0 || lpj_to_fms_indices[i] >= nlons*nlats) {
      fprintf(stderr, "Error: Invalid Cell-to-FMS index %d for Cell %d at %g,%g\n",
              lpj_to_fms_indices[i], i, grid[i].coord.lon, grid[i].coord.lat);
      abort();
    }
    // check that each entry in lpj_to_fms_indices[] is unique
    if (-1 != fms_to_lpj_indices[lpj_to_fms_indices[i]]) {
      fprintf(stderr, "Error: duplicate Cell-to-FMS index %d for Cell %d at %g,%g\n",
              lpj_to_fms_indices[i], i, grid[i].coord.lon, grid[i].coord.lat);
      abort();
    }
    fms_to_lpj_indices[lpj_to_fms_indices[i]] = i;
  }

  mevap_yesterday = newvec(Real, config.ngridcell);
  mtransp_yesterday = newvec(Real, config.ngridcell);
  mevap_lake_yesterday = newvec(Real, config.ngridcell);
  mevap_res_yesterday = newvec(Real, config.ngridcell);
}


/**
 * Second step of LPJ initialisation. The Fortran side now has allocated
 * some data structures based on the information returned by lpj_init_().
 * Here we fill in those structures.
 * Also, runoff mapping from land coast to adjacent ocean cells is initialized now.
 */
#ifdef __cplusplus
extern "C"
#endif
void lpj_init_grid_interpolation_
(
 /*! output parameters */
 double *lons, /*< 1-D array for cell center longitudes, for output axis description */
 double *lats, /*< 1-D-array for cell center latitudes, for output axis description */
 double *land_mask, /*< 2-D array, 1=land cell, 0 elsewhere */
 double *runoff_map_out, /* 2-D array, 1=runoff destination, 0=else */
 /*! and finally some interface-checking args again */
const int *const fse
 ) {
  int i, j;

  if (*fse != 4711) { printf("Error: %s : parameter test failed: 4711 != %d\n", __FUNCTION__, *fse); fflush(stdout); abort(); }

  // set up land-sea-mask and cell center coordinate vectors
  for (i = 0; i < nlons; i++) lons[i] = -100000.0;
  for (i = 0; i < nlats; i++) lats[i] = -100000.0;
  for (i = 0; i < nlons*nlats; i++) land_mask[i] = 0.0;
  for (i = 0; i < config.nall; i++) { // loop over cells
    double lon = grid[i].coord.lon < 0 ? grid[i].coord.lon + 360.0 : grid[i].coord.lon;
    int ilon = (int)((lon - lpj_lon_min) / config.resolution.lon + 0.5);
    int jlat = (int)((grid[i].coord.lat - lpj_lat_min) / config.resolution.lat + 0.5);
    assert(ilon >= 0 && ilon < nlons);
    assert(jlat >= 0 && jlat < nlats);
    assert(lons[ilon] == -100000.0 || lons[ilon] == lon);
    assert(lats[jlat] == -100000.0 || lats[jlat] == grid[i].coord.lat);
    lons[ilon] = lon;
    lats[jlat] = grid[i].coord.lat;
    land_mask[lpj_to_fms_indices[i]] = 1.0;
  }

  // Now the coordinate vectors still contain lots of missing values.
  // Fill them up according to bounding box and resolutions.
  // make sure that for already-filled-in cells we get the same values here as above
  for (i = 0; i < nlons; i++) {
    assert(-100000.0 == lons[i] || lpj_lon_min + i*config.resolution.lon == lons[i]);
    /*if (-100000.0 == lons[i])*/ lons[i] = lpj_lon_min + i*config.resolution.lon;
  }
  for (i = 0; i < nlats; i++) {
    assert(-100000.0 == lats[i] || lpj_lat_min + i*config.resolution.lat == lats[i]);
    /*if (-100000.0 == lats[i])*/ lats[i] = lpj_lat_min + i*config.resolution.lat;
  }

  {
    /*! River runoff routing into the ocean.
     * Now we have all information to set up a map of runoff points into the ocean.
     * In the LPJ river routing scheme, every cell which does not send its runoff into a neighbouring
     * _land_ cell, has its discharge.next target index set to -1.
     * That concerns ocean coast cells, but possibly also inland coast cells (Caspian Sea ...).
     *
     * Now we can iterate through the LPJ cell array, look for cells with discharge.next set to -1,
     * and check in the above-set-up land_mask if this is a coast cell.
     * If so, we define one adjacent ocean cell as discharge target.
     *
     * NOTE: we must distinguish between ocean cells according to DDM30
     * and ocean cells according to FMS land-sea-mask.
     * In several places DDM30 says there is ocean, where has land.
     * The DDM30 runoff chains end before such cells. Usually, there is no adjacent
     * FMS ocean cell in that case, which effectively creates an inland sink,
     * for which the runoff water is lost.
     * Sigh.
     * TODO: search for runoff path extension from such cells into (nearest) FMS ocean cells.
     *
     */
    int noceancells = 0, ninlandsinks = 0, ncoastcells = 0;
    runoff_map = (struct Runoff_map *)malloc(sizeof(struct Runoff_map) * config.nall);
    for (i = 0; i < config.nall; i++) { // loop over cells
      double lon = grid[i].coord.lon < 0 ? grid[i].coord.lon + 360.0 : grid[i].coord.lon;
      int ilon = (int)((lon - lpj_lon_min) / config.resolution.lon + 0.5);
      int jlat = (int)((grid[i].coord.lat - lpj_lat_min) / config.resolution.lat + 0.5);
      int ilond = -9999999;
      int jlatd = -9999999;
      runoff_map[i].from_i = ilon;
      runoff_map[i].from_j = jlat;
      runoff_map[i].to = -1;

      if (grid[i].discharge.next == -9) { /* DDM30 says this an ocean cell */
        noceancells++;
        printf("cell %5d %7.2f=%7.2f %6.2f %4d/%4d=%6d next %2d DDM30 ocean cell\n",
               i, grid[i].coord.lon, lon, grid[i].coord.lat, ilon, jlat, jlat*nlons+ilon, grid[i].discharge.next);
        continue;
      }
      if (grid[i].discharge.next >= 0
          || (!use_runoff_antarctica_from_lpj && grid[i].coord.lat < -55.0)
          || grid[i].discharge.next == -9) { /* DDM30 says this an ocean cell */
        printf("cell %5d %7.2f=%7.2f %6.2f %4d %4d next %2d inland or antarctica cell\n",
               i, grid[i].coord.lon, lon, grid[i].coord.lat, ilon, jlat, grid[i].discharge.next);
        continue;
      } else {
        int naoc = 0; // number of adjacent FMS ocean cells;
        /* aarrgghh. v is #defined to something in LPJ include/discharge.h */
        char vv[9] = "        "; // 8 blanks+\0 for visualisation
        int d;

        for (d = 0; d < 8; d++) { // loop over 8 neighbor cells
          /*                  NE  E   SE   S  SW   W  NW   N */
          static int di[8] = { +1, +1, +1,  0, -1, -1, -1,  0 };
          static int dj[8] = { +1,  0, -1, -1, -1,  0, +1, +1 };
          static char c[8] = {'`','>','\\','v','/','<','\'','^'};
          ilond = ilon+di[d];
          jlatd = jlat+dj[d];
          if (jlatd < 0 || jlatd >= nlats) continue; /* No wrap-around at the poles. */
          /* At least in present-day geography, we do not have coast lines near the poles,
           * so that it should not be necessary to wrap river runoff across the poles.
           */
          if (ilond < 0)      ilond = nlons+ilond; /* wrap-around W->E */
          if (ilond >= nlons) ilond = nlons-ilond; /* wrap-around E->W */
          if (land_mask[jlatd*nlons+ilond] < 0.5) { // found an adjacent ocean cell according to FMS land mask
            vv[d] = c[d];
            naoc++;
            runoff_map[i].to   = jlatd*nlons + ilond;
          }
        }
        printf("cell %5d %7.2f=%7.2f %6.2f %4d/%4d=%6d next %2d to %6d=%4d/%4d naoc %d %s\n",
               i, grid[i].coord.lon, lon, grid[i].coord.lat, ilon, jlat, jlat*nlons+ilon, grid[i].discharge.next,
               runoff_map[i].to, /* lon of dest, lat of dest, */ ilond, jlatd,
               naoc, vv);
        if (naoc == 0) ninlandsinks++; // this is an inland sink, not adjacent to FMS ocean.
        else ncoastcells++;

        // TODO: find largest connected set of adjacent ocean cells, and use middle one as outlet,
        //       instead of the arbitrary one which was set above

      } // else grid[i].discharge.next >= 0 ...
    } // loop over cells
    printf("LPJ river->ocean routing: found %d ocean cells according to DDM30, found %d drainless inland sinks, %d coast cells\n",
           noceancells, ninlandsinks, ncoastcells);
  } // scope for noceancells, ninlandsinks, ncoastcells

  // sigh.
  // DDM30 does not contain data for Antarctica.
  // The river_data.nc in the GFDL-supplied ESM2M_pi-control_C2 example setup contains
  // data for Antarctica, at 144x90 resolution.
  // The river_destination_field files in the GFDL-supplied CM2M_coarse_BLING and CM2.1p1
  // example setups contain (different) Antarctica data at 96x80 resolution.

  for (i = 0; i < config.nall; i++) { // loop over cells
    if (grid[i].discharge.next >= 0) {
      // regular inland runoff, just for information
      // runoff_map[i].to is -1 here.
      int next = grid[i].discharge.next;
      double lon = grid[next].coord.lon < 0 ? grid[next].coord.lon + 360.0 : grid[next].coord.lon;
      double lat = grid[next].coord.lat;
      int ilond = (int)((lon - lpj_lon_min) / config.resolution.lon + 0.5);
      int jlatd = (int)((lat - lpj_lat_min) / config.resolution.lat + 0.5);
      int d = jlatd * nlons + ilond;
      assert(d >= 0);
      assert(d < nlons*nlats);
      if (runoff_map_out[d] < -1) runoff_map_out[d] = 0;
      runoff_map_out[d]++;
    } else {
      if (runoff_map[i].to < 0) {
        // in-land sink
        assert(runoff_map[i].from_i + runoff_map[i].from_j*nlons >= 0
               && runoff_map[i].from_i + runoff_map[i].from_j*nlons < nlons*nlats);
        runoff_map_out[runoff_map[i].from_i + runoff_map[i].from_j*nlons] = -1 /*grid[i].discharge.next*/;
      } else {
        assert(runoff_map[i].to >= 0);
        assert(runoff_map[i].to < nlons*nlats);
        /* leave in-land sinks at -1 to make them stand out in the map */
        /* or maybe even add -1 for every inflow to the sink cell ? */
        if (runoff_map_out[runoff_map[i].to] < -1) runoff_map_out[runoff_map[i].to] = 0;
        runoff_map_out[runoff_map[i].to]++;
      }
    }
  }
#if 0
  { // for debugging, print out a nice ASCII map
    static char c[] =
      {'_', /* -10 = not covered by LPJ cell */
       /*-9*/'~','?','?','?','?','?','?','?','!'/*-1*/,
       /* 0*/'0','1','2','3','4','5','6','7','8','9',
       /*10*/'A','B','C','D','E','F','G','H','I','J',
       /*  */'K','L','M','N','O','P','Q','R','S','T',
       '@'
      };
    for (j = nlats-1; j >=0; j--) {
      for (i = 0; i < nlons; i++) {
        int next = runoff_map_out[i+j*nlons] + 10;
        if (next >= sizeof(c)) next = sizeof(c)-1;
        printf("%c", c[next]);
      }
      printf("\n");
    }
  }
#endif
}


/**
 * Update LPJ by one day's time step.
 * The original main program uses one iterate() function, which loops
 * over the entire simulation time, and for every year a function iterate_year()
 * is invoked. That in turn loops over months, and inside that over days.
 * We must break up the original iterate() function hierarchy so that it does one
 * tiny time step with every update invocation. The update function must then check
 * for new year / new month events to perform the necessary init / finish operations
 * according to the event.
 */
#ifdef __cplusplus
extern "C"
#endif
void lpj_update_
(  const int *const Time_year, const int *const Time_month, const int *const Time_day,
   const int *const Time_hour, const int *const Time_minute, const int *const Time_seconds,
   /*! get climate status information. Fortran 90 arrays of dimension 0:je-js, 0:ie-is */
   //const double *fprec_in, //< TODO: use liquid and frozen precipitation as given by FMS
   //const double *lprec_in,
   const double *prec_in, //< currently, LPJ expects total precipitation and calculates liquid/frozen itself
   const double *temp_mean_in,
   const double *temp_max_in,
   const double *temp_min_in,
   const double *swdown_in,
   const double *lwnet_in,
   const double *wind_in,
   const double *co2_in,
   /*! pass back results from LPJ */
   double *carbon_flux_out, /* daily carbon flux from LPJ to atmosphere (gC/m2/day) */
   double *evap_out,
   double *runoff_out,
   double *roughness_length_out,
   double *surface_temp_out,
   /*! TODO: folia protective cover [%] */
   /*! TODO: LAI */
   double *albedo_out,
   /*! minimal interface check */
   const int *const fortytwo /*< last arg, minimal safety against interface changes */
  ) {
  /* Starting simulation */

  /* broken up invocation of loop over the months, see above */

  /* main loop over spinup + simulation years  */
  /* for(year=config.firstyear-config.nspinup;year<=config.lastyear;year++) */
  {
    /*const int*/ year = *Time_year;
    const int month = *Time_month - 1; /* dailyclimate.c: month (0..11) */
    const int dayofmonth = *Time_day - 1;

    static int yesterday = -1;
    // TODO: make sure criteria for goodnight etc match those used in lpj_driver.F90 _exactly_
    // or: get those flags as input parameters from caller
    const Bool goodmorning = yesterday != dayofmonth;
    const Bool goodnight = (*Time_hour*3600+*Time_minute*60+*Time_seconds + dt_fast) >= NSECONDSDAY; /* last timestep of the day */
    const Bool newmoon = goodmorning && 0 == dayofmonth;
    const Bool monthend = goodnight && dayofmonth == ndaymonth[month] - 1;
    const Bool happynewyear = newmoon && 0 == month;
    const Bool silvester = (goodnight && yesterday != -1  /* not before the first simulation year */
                             && 11 == month && 30 == dayofmonth);

    assert(42 == *fortytwo); /* make sure that number of parameters matches between F90 and C */

    /* TODO: the ``real work'' of daily updates is called only once per day
     * (at goodnight time)
     * Thus we should possibly check right here if goodnight is true,
     * and just return otherwise.
     */

    if (isroot(config))
      printf("%s %04d-%02d-%02d-%02d:%02d:%02d %s %s %s %s %s %s\n",
             __FUNCTION__,
             year, month, dayofmonth, *Time_hour, *Time_minute, *Time_seconds,
             goodmorning ? "goodmorning" : "",
             goodnight   ? "goodnight"   : "",
             newmoon     ? "newmoon"     : "",
             monthend    ? "monthend"    : "",
             happynewyear? "happynewyear": "",
             silvester   ? "silvester"   : "");

    /* some things must be done at start of a new year only */
    if (happynewyear) { /* from iterate() */

#ifdef IMAGE
      if(year>=config.start_imagecoupling)
        co2=receive_image_co2(&config);
      else
#endif
      if (input.climate->co2.data) // invoke getco2() when reading from real file, not FMS
      co2=getco2(input.climate,year); /* get atmospheric CO2 concentration */
      /* else: use CO2 as given by LPJ coupler - done below in daily loop-over-cells */

      //if(year<input.climate->firstyear) /* are we in spinup phase? */
      ///* yes, let climate data point to stored data */
//#ifdef STORECLIMATE
      //  moveclimate(input.climate,store,
      //              (year-config.firstyear+config.nspinup) % config.nspinyear);
//#else
      //getclimate(input.climate,grid,input.climate->firstyear+(year-config.firstyear+config.nspinup) % config.nspinyear,&config);
//#endif
      //else
      {
#ifdef STORECLIMATE
        if(year==input.climate->firstyear && config.nspinup)
        {
          /* restore climate data pointers to initial data */
          input.climate->data=data_save;
          freeclimatedata(&store); /* free data not used anymore */
        }
#endif
        /* read climate from files */
#ifdef IMAGE
        if(year>=config.start_imagecoupling)
        {
          if(receive_image_climate(input.climate,year,&config))
          {
            fprintf(stderr,"ERROR104: Simulation stopped in receive_image_climate().\n");
            fflush(stderr);
            /*break;*/ /* leave time loop */ abort();
          }
        }
        else
#endif
          if(getclimate(input.climate,grid,year,&config))
          {
            fprintf(stderr,"ERROR104: Simulation stopped in getclimate().\n");
            fflush(stderr);
            /*break;*/ /* leave time loop */ abort();
          }
      }
      if(input.landuse!=NULL)
      {
        calc_seasonality(grid,npft,ncft,&config);
        if(config.withlanduse==CONST_LANDUSE || config.withlanduse==ALL_CROPS) /* constant landuse? */
          landuse_year=param.landuse_year_const;
        else if(year>2000)			/* 2 lines added to use year 2000 landuse after 2000 if climate data is used */
          landuse_year=2000;			/* MF 2010-08-25 */
        else
          landuse_year=year;
        /* under constand landuse also keep wateruse at landuse_year_const */
        if(config.withlanduse==CONST_LANDUSE)
          wateruse_year=param.landuse_year_const;
        else wateruse_year=year;
#ifdef IMAGE
        if(year>=config.start_imagecoupling)
        {
          if (receive_image_data(grid,npft,ncft,&config))
          {
            fprintf(stderr,"ERROR104: Simulation stopped in receive_image_data().\n");
            fflush(stderr);
            /*break;*/ /* leave time loop */ abort();
          }
        }
        else
#endif
          /* read landuse pattern from file */
          if(getlanduse(input.landuse,grid,landuse_year,ncft,&config))
          {
            fprintf(stderr,"ERROR104: Simulation stopped in getlanduse().\n");
            fflush(stderr);
            /*break;*/ /* leave time loop */ abort();
          }
        if(config.reservoir)
          allocate_reservoir(grid,year,&config);
      }
      if(input.wateruse!=NULL)
      {
        /* read wateruse data from file */
        if(getwateruse(input.wateruse,grid,wateruse_year,&config))
        {
          fprintf(stderr,"ERROR104: Simulation stopped in getwateruse().\n");
          fflush(stderr);
          /*break;*/ /* leave time loop */ abort();
        }
      }
      if(config.ispopulation)
      {
        if(readpopdens(input.popdens,year,grid,&config))
        {
          fprintf(stderr,"ERROR104: Simulation stopped in getpopdens().\n");
          fflush(stderr);
          /*return year*/ abort();
        }
      }
      if(year>=config.firstyear)
        openoutput_yearly(output,year,&config);

    } /* if (happynewyear) */

    /* perform iteration for one year */
    /* broken up iterateyear_river() to be called on every time step */
    if(output->method==LPJ_SOCKET || config.river_routing) {
      /*iterateyear_river(output,grid,input,co2,npft,ncft,&config,year);*/
      { /* begin iterateyear_river() */
        static Dailyclimate daily;
        static Bool intercrop,istimber;
        /* dailyclimate.c: month (0..11) day (1..365) */
        static int /*month,*/ /*dayofmonth,*/ dayofyear;
        int cell,i;
        static Real popdens=0; /* population density (capita/km2) */

        if (happynewyear) { /* things from iterateyear_river() to be done at begin of year */
          popdens=0;
#ifdef IMAGE
          istimber=(config.start_imagecoupling!=INT_MAX);
#else
          istimber=FALSE;
#endif
          intercrop=getintercrop(input.landuse);
          for(cell=0;cell<config.ngridcell;cell++)
          {
            grid[cell].output.adischarge=0;
            grid[cell].output.surface_storage=0;
            if(!grid[cell].skip)
            {
              init_annual(grid+cell,npft,config.nbiomass,config.nagtree,ncft);
              if(input.landuse!=NULL)
              {
                if(grid[cell].lakefrac<1)
                {
                  /* calculate landuse change */
                  if(!config.isconstlai)
                    laimax_manage(&grid[cell].ml.manage,config.pftpar+npft,npft,ncft,year);
                  if(year>config.firstyear-config.nspinup)
                    landusechange(grid+cell,config.pftpar,npft,config.nagtree,ncft,config.ntypes,
                                  intercrop,istimber,year,config.pft_output_scaled);
                  else if(grid[cell].ml.dam)
                    landusechange_for_reservoir(grid+cell,config.pftpar,npft,istimber,
                                                intercrop,ncft);
                }
#ifdef IMAGE
                setoutput_image(grid+cell,ncft);
#endif
              }
              initgdd(grid[cell].gdd,npft);
            } /*gridcell skipped*/
          } /* of for(cell=...) */
          dayofyear = 1;
        } /* if (happynewyear) */

        //day=1;
        //foreachmonth(month)
        if (newmoon) {
          for(cell=0;cell<config.ngridcell;cell++)
          {
            grid[cell].discharge.mfin=grid[cell].discharge.mfout=grid[cell].output.mdischarge=
              grid[cell].ml.mdemand=0.0;
            if(!grid[cell].skip)
            {
              initoutput_monthly(&((grid+cell)->output));
              mevap_yesterday[cell] = mtransp_yesterday[cell] = 0.0; // reset yesterday's saved values
              mevap_lake_yesterday[cell] = mevap_res_yesterday[cell] = 0.0; // reset yesterday's saved values
              /* Initialize random seed */
              if(israndomprec(input.climate))
                srand48(config.seed+(config.startgrid+cell)*year*month);
              initclimate_monthly(input.climate,&grid[cell].climbuf,cell,month);
#ifdef IMAGE
              monthlyoutput_image(&grid[cell].output,input.climate,cell,month);
#endif

#ifdef DEBUG
            /*  printf("temp = %.2f prec = %.2f wet = %.2f",
                     (getcelltemp(input.climate,cell))[month],
                     (getcellprec(input.climate,cell))[month],
                     (israndomprec(input.climate)) ? (getcellwet(input.climate,cell))[month] : 0);
              if(config.with_radiation)
                printf("lwnet = %.2f swdown = %.2f\n",
                       (getcelllwnet(input.climate,cell))[month],
                       (getcellswdown(input.climate,cell))[month]);
              else
                printf("sun = %.2f\n",(getcellsun(input.climate,cell))[month]);*/
#endif
            }
          } /* of 'for(cell=...)' */
        } /* if(newmoon) */

        /*foreachdayofmonth(dayofmonth,month)*/  /* for(day=0;day<ndaymonth[month];day++) */
        if (goodnight) {  /* do it only once per day */
          for(cell=0;cell<config.ngridcell;cell++)
          {
            if(!grid[cell].skip)
            {
              if(config.ispopulation)
                popdens=getpopdens(input.popdens,cell);
              grid[cell].output.dcflux=0;
              initoutput_daily(&(grid[cell].output.daily));
              /* get daily values for temperature, precipitation and sunshine */

              /* Fill in the daily structure with the current values we got from
               * land_lad. Those values, for which the LPJ input configuration
               * specifies FMS_COUPLER instead of a file name, will be left untouched
               * in dailyclimate(). Values for which a file name is configured, are
               * overwritten.
               */
              assert(lpj_to_fms_indices[cell] >= 0 && lpj_to_fms_indices[cell] < nlons*nlats);
              daily.temp       = temp_mean_in[lpj_to_fms_indices[cell]];
              daily.prec       = prec_in     [lpj_to_fms_indices[cell]];
              daily.windspeed  = wind_in     [lpj_to_fms_indices[cell]];
              daily.tmin       = temp_min_in [lpj_to_fms_indices[cell]];
              daily.tmax       = temp_max_in [lpj_to_fms_indices[cell]];
              //daily.lightning= lightning_in[lpj_to_fms_indices[cell]];
              daily.lwnet      = lwnet_in    [lpj_to_fms_indices[cell]];
              daily.swdown     = swdown_in   [lpj_to_fms_indices[cell]];

              dailyclimate(&daily,input.climate,&grid[cell].climbuf,cell,dayofyear,
                           month,dayofmonth);
              // Get current CO2 value from LPJ_COUPLER / land_lad
              // When receiving data from FMS, we get daily per-cell CO2 values.
              // When reading from file via getco2() we get globally and annually averaged values.
              if (NULL == input.climate->co2.data) // if reading from FMS, not from real file via getco2() above
                co2 = co2_in[lpj_to_fms_indices[cell]]; /* get atmospheric CO2 concentration */
              /* get daily values for temperature, precipitation and sunshine */
              grid[cell].output.daily.temp=daily.temp;
              grid[cell].output.daily.prec=daily.prec;
              //grid[cell].output.daily.sun=daily.sun; not used for FMS coupling

#ifdef DEBUG
              //printf("day=%d cell=%d CO2=%g\n",dayofyear,cell, co2);
#endif
              update_daily(grid+cell,co2,popdens,daily,dayofyear,npft,ncft,
                           year,month,output->withdaily,intercrop,&config);
            }
          }


          if(config.river_routing)
          {
            if(input.landuse!=NULL || input.wateruse!=NULL)
              withdrawal_demand(grid,&config);

            drain(grid,&config,month);

            if(input.landuse!=NULL || input.wateruse!=NULL)
              wateruse(grid,npft,ncft,&config);

          }

          if(output->withdaily && year>=config.firstyear)
            fwriteoutput_daily(output,grid,dayofyear-1,year,&config);

          // TODO: copy daily output into output variables for land_lad
          for (cell=0; cell<config.ngridcell; cell++) {
            if (!grid[cell].skip) {
              Real temp = 0.0, fracsum = 0.0;

              // in daily_natural.c:
              // output->mnpp+=npp*stand->frac;
              // i.e. it is summed up each day. Thus we need to remember yesterday's
              // value, and compute the difference each day.
              // npp = assimilation of CO2 from atmosphere into soil
              // [g C / m^2 ] - difference per day yields [g C / m^2 / day]
              //
              // in update_daily.c:
              // cell->output.mrh+=hetres*stand->frac;
              // mrh = soil respiration, i.e. source of CO2 going from soil into atmosphere
              // [g C / m^2 ] - difference per day yields [g C / m^2 / day]
              // ---
              // nep = npp - rh
              // in update_monthly.c:
              //grid[cell].balance.nep Pro Zelle  [g C/(m^2 * day)]
              //  positiv: Senke
              //  negativ: Emission
              // grid[cell].coord.area [m^2]
              // But this is done only monthly!
              //
              // grid[cell].output.firec [g C/(m^2 * day)]
              // positiv: Emission
              // negativ: geht nicht
              // Standard: computed annually
              // With SPITFIRE: computed daily
              //
              // grid[cell].output.flux_harvest [g C/(m^2 * day)]
              //   Ernte
              //   geht nicht direkt in die Atmopshaere -> hier unberuecksichtigt erstmal
              //  (in land_lad2 kommt sowas offenbar auch vor)
              //
              // in update_daily.c:
              // grid[cell].output.flux_estab
              //  positiv: Senke
              //  negativ: geht nicht
              // Carbon intake of saplings - additional to npp
              // [ g C / m^2 / day ]
              //
              // Umrechnung g C -> CO2 ueber Mol-Gewicht (12)
              //
              // CO2 fertilization abschalten, CO2 auf festem Wert lassen z.B. 288 ppm
              // ---
              // All the above is now summed up daily in output.dcflux (gC/m2/day)
              carbon_flux_out[lpj_to_fms_indices[cell]] = grid[cell].output.dcflux;

              // Evaporation: accumulated monthly in cell[].output.mevap.
              //              The daily value is stored in a local variable in update_daily
              // WvB: evap contains only the part that comes through sublimation of snow into the atmosphere,
              //      and is calculated in the permafrost module only.
              //      Other sources are given through output->mtransp.
              // Here we remember yesterdays's monthly accumulated transpiration and evaporation,
              // and subtract it from today's monthly sum.
              //assert(lpj_to_fms_indices[cell] >= 0 && lpj_to_fms_indices[cell] < nlons*nlats);
              evap_out[lpj_to_fms_indices[cell]] =
                grid[cell].output.mtransp - mtransp_yesterday[cell]
                + grid[cell].output.mevap - mevap_yesterday[cell]
                + grid[cell].output.mevap_lake - mevap_lake_yesterday[cell]
                + grid[cell].output.mevap_res - mevap_res_yesterday[cell];
              mtransp_yesterday[cell] = grid[cell].output.mtransp;
              mevap_yesterday[cell] = grid[cell].output.mevap;
              mevap_lake_yesterday[cell] = grid[cell].output.mevap_lake;
              mevap_res_yesterday[cell] = grid[cell].output.mevap_res;

              // Runoff: grid[cell].discharge.fout contains the daily runoff [m3/s]
              //   It is updated in drain() above.
              // The actual ocean cells that the runoff is going into are entered into runoff_map
              // during lpj_init_grid_interpolation_().
              // In LPJ the land cells from which the runoff does not go into neighbor land cells,
              // just have a negative runoff-neighbor index, pointing to nowhere.
              // Here, we move runoff from those cells into the corresponding ocean cell.
              if (grid[cell].discharge.next < 0) {
                double lon = grid[cell].coord.lon < 0 ? grid[cell].coord.lon + 360.0 : grid[cell].coord.lon;
                if (runoff_map[cell].to < 0) {
                  // in-land sink - water is lost
                  printf("runoff cell %5d %7.2f=%7.2f %6.2f next %2d to %6d inland sink\n",
                         cell, grid[cell].coord.lon, lon, grid[cell].coord.lat,
                         grid[cell].discharge.next,
                         runoff_map[cell].to);
                } else {
                  printf("runoff cell %5d %7.2f=%7.2f %6.2f next %2d to %6d coast cell\n",
                         cell, grid[cell].coord.lon, lon, grid[cell].coord.lat,
                         grid[cell].discharge.next,
                         runoff_map[cell].to);
                    runoff_out[runoff_map[cell].to] = grid[cell].discharge.fout; // [liter / day] ?
                  // in drain.c, fout*1e-3*dayseconds1 yields [m3/sec]
                }
              }
              else {
                double lon = grid[cell].coord.lon < 0 ? grid[cell].coord.lon + 360.0 : grid[cell].coord.lon;
                printf("runoff cell %5d %7.2f=%7.2f %6.2f next %2d to %6d inland runoff\n",
                       cell, grid[cell].coord.lon, lon, grid[cell].coord.lat,
                       grid[cell].discharge.next,
                       runoff_map[cell].to);
              }
              // for debugging, write out all inland runoff also
              // -> sigh. for inland-sinks, runoff_map[cell].to is -1
              //runoff_out[runoff_map[cell].to] = grid[cell].discharge.fout; // [liter / day] ?
              // in drain.c, fout*1e-3*dayseconds1 yields [m3/sec]

              roughness_length_out[lpj_to_fms_indices[cell]] = roughnesslength(grid[cell].standlist);

              // Soil Temperature of uppermost soil layer:
              //  temp[0] inside soil inside each stand of each cell.
              for (i = 0; i < grid[cell].standlist->n; i++) {
                Real frac = ((struct stand *)(grid[cell].standlist->data)[i])->frac;
                temp += ((struct stand *)(grid[cell].standlist->data)[i])->soil.temp[0] * frac;
                fracsum += frac;
              }
              surface_temp_out[lpj_to_fms_indices[cell]] = temp / fracsum; // fracsum might be < 1 if lakefrac > 0

              albedo_out[lpj_to_fms_indices[cell]] = albedo(&grid[cell]);

            } /* if (!grid[cell].skip) */
          } /* loop over cells */

          dayofyear++;
        } /* if (goodnight) */ /* of 'foreachdayofmonth */

        if (monthend) {
          /* Calculate resdata->mdemand as sum of ddemand to reservoir, instead of the sum of evaporation deficits per cell*/
          /*   if(config.reservoir)
                 sum_irrig_demand(grid,config); */
          for(cell=0;cell<config.ngridcell;cell++)
          {
            if(grid[cell].output.mdischarge<0)
              grid[cell].output.mdischarge=0;
            if(grid[cell].discharge.next<0)
              grid[cell].output.adischarge+=grid[cell].output.mdischarge;
            grid[cell].output.mdischarge*=1e-9/ndaymonth[month]; /* daily mean discharge per month in 1.000.000 m3 per cell */
            grid[cell].output.mres_storage*=1e-9/ndaymonth[month]; /* hb 5-11-09 mean monthly reservoir storage in 1.000.000 m3 per cell */
            if(!grid[cell].skip)
              update_monthly(grid+cell,getmtemp(input.climate,&grid[cell].climbuf,
                                                cell,month),getmprec(input.climate,&grid[cell].climbuf,
                                                                     cell,month),month);
#ifdef DEBUG
          //  printcell(grid+cell,1,ncft,input.landuse!=NULL,TRUE);
#endif
          } /* of 'for(cell=0;...)' */

#ifdef IMAGE
          if(year>=config.firstyear-istimber*10)
#else
            if(year>=config.firstyear)
#endif
              /* write out monthly output */
              fwriteoutput_monthly(output,grid,month,year,&config);

        } /* if (monthend) */ /* of 'foreachmonth */

        if (silvester) { /* from iterateyear_river() */
          for(cell=0;cell<config.ngridcell;cell++)
          {
            if(!grid[cell].skip)
            {

              update_annual(grid+cell,npft,ncft,popdens,year,intercrop,&config);
#ifdef SAFE
              check_fluxes(grid+cell,year,cell,&config);
#endif

#ifdef DEBUG
             /* if(year>config.firstyear)
              {
                printf("year=%d\n",year);
                printf("cell=%d\n",cell);
                printcell(grid+cell,1,ncft,input.landuse!=NULL);
              }*/
#endif
              if(config.nspinup>veg_equil_year &&
                 year==config.firstyear-config.nspinup+veg_equil_year && !config.from_restart)
                equilveg(grid+cell);

              if(config.nspinup>soil_equil_year &&
                 year==config.firstyear-config.nspinup+soil_equil_year && !config.from_restart)
                equilsom(grid+cell,npft+ncft,config.pftpar);

            }

            grid[cell].output.surface_storage=grid[cell].discharge.dmass_lake+grid[cell].discharge.dmass_river;
            if(grid[cell].ml.dam)
            {
              grid[cell].output.surface_storage+=grid[cell].ml.resdata->dmass;
              for(i=0;i<NIRRIGDAYS;i++)
                grid[cell].output.surface_storage+=grid[cell].ml.resdata->dfout_irrigation_daily[i];
            }
          } /* of for(cell=0,...) */

#ifdef IMAGE
          if(year>=config.firstyear-istimber*10)
#else
            if(year>=config.firstyear)
#endif
            {
              /* write out annual output */
              fwriteoutput_annual(output,grid,year,&config);
              fwriteoutput_pft(output,grid,npft,ncft,year,&config);
            }

        } /* if (silvester) */ /* end iterateyear_river() */
      } /* end iterateyear_river */
    } else {
      /* iteration without river routing */
      /*iterateyear(output,grid,input,co2,npft,ncft,&config,year);*/
      fprintf(stderr, "Oops, river routing was not enabled in the LPJ configuration\n");
      exit(1);
    }

    if (silvester) { /* from iterate() */
      if(year>=config.firstyear)
      closeoutput_yearly(output,&config);

      /* calculating total carbon and water fluxes collected from all tasks */
      cflux_total=flux_sum(&flux,grid,&config);
      if(config.rank==0)
      {
        /* output of total carbon flux and water on stdout on root task */
        printflux(flux,cflux_total,year,&config);
        if(output->method==LPJ_SOCKET && output->socket!=NULL &&
           year>=config.firstyear)
          output_flux(output,flux);
        fflush(stdout); /* force output to console */
#ifdef SAFE
        check_balance(flux,year,&config);
#endif
      }
#ifdef IMAGE
      if(year>=config.start_imagecoupling)
      {
        /* send data to IMAGE */
#ifdef DEBUG_IMAGE
        if(config.rank==0)
        {
          printf("sending data to image? year %d startimagecoupling %d\n",
                 year,config.start_imagecoupling);
          fflush(stdout);
        }
#endif
        if(send_image_data(&config,grid,input.climate,npft))
          fail(SEND_IMAGE_ERR,FALSE,
               "Problem with writing maps for transfer to IMAGE");
      }
#endif
      if(iswriterestart(&config) && year==config.restartyear)
        fwriterestart(grid,npft,ncft,year,&config); /* write restart file */
    } /* if (silvester) */

    yesterday = dayofmonth;

  } /* of 'for(year=...)' */

#ifdef STORECLIMATE
  if(config.nspinup && (config.lastyear<input.climate->firstyear || year<input.climate->firstyear))
  {
    /* restore climate data pointers to initial data */
    input.climate->data=data_save;
    freeclimatedata(&store); /* free data not used anymore */
  }
#endif
  return /*year*/;
}


#ifdef __cplusplus
extern "C"
#endif
void lpj_end_() {

  /* Simulation has finished */
  fcloseoutput(output,&config);
  if(isroot(config))
    puts((year>config.lastyear) ? "Simulation ended." : "Simulation stopped.");
  /* free memory */
  freeinput(input,isroot(config));
  freegrid(grid,config.npft[GRASS]+config.npft[TREE],&config);
  if(isroot(config))
  {
    printf( (year>config.lastyear) ? "%s successfully" : "%s errorneously",progname);
    printf(" terminated, %d grid cells processed.\n",
           config.total
           );
  }
#ifdef IMAGE
  if(config.sim_id==LPJML_IMAGE)
    close_image(&config);
#endif
  freeconfig(&config);

  free(mevap_yesterday);
  free(mevap_lake_yesterday);
  free(mevap_res_yesterday);
  free(mtransp_yesterday);
  return /*EXIT_SUCCESS*/ ;
}
