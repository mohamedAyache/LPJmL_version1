/***************************************************************************/
/**                                                                       **/
/**                 c  l  i  m  a  t  e  .  h                             **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     Header for the climate datatype and related functions             **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  12.12.2012                                          **/
/**                                                                       **/
/***************************************************************************/

#ifndef CLIMATE_H /* Already included? */
#define CLIMATE_H

#define COLDEST_DAY_NHEMISPHERE 14
#define COLDEST_DAY_SHEMISPHERE 195


/* Definitions of datatypes */

typedef struct
{
  Real *temp; /* temperature data (deg C) */
  Real *prec; /* precipitation data (mm) */
  Real *sun;  /* sunshine data (%) */
  Real *wet;  /* number of wet days in a month */
  Real *wind; /* wind speed (m/min) */
  Real *tamp; /* temperature amplitude */
  Real *tmax; /* maximum temperature (deg C) */
  Real *lightning;
  Real *lwnet;   /* long wave net downward flux (W m-2) */
  Real *swdown;  /* short wave downward flux component (W m-2) */
} Climatedata;

typedef struct
{
  Real temp;       /* temperature (deg C) */
  Real prec;       /* precipitation (mm) */
  Real sun;        /* sunshine (%) */
  Real windspeed;  /* windspeed (m/min) */
  Real tmin;       /* minimum temperature (deg C) */
  Real tmax;       /* maximum temperature (deg C) */
  Real lightning;  /* daily lightning ignition  */
  Real lwnet;      /* long wave net downward flux (W/m2) */
  Real swdown;     /* short wave downward flux component (W/m2) */
} Dailyclimate;

typedef struct
{
  int firstyear;    /* first year of CO2 data (AD) */
  int nyear;        /* number of years of climate data */
  Real *data;       /* atmospheric CO2 (ppmv) */
} Co2data;

typedef struct
{
  int firstyear;    /* first year of data available for all variables (AD) */
  Co2data co2;      /* CO2 data */
  Climatefile file_temp,file_prec,file_wet; /* file pointers */
  Climatefile file_cloud,file_lwnet,file_swdown;
  Climatefile file_wind,file_tamp,file_tmax,file_lightning;
#ifdef IMAGE
  Climatefile file_temp_var,file_prec_var;
#endif
  Climatedata data; /* climate data arrays */
} Climate;

/* Definitions of macros */

#define getcelltemp(climate,cell) climate->data.temp+(cell)*NMONTH
#define getcellprec(climate,cell) climate->data.prec+(cell)*NMONTH
#define getcellsun(climate,cell) climate->data.sun+(cell)*NMONTH
#define getcelllwnet(climate,cell) climate->data.lwnet+(cell)*NMONTH
#define getcellswdown(climate,cell) climate->data.swdown+(cell)*NMONTH
#define getcellwet(climate,cell) climate->data.wet+(cell)*NMONTH
#define getcellwind(climate,cell) climate->data.wind+(cell)*NMONTH
#define getcelltamp(climate,cell) climate->data.tamp+(cell)*NMONTH
#define getcelltmax(climate,cell) climate->data.tmax+(cell)*NMONTH
#define getcelllightning(climate,cell) climate->data.lightning+(cell)*NMONTH
#define getprec(cell,d) (cell).climbuf.dval_prec[(d)+1]
#define israndomprec(climate) ((climate)->data.wet!=NULL)

/* Declaration of functions */

extern Climate *initclimate(const Cell *,const Config *);
extern Bool getclimate(Climate *,const Cell *,int,const Config *);
extern Real getco2(const Climate *,int);
extern void freeclimate(Climate *,Bool);
extern Bool storeclimate(Climatedata *,Climate *,const Cell *,int,int,
                         const Config *);
extern void freeclimatedata(Climatedata *);
extern void restoreclimate(Climate *,const Climatedata *,int);
extern void moveclimate(Climate *,Climatedata,int);
extern void prdaily(Real [],int,Real,Real);
extern void dailyclimate(Dailyclimate *,const Climate *,Climbuf *,
                         int,int,int,int);
extern Real getmtemp(const Climate *,const Climbuf *,int,int);
extern Real getmprec(const Climate *,const Climbuf *,int,int);
extern void initclimate_monthly(const Climate *,Climbuf *,int,int);
extern Bool openclimate(Climatefile *,const Filename *,const char *,size_t,
                        const Config *);
extern Real avgtemp(const Climate *,int cell);
extern Real avgprec(const Climate *,int cell);
extern void closeclimatefile(Climatefile *,Bool);
extern Bool readclimate(Climatefile *,Real *,Real,Real,const Cell *,int,
                        const Config *);
extern Bool checkvalidclimate(Climate *,Cell *,Config *);
extern Bool readco2(Co2data *,const Filename *,Bool);

/* barrier.n */
extern Bool readclimatefilename(FILE *, Filename *,const char *, Bool, Bool);

#endif
