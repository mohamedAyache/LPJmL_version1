/***************************************************************************/
/**                                                                       **/
/**             i  n  i  t  _  a  n  n  u  a  l  .  c                     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void init_annual(Cell *cell,   /* Pointer to cell */
                 int npft,     /* number of natural PFTs */
                 int nbiomass, /* number of biomass PFTs */
                 int nagtree,  /* number of agriculture tree PFTs */
                 int ncft      /* number of crop PFTs */
                )
{
  int s,p;
  Pft *pft;
  Stand *stand;
  init_climbuf(&cell->climbuf);
  cell->balance.aprec=cell->balance.nep=cell->balance.awater_flux=0.0;
  cell->afire_frac=cell->balance.biomass_yield=0.0;
  cell->balance.total_irrig_from_reservoir=cell->balance.total_reservoir_out=0.0;
  foreachstand(stand,s,cell->standlist)
  {
#ifdef DEBUG3
    printf("init npft=%d\n",stand->pftlist.n);
#endif
    //printf("stand(%s)=%g %d\n",stand->type->name,stand->frac,stand->pft_id);
    stand->fire_sum=0;
    stand->soil.maxthaw_depth=0;
    foreachpft(pft,p,&stand->pftlist)
    {
      init(pft);
    }
  } /* of foreachstand */
  initoutput_annual(&cell->output,npft,nbiomass,nagtree,ncft);
#ifdef IMAGE
  cell->ml.image_data->anpp=cell->ml.image_data->arh=
    cell->ml.image_data->prod_turn_fast=
    cell->ml.image_data->prod_turn_slow=0.0;
#endif
} /* of 'init_annual' */
