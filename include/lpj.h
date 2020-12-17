/***************************************************************************/
/**                                                                       **/
/**                      l  p  j  .  h                                    **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
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
/**     LPJ header file containing all necessary includes                 **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     See file AUTHORS for list of authors contributing to this code    **/
/**                                                                       **/
/**     Last change: 10.01.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#ifndef LPJ_H /* Already included? */
#define LPJ_H

#define LPJ_VERSION  "3.5.003"

/* Necessary header files */

/* Standard C header files */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#ifdef USE_MPI
#include <mpi.h> /* Include MPI header for parallel program */
#endif

typedef struct cell Cell;   /* forward declaration of cell */
typedef struct stand Stand; /* forward declaration of stand */

/*  Defined header files for LPJ */

#include "list.h"
#include "types.h"
#include "swap.h"
#include "channel.h"
#include "queue.h"
#include "pnet.h"
#include "coord.h"
#include "buffer.h"
#include "climbuf.h"
#include "soil.h"
#include "pftpar.h"
#include "output.h"
#include "date.h"
#include "manage.h"
#include "pft.h"
#include "config.h"
#include "cdf.h"
#include "outfile.h"
#include "param.h"
#include "header.h"
#include "climate.h"
#ifdef IMAGE            /* Compile with IMAGE coupler? */
#include "image.h"
#endif
#include "cropdates.h"
#include "reservoir.h"
#include "landuse.h"
#include "errmsg.h"
#include "pftlist.h"
#include "spitfire.h"
#include "numeric.h"
#include "units.h"
#include "conf.h"
#include "stand.h"
#include "crop.h"
#include "discharge.h"
#include "input.h"
#include "cell.h"
#include "tree.h"
#include "biomass_tree.h"
#include "landuse.h"
#include "biomes.h"

// barrier.n: adding the nitrogen include file
#include "nitrogen.h"

/* Definition of constants */

/* Environment variables */

#define LPJROOT "LPJROOT"            /* LPJ root directory */
#define LPJPREP "LPJPREP"            /* preprocessor command */
#define LPJCONFIG "LPJCONFIG"        /* default LPJ configuration filename */
#define LPJOPTIONS "LPJOPTIONS"      /* LPJ runtime options */
#define LPJINPUT "LPJINPATH"         /* path for input files */
#define LPJOUTPUT "LPJOUTPATH"       /* path for output files */
#define LPJRESTART "LPJRESTARTPATH"  /* path for restart file */
#define LPJOUTPUTMETHOD "LPJOUTPUT"  /* default output method */

/* Declaration of variables */

extern char *usage;

/* Declaration of functions */

extern Cell *newgrid(Config *,const Standtype [],int,int,int);
extern Bool fwriterestart(const Cell[],int,int,int,const Config *);
extern FILE *openrestart(const char *,Config *,int,Bool *);
extern void copyright(const char *);
extern void help(const char *,const char *);
extern void fprintflux(FILE *file,Flux,Real,int,const Config *);
extern void failonerror(const Config *,int,int,const char *);
extern Bool iserror(int,const Config *);

/* Definition of macros */

#define printflux(flux,total,year,config) fprintflux(stdout,flux,total,year,config)

#endif
