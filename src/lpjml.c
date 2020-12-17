/***************************************************************************/
/**                                                                       **/
/**                        l  p  j  m  l  .  c                            **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**     Version with managed land and river routing                       **/
/**                                                                       **/
/**     The model simulates vegetation dynamics, hydrology and soil       **/
/**     organic matter dynamics on an area-averaged grid cell basis using **/
/**     one-year time step. Input parameters are monthly mean air         **/
/**     temperature, total precipitation and percentage of full sunshine, **/
/**     annual atmospheric CO2 concentration and soil texture class. The  **/
/**     simulation for each grid cell begins from "bare ground",          **/
/**     requiring a "spin up" (under non-transient climate) of c. 1000    **/
/**     years to develop equilibrium vegetation and soil structure.       **/
/**                                                                       **/
/**     Main program:                                                     **/
/**                                                                       **/
/**     {                                                                 **/
/**       Cell *grid;                                                     **/
/**       readconfig();                                                   **/
/**       if(config.sim_id==LPJML_IMAGE) open_image();                    **/
/**       initinput();                                                    **/
/**       printconfig();                                                  **/
/**       grid=newgrid();                                                 **/
/**       if(river_routing) initdrain(grid);                              **/
/**       fopenoutput();                                                  **/
/**       iterate(grid);                                                  **/
/**       fcloseoutput();                                                 **/
/**       freegrid(grid);                                                 **/
/**       freeinput();                                                    **/
/**       freeconfig();                                                   **/
/**     }                                                                 **/
/**                                                                       **/
/**     Basic datatype Cell has the following hierarchy:                  **/
/**                                                                       **/
/**     --Cell                                                            **/
/**       +-Coord                                                         **/
/**       +-Managed_land                                                  **/
/**       | +-Landfrac                                                    **/
/**       | +-Resdata                                                     **/
/**       | | +-Resource                                                  **/
/**       | +-Cropdates                                                   **/
/**       | +-Manage                                                      **/
/**       | +-Image_data (only for version with -DIMAGE set)              **/
/**       |   +-Pool                                                      **/
/**       +-Ignition                                                      **/
/**       +-Discharge                                                     **/
/**       | +-Queue                                                       **/
/**       +-Output                                                        **/
/**       +-Climbuf                                                       **/
/**       | +-Buffer                                                      **/
/**       +-Balance                                                       **/
/**       +-Standlist                                                     **/
/**         +-Stand                                                       **/
/**           +-Pftlist                                                   **/
/**           | +-Pft-+-Pftgrass                                          **/
/**           |       +-Pfttree                                           **/
/**           |       +-Pftcrop                                           **/
/**           +-Soil                                                      **/
/**             +-Pool                                                    **/
/**             +-Litter                                                  **/
/**               +-Litteritem                                            **/
/**                 +-Trait                                               **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     See file AUTHORS for list of authors contributing to this code    **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include <time.h>
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
#include "nitrogen.h"   /* barrier.n */

#define NTYPES 3 /* number of plant functional types: grass, tree, annual_crop */
#define NSTANDTYPES 11 /* number of stand types / land use types as defined in landuse.h*/

#define dflt_conf_filename_ml "lpjml.conf" /* Default LPJ configuration file
                                              if called by lpjml */
#define dflt_conf_filename "lpj.conf" /* Default LPJ configuration file
                                         if called by lpj */
