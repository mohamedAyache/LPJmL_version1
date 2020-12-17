/***************************************************************************/
/**                                                                       **/
/**               a  n  n  u  a  l  _  s  e  t  a  s  i  d  e  .  c       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function performs necessary updates after iteration over one      **/
/**     year for setaside stand                                           **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     See file AUTHORS for list of authors contributing to this code    **/
/**                                                                       **/
/**     Last change: $Date:: 2016-02-11 11:10:20 +0100 (jeu., 11 févr.#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"
#include "agriculture.h"

Bool annual_setaside(Stand *stand,         /* Pointer to stand */
                     int npft,             /* number of natural PFTs */
                     int UNUSED(ncft),     /* number of crop PFTs */
                     Real UNUSED(popdens), /* population density (capita/km2) */
                     int UNUSED(year),
                     Bool intercrop,
                     const Config *config
                    )
{
  int p;
  Pft *pft;
  Real fpc_inc,excess;
  Real fpc_total,*fpc_type;
#ifndef DAILY_ESTABLISHMENT
  Bool *present;
  Real acflux_estab;
  int n_est=0;
  present=newvec(Bool,npft);
  check(present);
  acflux_estab=0;
  for(p=0;p<npft;p++)
    present[p]=FALSE;
#endif
  fpc_type=newvec(Real,config->ntypes);
  check(fpc_type);

  foreachpft(pft,p,&stand->pftlist)
  {
#ifdef DEBUG2
    printf("PFT:%s fpc=%g\n",pft->par->name,pft->fpc);
    printf("PFT:%s bm_inc=%g vegc=%g soil=%g\n",pft->par->name,
           pft->bm_inc,vegc_sum(pft),soilcarbon(&stand->soil));
#endif

#ifndef DAILY_ESTABLISHMENT
    present[pft->par->id]=TRUE;
#endif
    if(annual_grass(stand,pft,&fpc_inc))
    {
      /* PFT killed, delete from list of established PFTs */
      litter_update_grass(&stand->soil.litter,pft,pft->nind);
      delpft(&stand->pftlist,p);
      p--; /* adjust loop variable */ 
    }
        
  } /* of foreachpft */
#ifdef DEBUG2
  printf("Number of updated pft: %d\n",stand->pftlist.n);
#endif
  fpc_sum(fpc_type,config->ntypes,&stand->pftlist);
  if(fpc_type[GRASS]>1.0)
    foreachpft(pft,p,&stand->pftlist)
    {
      excess=(fpc_type[GRASS]-1.0)*(pft->fpc/fpc_type[GRASS]);
      light_grass(&stand->soil.litter,pft,excess);
    }

#ifndef DAILY_ESTABLISHMENT
  if(intercrop)
  {
    for(p=0;p<npft;p++)
    {
      if(config->pftpar[p].type==GRASS && config->pftpar[p].cultivation_type==NONE 
         && establish(stand->cell->gdd[p],config->pftpar+p,&stand->cell->climbuf))
      {
        if(!present[p])
         addpft(stand,config->pftpar+p);
        n_est++;
      }
    }
    fpc_total=fpc_sum(fpc_type,config->ntypes,&stand->pftlist);
    foreachpft(pft,p,&stand->pftlist)
     if(establish(stand->cell->gdd[pft->par->id],pft->par,&stand->cell->climbuf))
      acflux_estab+=establishment_grass(pft,fpc_total,fpc_type[pft->par->type],n_est);

    stand->cell->output.flux_estab+=acflux_estab*stand->frac;
    stand->cell->output.dcflux-=acflux_estab*stand->frac;
  }
#endif

#ifndef DAILY_ESTABLISHMENT
  free(present);
#endif
  free(fpc_type);
  return FALSE;
} /* of 'annual_setaside' */
