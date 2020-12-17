/***************************************************************************/
/**                                                                       **/
/**                   i  n  p  u  t  .  h                                 **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 14.02.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#ifndef INPUT_H
#define INPUT_H

/* Definition of datatypes */

typedef struct
{
  Climate *climate;
  Landuse landuse;
  Wateruse wateruse;
  Popdens popdens;
} Input;

/* Declaration of functions */

extern Bool initinput(Input *,const Cell *,int,const Config *);
extern void freeinput(Input,Bool);

#endif
