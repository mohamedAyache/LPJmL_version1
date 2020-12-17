/***************************************************************************/
/**                                                                       **/
/**        t u r n o v e r _ m o n t h l y _ g r a s s . c                **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 07.03.2012                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"

void turnover_monthly_grass(Litter *litter,Pft *pft)
{
  Pftgrass *grass;
  Pftgrasspar *grasspar;
  if(pft->stand->type->landusetype==NATURAL)
  {
    grasspar=pft->par->data;
    grass=pft->data;
    grass->turn.root+=grass->ind.root*grasspar->turnover.root/NMONTH;
    litter->bg[pft->litter]+=grass->ind.root*grasspar->turnover.root/NMONTH*pft->nind;
  }
} /* of 'turnover_monthly_grass' */
