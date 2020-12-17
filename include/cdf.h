/***************************************************************************/
/**                                                                       **/
/**                              c  d  f  .  h                            **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     Header file for NetCDF interface for input/output                 **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-09-29 16:02:37 +0200 (lun. 29 sept. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef CDF_H
#define CDF_H

#define MISSING_VALUE_SHORT -9999
#define MISSING_VALUE_INT -999999
#define MISSING_VALUE_BYTE 99
#define NO_TIME -1
#define LON_NAME "longitude"
#define LAT_NAME "latitude"
#define TIME_NAME "time"
#define LON_DIM_NAME "longitude"
#define LAT_DIM_NAME "latitude"
#define TIME_DIM_NAME "time"

typedef enum { ONEFILE,CREATE,APPEND,CLOSE} State_nc;

typedef struct
{
  Real lon_min;
  Real lat_min;
  int nlon;
  int nlat;
  int *index;
} Coord_array;

typedef struct cdf 
{
  State_nc state;
  struct cdf *root;
  int ncid;
  int varid;
  int time_dim_id,lon_dim_id,lat_dim_id;
  int time_var_id,lon_var_id,lat_var_id;
  int n;
  const Coord_array *index;
  float missing_value;
} Netcdf;

typedef struct
{
  int firstyear;    /* first year of climate data (AD) */
  int n;            /* number of grid cell data to be read */
  long long offset; /* file offset in bytes */
  long long size;   /* size of dataset for each year in bytes */
  int nyear;        /* number of years of climate data */
  Bool isdaily;     /* daily values (TRUE/FALSE) */
  Bool ready;       /* data was already averaged */
  Bool swap;        /* byte order has to be changed (TRUE/FALSE) */
  FILE *file;       /* file pointer */
  int fmt;          /* file format (RAW/CLM/CDF) */
  int version;      /* file version number */
  float cellsize;   /* cell size (degree) */
  float scalar;     /* conversion factor */
  char *filename;
  const char *var;  /* variable name */
  const char *units;/* variable units or NULL */
  Bool oneyear;     /* one file for each year (TRUE/FALSE) */
  size_t var_len;
  int ncid;
#if defined(USE_NETCDF) || defined(USE_NETCDF4)
  int varid;
  Type type;
  Bool isleap;      /* leap days in file (TRUE/FALSE) */
  int nlon,nlat;    /* dimensions of longitude/latitude */
  float lon_min;
  float lat_min;
  union
  {
    short s;
    int i;
    float f;
    double d;
  } missing_value;  /* missing/fill value in file */
  double slope,intercept; /* conversion values derived from udunits */
#endif
} Climatefile;

typedef struct coord_netcdf *Coord_netcdf;

typedef struct input_netcdf *Input_netcdf;

typedef union
{
  FILE *file;
  Input_netcdf cdf;
} Infile;

extern Bool create1_netcdf(Netcdf *,const char *,const char *,
                          const char *,const char *,Type,int,
                          const Coord_array *,int,const Config *);
extern Bool create_netcdf(Netcdf *,const char *,const char *,
                          const char *,const char *,Type,int,
                          const Coord_array *,const Config *);
extern Bool openclimate_netcdf(Climatefile *,const char *,const char *,
                               const char *,const Config *);
extern Bool mpi_openclimate_netcdf(Climatefile *,const char *,const char *,
                                   const char *,const Config *);
extern Bool create_pft_netcdf(Netcdf *,const char *,int,int,int,const char *,
                              const char *,const char *,Type,
                              const Coord_array *,const Config *);
extern Bool create1_pft_netcdf(Netcdf *,const char *,int,int,int,const char *,
                              const char *,const char *,Type,
                              const Coord_array *,const Config *);
extern Bool close_netcdf(Netcdf *);
extern Bool readclimate_netcdf(Climatefile *,Real *,const Cell *,int,
                               const Config *);
extern int checkvalidclimate_netcdf(Climatefile *,Cell *,int,const Config *);
extern Bool readintclimate_netcdf(Climatefile *,int *,const Cell *,int,
                                  const Config *);
extern void closeclimate_netcdf(Climatefile *,Bool);

extern Bool write_float_netcdf(const Netcdf *,const float[],int,int);
extern Bool write_int_netcdf(const Netcdf *,const int[],int,int);
extern Bool write_short_netcdf(const Netcdf *,const short[],int,int);
extern Bool write_pft_float_netcdf(const Netcdf *,const float[],int,int,int);
extern Bool write_pft_short_netcdf(const Netcdf *,const short[],int,int,int);
extern void freecoordarray(Coord_array *);
extern Bool openfile_netcdf(Climatefile *,const char *,
                     const char *,const char *,const Config *);
extern Bool opendata_netcdf(Climatefile *,const char *,
                     const char *,const char *,const Config *);
extern Bool readdata_netcdf(const Climatefile *,Real *,const Cell *,
                            int,const Config *);
extern Bool readintdata_netcdf(const Climatefile *,int *,const Cell *,
                               int,const Config *);
extern Bool readshortdata_netcdf(const Climatefile *,short *,const Cell *,
                                 int,const Config *);
extern Coord_netcdf opencoord_netcdf(const char *,const char *,Bool);
extern void closecoord_netcdf(Coord_netcdf);
extern Bool seekcoord_netcdf(Coord_netcdf,int);
extern Bool readcoord_netcdf(Coord_netcdf,Coord *,Coord,int *);
extern int numcoord_netcdf(const Coord_netcdf);
extern int *getindexcoord_netcdf(const Coord_netcdf);
extern void getresolution_netcdf(const Coord_netcdf,Coord *);
extern void getextension_netcdf(Extension *,const Coord_netcdf);
extern Input_netcdf openinput_netcdf(const char *,const char *,const char *,
                                     int,const Config *);
extern void closeinput_netcdf(Input_netcdf);
extern size_t getindexinput_netcdf(const Input_netcdf,
                                   const Coord *,const Coord *);
extern size_t getindexsize_netcdf(const Input_netcdf);
extern Bool readinput_netcdf(const Input_netcdf,Real *,const Coord *,
                             const Coord *);
extern Bool readintinput_netcdf(const Input_netcdf,int *,const Coord *,
                                const Coord *);
extern Bool readshortinput_netcdf(const Input_netcdf,short *,const Coord *,
                                  const Coord *);
extern Bool setvarinput_netcdf(Input_netcdf,const char *,const char *,
                               const char *,int,const Config *);
extern Input_netcdf dupinput_netcdf(const Input_netcdf);
extern Type getinputtype_netcdf(const Input_netcdf);
extern Bool getlatlon_netcdf(Climatefile *,const char *,const Config *);
extern Bool getvar_netcdf(Climatefile *,const char *,const char *,
                          const char *,const Config *);
extern void closeinput(Infile,int);
extern int open_netcdf(const char *,int *,Bool *);
extern void free_netcdf(int);

#ifdef USE_MPI
extern Bool mpi_write_netcdf(const Netcdf *,void *,MPI_Datatype,int,int,
                             int [],int [],int,MPI_Comm);
extern Bool mpi_write_pft_netcdf(const Netcdf *,void *,MPI_Datatype,int,
                                 int,int,int [],int [],int,MPI_Comm);
#endif

#endif
