/***************************************************************************/
/**                                                                       **/
/**         h  a  r  v  e  s  t  _  s  t  a  n  d  .  c                   **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     Function harvests grassland stand                                 **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-03-25 09:51:44 +0100 (mer., 25 mars 2#$ **/
/**     By         : $Author:: heinke                          $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"
#include "agriculture.h"
#ifdef NEW_GRASS
#include "landuse.h"
#include "grassland.h"
#endif

Harvest harvest_grass(Stand *stand, Real hfrac)
{
  Harvest harvest;
  Harvest sum={0,0,0,0};
  Pftgrass *grass;
  Pft *pft;
  int p;
  
  foreachpft(pft,p,&stand->pftlist)
  {
    grass=pft->data;
    harvest.harvest=grass->ind.leaf*hfrac;
    grass->ind.leaf*=(1-hfrac);
    sum.harvest+=harvest.harvest;
#ifdef NEW_GRASS
    grass->max_leaf = grass->ind.leaf;
#else
    /*sum.residual+=harvest.residual;
    sum.residuals_burnt+=harvest.residuals_burnt;
    sum.residuals_burntinfield+=harvest.residuals_burntinfield; */
    /*pft->gdd=10;
    pft->phen=0.1;*/
    pft->gdd=pft->phen=0.0;
#endif
  }
  return sum;
} /* of 'harvest_grass' */

#ifdef NEW_GRASS
static Harvest harvest_grass_mowing(Stand *stand)
{
  Harvest harvest;
  Harvest sum={0,0,0,0};
  Pftgrass *grass;
  Pft *pft;
  int p;

  foreachpft(pft,p,&stand->pftlist)
  {
    grass=pft->data;
    harvest.harvest=grass->ind.leaf - STUBBLE_HEIGHT_MOWING;
    grass->ind.leaf = STUBBLE_HEIGHT_MOWING;
    sum.harvest+=harvest.harvest;
    pft->gdd=pft->phen=0.0; // change -> relative from ind.leaf
  }
  return sum;
} /* of 'harvest_grass_mowing' */

/*
 * called in function phenology_grassland() when managed grassland
 * is harvested
 *
 */
static Harvest harvest_grass_grazing_ext(Stand *stand)
{
  Harvest sum={0,0,0,0};
  Pftgrass *grass;
  Pft *pft;
  int p;
  Real bm_grazed;
  Real fact;
  Real bm_tot = 0.0;
  Real bm_grazed_pft;

  foreachpft(pft,p,&stand->pftlist)
  {
    grass=pft->data;
    bm_tot += grass->ind.leaf;
  }
//  bm_grazed = stand->cell->ml.nr_of_lsus_ext * DEMAND_COW_EXT; 
  bm_grazed = 1e-4* stand->cell->ml.nr_of_lsus_ext * DEMAND_COW_EXT; 

  foreachpft(pft,p,&stand->pftlist)
  {
    grass=pft->data; 
    if (bm_tot < 1e-5) // to avoid division by zero!
      fact = 1;
    else
      fact = grass->ind.leaf / bm_tot;

    bm_grazed_pft = bm_grazed * fact;
    if (grass->ind.leaf - bm_grazed_pft < GRAZING_STUBLE)
      bm_grazed_pft = grass->ind.leaf - GRAZING_STUBLE;
    if (bm_grazed_pft < 0)
      bm_grazed_pft = 0;

    pft->gdd = (1-(bm_grazed_pft/grass->ind.leaf)) * pft->gdd;

    grass->ind.leaf = grass->ind.leaf - bm_grazed_pft;

    sum.harvest += (1-MANURE)*bm_grazed_pft;                       // 60% atmosphere, 15% cows
    stand->soil.cpool->fast += MANURE * bm_grazed_pft;             // 25% back to soil 
    // pft->phen recalculated in phenology_grass
  }
  return sum;
} /* of 'harvest_grass_grazing_ext' */


