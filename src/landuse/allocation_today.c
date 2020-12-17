/***************************************************************************/
/**                                                                       **/
/**        a  l  l  o  c  a  t  i  o  n  _  t  o  d  a  y  .  c           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 25.01.2006  by S.Jachner, M.Gumpenberger             **/
/**     Last change: $Date:: 2015-07-15 12:26:32 +0200 (mer., 15 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"
#include "landuse.h"

void allocation_today(Stand *setasidestand, /* pointer to setaside stand */
                      int ntypes /* number of different PFT classes */
                     )
{
  int p,npft;
  Real *fpc_inc;
  Pft *pft;
  npft=getnpft(&setasidestand->pftlist); /* get number of established PFTs */
  if(npft>0) /* nonzero? */
  {
    fpc_inc=newvec(Real,npft);
    check(fpc_inc);
    foreachpft(pft,p,&setasidestand->pftlist) 
    {
      /* only grass PFTs are established on setaside stand */
      if(allocation_grass(&setasidestand->soil.litter,pft,fpc_inc+p))
      {
        /* kill PFT from list of established PFTs */
        fpc_inc[p]=fpc_inc[getnpft(&setasidestand->pftlist)-1]; /*moved here by W. von Bloh */
        litter_update_grass(&setasidestand->soil.litter,pft,pft->nind);
        delpft(&setasidestand->pftlist,p);
        p--; /* adjust loop variable */ 
      }       
      else 
        pft->bm_inc=0;
    } /* of foreachpft */
    light(setasidestand,ntypes,fpc_inc);
    free(fpc_inc);
  }
} /* of 'allocation_today' */

/*
- called in functions sowing.c and update_daily.c
- calls specific function allocation() for each pft
- sets bm_inc for each pft to 0
- deletes pft if it could not survive
- calls specific function light()
*/