int main(int argc,char **argv)
{
    Outputfile *output; /* Output file array */
    const char *progname;
    int year,rc;
    Cell *grid;         /* cell array */
    Input input;        /* input data */
    time_t tstart,tend,tbegin,tfinal;   /* variables for timing */
    Standtype standtype[NSTANDTYPES];
    Config config;         /* LPJ configuration */
    //Real rtemp;

    /* Create array of functions, uses the typedef of (*Fscanpftparfcn) in pft.h */
    Fscanpftparfcn scanfcn[NTYPES]={fscanpft_grass,fscanpft_tree,fscanpft_crop};

    standtype[NATURAL]=natural_stand;
    standtype[SETASIDE_RF]=setaside_rf_stand;
    standtype[SETASIDE_IR]=setaside_ir_stand;
    standtype[AGRICULTURE]=agriculture_stand;
    standtype[MANAGEDFOREST]=managedforest_stand;
    standtype[GRASSLAND]=grassland_stand;
    standtype[BIOMASS_TREE]=biomass_tree_stand;
    standtype[BIOMASS_GRASS]=biomass_grass_stand;
    standtype[AGRICULTURE_TREE]=agriculture_tree_stand;
    standtype[AGRICULTURE_GRASS]=agriculture_grass_stand;
    standtype[KILL]=kill_stand;


    time(&tbegin);         /* Start timing for total wall clock time */

#ifdef USE_MPI
    MPI_Init(&argc,&argv); /* Initialize MPI */
    /*
     * Use default communicator containing all processors. In defining your own
     * communicator it is possible to run LPJ on a subset of processors
     */
    initmpiconfig(&config,MPI_COMM_WORLD);
#else
    /* sequential version of LPJ */
    initconfig(&config);
#endif
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
#ifdef USE_MPI
            MPI_Finalize();
#endif
            return EXIT_SUCCESS;
        }
        else if(!strcmp(argv[1],"-v")) /* check for version option */
        {
            if(isroot(config))
                printflags(progname);
#ifdef USE_MPI
            MPI_Finalize();
#endif
            return EXIT_SUCCESS;
        }
    }
    if(isroot(config))
    { /* Output only for the root task 0 */
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
        fputs("WARNING018: Arguments listed after configuration filename, will be ignored.\n",stderr);
    if(isroot(config))
        printconfig(&config,config.npft[GRASS]+config.npft[TREE],
                config.npft[CROP]);
    if(config.sim_id==LPJML_FMS)
    {
        if(isroot(config))
            fputs("ERROR032: FMS coupler not supported.\n",stderr);
        return NO_FMS_ERR;
    }
#ifdef IMAGE
    if(config.sim_id==LPJML_IMAGE)
    {
        rc=open_image(&config);
        failonerror(&config,rc,OPEN_IMAGE_ERR,"Cannot open IMAGE coupler");
    }
#endif
    /* Allocation and initialization of grid */
    rc=((grid=newgrid(&config,standtype,NSTANDTYPES,config.npft[GRASS]+config.npft[TREE],config.npft[CROP]))==NULL);
    failonerror(&config,rc,INIT_GRID_ERR,"Initialization of LPJ grid failed");

    rc=initinput(&input,grid,config.npft[CROP],&config);
    /* barrier.n: test on reading the water temperature file */
    if (config.with_nitrogen){


        if(config.with_wastewater){
            // opening the files associated with wastewater (yearly files for N/P/C)
            //printf("Initialisation of wastewater\n");
            rc = nit_init_wastewater_country_par(&config);
        }

        if(config.with_manure){
            //printf("Initialisation of manure\n");
            // opening AND READING of the 2D sedac variable (i.e no time dimension!!!!)
            rc = nit_init_manure_sedac(&config, grid);
            // opening the manure file for N manure and pasture (yearly files)
            rc = nit_init_manure_n(&config);
        }

        if(config.with_ferti){
            //printf("Initialisation of fertilizers\n");
            // opening the fertilizers file for N and P
            rc = nit_init_fert(&config);
        }

        // if manure or fert, we need the countfrac arrays
        if(config.with_ferti || config.with_manure){
            //printf("Initialisation of country fractions\n");
            nit_init_countfrac(&config);
        }

    }
    failonerror(&config,rc,INIT_INPUT_ERR,
            "Initialization of input data failed");
    rc=checkvalidclimate(input.climate,grid,&config);
    failonerror(&config,rc,INIT_INPUT_ERR,
            "Check of climate data failed");
    /* open output files */
    output=fopenoutput(grid,NOUT,&config);

    if(isopen(output,GRID))
        writecoords(output,GRID,grid,&config);
    if(isopen(output,COUNTRY) && config.withlanduse)
        writecountrycode(output,COUNTRY,grid,&config);
    if(isopen(output,REGION) && config.withlanduse)
        writeregioncode(output,REGION,grid,&config);

    // barrier.n: print the nitrogen log
    if(isroot(config))
        nit_printlog(&config);
    if(isroot(config))
        puts("Simulation begins...");
    time(&tstart); /* Start timing */
    /* Starting simulation */
    year=iterate(output,grid,input,
            config.npft[GRASS]+config.npft[TREE],config.npft[CROP],
            &config);
    /* Simulation has finished */
    time(&tend); /* Stop timing */
    fcloseoutput(output,&config);
    if(isroot(config))
        puts((year>config.lastyear) ? "Simulation ended." : "Simulation stopped.");
    /* free memory */
    freeinput(input,isroot(config));
    freegrid(grid,config.npft[GRASS]+config.npft[TREE],&config);
    if(isroot(config))
    {
        printf( (year>config.lastyear) ? "%s successfully" : "%s errorneously",progname);
        printf(" terminated, %d grid cells processed.\n"
                "Wall clock time:\t%d sec, %.2g sec/cell/year.\n",
                config.total,(int)(tend-tstart),
                (double)(tend-tstart)/config.total/(year-config.firstyear+
                    config.nspinup));
    }
#ifdef IMAGE
    if(config.sim_id==LPJML_IMAGE)
        close_image(&config);
#endif
    freeconfig(&config);
#ifdef USE_MPI
    /* Wait until all tasks have finished to measure total wall clock time */
    MPI_Barrier(MPI_COMM_WORLD);
    /* Exit MPI */
    MPI_Finalize();
#endif
    time(&tfinal);
    if(isroot(config))
    {
        printf("Total wall clock time:\t%d sec (",(int)(tfinal-tbegin));
        printtime(tfinal-tbegin);
        puts(").");
    }
    return EXIT_SUCCESS;
} /*of 'main' */
