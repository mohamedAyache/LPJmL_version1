/***************************************************************************/
/**                                                                       **/
/**                     h  e  a  d  e  r  .  h                            **/
/**                                                                       **/
/**     Reading/Writing file header for LPJ related files. Detects        **/
/**     whether byte order has to be changed                              **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-14 09:09:50 +0200 (mar. 14 juil. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef HEADER_H /* Already included? */
#define HEADER_H

/* Definition of constants */

#define RESTART_HEADER "LPJRESTART"
#define RESTART_VERSION 12
#define LPJ_CLIMATE_HEADER "LPJCLIM"
#define LPJ_CLIMATE_VERSION 2
#define LPJ_LANDUSE_HEADER "LPJLUSE"
#define LPJ_LANDUSE_VERSION 2
#define LPJ_SOWING_HEADER "LPJSOWD"
#define LPJ_SOWING_VERSION 2
#define LPJGRID_HEADER "LPJGRID"
#define LPJGRID_VERSION 2
#define LPJDRAIN_HEADER "LPJDRAI"
#define LPJDRAIN_VERSION 2
#define LPJ_COUNTRY_HEADER "LPJ_COW"
#define LPJ_COUNTRY_VERSION 2
#define LPJWATERUSE_HEADER "LPJWUSE"
#define LPJWATERUSE_VERSION 2
#define LPJNEIGHB_IRRIG_HEADER "LPJNIRR"
#define LPJNEIGHB_IRRIG_VERSION 2
#define LPJSOIL_HEADER "LPJSOIL"
#define LPJSOIL_VERSION 2
#define LPJRESERVOIR_HEADER "LPJDAMS"
#define LPJRESERVOIR_VERSION 2
#define LPJELEVATION_HEADER "LPJELEV"
#define LPJELEVATION_VERSION 2
#define LPJ_POPDENS_HEADER "LPJPOPD"
#define LPJ_POPDENS_VERSION 2
#define LPJ_PRODINI_HEADER "LPJPROD"
#define LPJ_PRODINI_VERSION 2
#define LPJ_LAKEFRAC_HEADER "LPJLAKE"
#define LPJ_LAKEFRAC_VERSION 2
#define CELLYEAR 1
#define YEARCELL 2
#define READ_VERSION -1

/* Definition of datatypes */

typedef struct
{
  int order;       /* order of data items , either CELLYEAR or YEARCELL */
  int firstyear;   /* first year for data */
  int nyear;       /* number of years */
  int firstcell;   /* index of first data item */
  int ncell;       /* number of data item per year */
  int nbands;      /* number of data elements per cell */
  float cellsize;  /* cellsize in deg */
  float scalar;    /* conversion factor*/
  /*  Type datatype; */
} Header;

typedef struct
{
  int order;       /* order of data items , either CELLYEAR or YEARCELL */
  int firstyear;   /* first year for data */
  int nyear;       /* number of years */
  int firstcell;   /* index of first data item */
  int ncell;       /* number of data item per year */
  int nbands;      /* number of data elements per cell */
} Header_old;

typedef struct
{
  Bool landuse;       /* land use enabled (TRUE/FALSE) */
  Bool river_routing; /* river routing enabled (TRUE/FALSE) */
  int sdate_option;   /* sowing date option (0-2)*/
  Type datatype;      /* float or double values */
} Restartheader;

/* Declaration of functions */

extern Bool fwriteheader(FILE *,Header, const char *,int);
extern Bool freadheader(FILE *,Header *,Bool *,const char *,int *);
extern Bool freadrestartheader(FILE *,Restartheader *,Bool);
extern Bool freadanyheader(FILE *,Header *,Bool *,String,int *);
extern size_t headersize(const char *,int);
extern FILE *openinputfile(Header *, Bool *,const Filename *,
                           String, int *,const Config *);
extern void fprintheader(FILE *,Header);

/* Definition of macros */

#define printheader(header) fprintheader(stdout,header)

#endif
