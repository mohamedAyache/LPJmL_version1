/***************************************************************************/
/**                                                                       **/
/**               f  r  e  a  d  _  g  r  a  s  s  .  c                   **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 14.10.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"

Bool fread_grass(FILE *file,Pft *pft,Bool swap)
{
  Pftgrass *grass;
  grass=new(Pftgrass);
  pft->data=grass;
  if(grass==NULL)
    return TRUE;
  freadreal((Real *)&grass->turn,sizeof(Grassphys)/sizeof(Real),swap,file);
#ifdef NEW_GRASS
  freadreal((Real *)&grass->ind,sizeof(Grassphys)/sizeof(Real),swap,file)!=sizeof(Grassphys)/sizeof(Real);
  return freadreal((Real *)&grass->max_leaf,1,swap,file)!=1;
#else
  return freadreal((Real *)&grass->ind,sizeof(Grassphys)/sizeof(Real),swap,file)!=sizeof(Grassphys)/sizeof(Real);
#endif
  /*fpc_grass(pft); */
} /* of 'fread_grass' */