static Harvest harvest_grass_grazing_int(Stand *stand)
{
  Harvest sum={0,0,0,0};
  Pftgrass *grass;
  Pft *pft;
  int p;
  Real rotation_len;
  Real fact;
  Real bm_grazed;
  Real bm_tot = 0;
  Real bm_grazed_pft;
  Rotation *rotation;

  rotation = &(stand->cell->ml.rotation);
  foreachpft(pft,p,&stand->pftlist)
  {
    grass=pft->data;
    bm_tot += grass->ind.leaf;
  }
  
  if (rotation->rotation_mode == RM_UNDEFINED) //initial calculate grazing days and recovery days
  {
    rotation_len = (bm_tot - GRAZING_STUBLE) / (1e4*stand->cell->ml.nr_of_lsus_int * DEMAND_COW_INT) ;
    if (rotation_len > MAX_ROTATION_LENGTH)
      rotation_len = MAX_ROTATION_LENGTH;

    if (rotation_len > MIN_ROTATION_LENGTH) // otherwise wait for more growth 
    {
      rotation->grazing_days = (int)ceil(rotation_len/MAX_PADDOCKS);
      rotation->paddocks = (int)floor((rotation_len/rotation->grazing_days) + 0.5);
      rotation->recovery_days = (rotation->paddocks-1) * rotation->grazing_days;
      rotation->rotation_mode = RM_GRAZING;
    }
  }

  if (rotation->rotation_mode == RM_GRAZING)
  {
    bm_grazed = stand->cell->ml.nr_of_lsus_int * DEMAND_COW_INT * rotation->paddocks;
    foreachpft(pft,p,&stand->pftlist)
    {
      grass=pft->data;
      fact = grass->ind.leaf / bm_tot;
      bm_grazed_pft = bm_grazed * fact;

      if (grass->ind.leaf - bm_grazed_pft < GRAZING_STUBLE)
        bm_grazed_pft = grass->ind.leaf - GRAZING_STUBLE;

      if (bm_grazed_pft < 0)
        bm_grazed_pft =0;

      pft->gdd = (1-(bm_grazed_pft/grass->ind.leaf)) * pft->gdd;

      grass->ind.leaf = grass->ind.leaf - bm_grazed_pft;
      sum.harvest += (1-MANURE)*bm_grazed_pft;              // 60% atmosphere, 15% cows
      stand->soil.cpool->fast += MANURE * bm_grazed_pft;    // 25% back to soil
    }

    rotation->grazing_days -= 1;
    if (rotation->grazing_days == 0)
      rotation->rotation_mode = RM_RECOVERY;
  }
  else if (rotation->rotation_mode == RM_RECOVERY)
  {
    rotation->recovery_days -= 1;
    if (rotation->recovery_days == 0)
      rotation->rotation_mode = RM_UNDEFINED;
  }
  return sum;
} /* of 'harvest_grass_grazing_int' */
#endif

Harvest harvest_stand(Output *output, /* Output data */
                      Stand *stand,   /* pointer to grassland stand */
                      int ntypes,     /* number of different PFT classes */
                      Real hfrac      /* harvest fraction */
                     )
{
  Harvest harvest;
  int p;
  Pft *pft;

#ifdef NEW_GRASS
  if (stand->type->landusetype == GRASSLAND) 
  {
    switch (stand->cell->ml.grass_scenario)
    {
      case GS_DEFAULT: // default
        harvest=harvest_grass(stand,hfrac);
        break;
      case GS_MOWING: // mowing
        harvest=harvest_grass_mowing(stand);
        break;
      case GS_GRAZING_EXT: // ext. grazing
        harvest=harvest_grass_grazing_ext(stand);
        break;
      case GS_GRAZING_INT: // int. grazing
        harvest=harvest_grass_grazing_int(stand);
        break;
    }
  }
  else /* option for biomass_grass */
  {
    harvest=harvest_grass(stand,hfrac);
  }
#else
  allocation_today(stand,ntypes);
  harvest=harvest_grass(stand,hfrac);
  foreachpft(pft,p,&stand->pftlist)
  {
    turnover_grass(&stand->soil.litter,pft,(Real)stand->growing_days/NDAYYEAR);
    fpc_grass(pft);
  }
  stand->growing_days=0;
#endif
  output->flux_harvest+=(harvest.harvest+harvest.residual)*stand->frac;
  output->dcflux+=(harvest.harvest+harvest.residual)*stand->frac;
  return harvest;

} /* of 'harvest_stand' */
