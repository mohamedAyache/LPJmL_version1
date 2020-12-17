/***************************************************************************/
/**                                                                       **/
/**                        o  u  t  f  i  l  e  .  h                      **/
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
/**     Last change: $Date:: 2014-09-29 16:02:37 +0200 (lun. 29 sept. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef OUTFILE_H
#define OUTFILE_H

/* Definition of datatypes */

typedef struct
{
  Bool isopen;       /* file is open for output (TRUE/FALSE) */
  int fmt;           /* file format (TXT/RAW/CDF) */
  Bool oneyear;      /* separate output files for each year (TRUE/FALSE) */
  Bool compress;     /* compress file after write (TRUE/FALSE) */
  const char *filename;
  union
  { 
#ifdef USE_MPI
    MPI_File mpi_file; /* MPI-2 file descriptor */
#endif
    FILE *file;        /* file pointer */
    Netcdf cdf;
  } fp;
} File;

typedef struct
{
#ifdef USE_MPI
  int *counts;         /* sizes for MPI_Gatherv */
  int *offsets;        /* offsets for MPI_Gatherv */
#endif
  Outputmethod method;
  Socket *socket;
  File *files;
  int n;          /* size of File array */
  Coord_array *index;
  Coord_array *index_all;
  Bool withdaily; /* with daily output (TRUE/FALSE) */
} Outputfile;

extern Outputmonth *newoutputmonthly(Outputfile *,int);
extern Outputday *newoutputdaily(Outputfile *,int);
extern int findfile(const Outputvar *,int,int);
extern Outputfile *fopenoutput(const Cell *,int,const Config *);
extern void openoutput_yearly(Outputfile *,int,const Config *);
extern void closeoutput_yearly(Outputfile *,const Config *);
extern void fcloseoutput(Outputfile *,const Config *);
extern void fwriteoutput_monthly2(Outputfile *,const Outputmonth *,int,
                                  const Config *);
extern void fwriteoutput_daily2(Outputfile *,const Outputday *,int,const Config *);
extern Coord_array *createcoord(Outputfile *,const Cell *,const Config *);
extern Coord_array *createcoord_all(const Cell *,const Config *);
extern Coord_array *createindex(const Coord *,int,Coord);
extern void outputnames(Outputfile *,const Config *);
#endif
