/***************************************************************************/
/**                                                                       **/
/**     l  i  t  t  e  r  _  u  p  d  a  t  e  _  g  r  a  s  s  .  c     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-10-28 10:14:19 +0100 (mer., 28 oct. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"

void litter_update_grass(Litter *litter, /* Litter pool */
                         Pft *pft,       /* PFT variables */
                         Real frac      /* fraction (0..1) */
                        )
{
  Pftgrass *grass;
  grass=pft->data;
  
  litter->ag[pft->litter].trait.leaf+=grass->ind.leaf*frac;
  // barrier.n
  grass->residue.leaf += grass->ind.leaf*frac;
  update_fbd_grass(litter,pft->par->fuelbulkdensity,
                   grass->ind.leaf*frac);
  litter->bg[pft->litter]+=grass->ind.root*frac;
  // barrier.n
  grass->residue.root += grass->ind.root*frac;


} /* of 'litter_update_grass' */
