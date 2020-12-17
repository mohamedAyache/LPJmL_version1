/***************************************************************************/
/**                                                                       **/
/**                    f  r  e  e  c  e  l  l  .  c                       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-24 11:22:32 +0200 (ven., 24 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void freecell(Cell *cell,int npft,Bool river_routing)
{
  freemanage(&cell->ml.manage,npft);
  if(river_routing)
    freequeue(cell->discharge.queue);
  if(!cell->skip)
  {
    while(!isempty(cell->standlist))
      delstand(cell->standlist,0);

    freelist(cell->standlist);
    freeclimbuf(&cell->climbuf);
    freecropdates(cell->ml.cropdates);
    freeoutput(&cell->output);
    free(cell->gdd);
    free(cell->ml.sowing_month);
    free(cell->ml.gs);
  }
} /* of 'freecell' */
