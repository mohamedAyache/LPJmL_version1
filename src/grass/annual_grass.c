/***************************************************************************/
/**                                                                       **/
/**            a  n  n  u  a  l  _  g  r  a  s  s  .  c                   **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-15 12:26:32 +0200 (mer., 15 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"

Bool annual_grass(Stand *stand, /* pointer to stand */
                  Pft *pft,     /* pointer to PFT variables */
                  Real *fpc_inc /* FPC increment */
                 )              /* returns TRUE on death */
{
  Bool isdead;

#ifdef NEW_GRASS 
  if(stand->type->landusetype!=GRASSLAND && stand->type->landusetype!=BIOMASS_GRASS) 
    turnover_grass(&stand->soil.litter,pft,(Real)stand->growing_days/NDAYYEAR);
#else
  turnover_grass(&stand->soil.litter,pft,(Real)stand->growing_days/NDAYYEAR);
#endif
  isdead=allocation_grass(&stand->soil.litter,pft,fpc_inc);
  stand->growing_days=0;
  if(!isdead)
    isdead=!survive(pft->par,&stand->cell->climbuf);
  return isdead;
} /* of 'annual_grass' */
