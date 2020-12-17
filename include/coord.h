/***************************************************************************/
/**                                                                       **/
/**                    c  o  o  r  d  .  h                                **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     Definition of coordinate datatype                                 **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-05-30 17:09:50 +0200 (ven. 30 mai 201#$ **/
/**     By         : $Author:: rolinski                        $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef COORD_H /* Already included? */
#define COORD_H

/* Definition of datatypes */

typedef struct
{
  short lon,lat; /* longitude, latitude in degrees * 100 */
} Intcoord;

typedef struct
{
  Real lon,lat;  /* longitude, latitude in degrees */
  Real area;     /* cell area (m^2) */
} Coord;

typedef struct coordfile *Coordfile;

/* Declaration of functions */

extern Coordfile opencoord(const Filename *);
extern Bool readintcoord(FILE *,Intcoord *,Bool);
extern int seekcoord(Coordfile,int);
extern Bool readcoord(Coordfile,Coord *,Coord);
extern void closecoord(Coordfile);
extern Bool writecoord(FILE *,const Coord);
extern Real cellarea(Coord,Coord);
extern Bool fscancoord(FILE *,Coord *,Bool);
extern double distHaversine(double, double, double, double);
extern int numcoord(const Coordfile);
extern Real getcellsizecoord(const Coordfile);
extern void fprintcoord(FILE *,const Coord);
extern int findcoord(Coord,const Coord [],int);

/* Definition of macros */

#define printcoord(coord) fprintcoord(stdout,coord)

#endif
