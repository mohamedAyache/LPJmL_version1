/***************************************************************************/
/**                                                                       **/
/**                       n  e  w  _  g  r  a  s  s  .  c                 **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function allocates and initializes grass-specific variables       **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-06-19 16:27:28 +0200 (ven. 19 juin 20#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"

void new_grass(Pft *pft /* pointer to PFT variables */
              )        
{
  Pftgrass *grass;
  grass=new(Pftgrass);
  check(grass);
  pft->data=grass;
  pft->bm_inc=pft->wscal_mean=pft->phen=0;
  pft->stand->growing_days=0;
#ifdef NEW_GRASS
  grass->max_leaf=0;
#endif
  pft->nind=1;
  pft->flaimax=0;
  grass->ind.leaf=grass->ind.root=grass->turn.leaf=grass->turn.root=0;
} /* of 'new_grass' */
