/***************************************************************************/
/**                                                                       **/
/**            f  r  e  e  o  u  t  p  u  t  v  a  r  .  c                **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 29.01.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void freeoutputvar(Variable *var,int nout_max)
{
  int i;
  if(var!=NULL)
  {
    for(i=0;i<nout_max;i++)
    {
      free(var[i].name);
      free(var[i].var);
      free(var[i].descr);
      free(var[i].unit);
    }
    free(var);
  }
} /* of 'freeoutputvar' */
