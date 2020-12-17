/***************************************************************************/
/**                                                                       **/
/**            a  n  n  u  a  l  _  n  a  t  u  r  a  l  .  c             **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function performs necessary updates after iteration over one      **/
/**     year for natural stand                                            **/
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
#include "natural.h"

Bool annual_natural(Stand *stand,         /* Pointer to stand */
                    int npft,             /* number of natural pfts */
                    int UNUSED(ncft),     /* number of crop PFTs */
                    Real popdens,         /* population density (capita/km2) */
                    int year,             /* year (AD) */
                    Bool UNUSED(intercrop),
                    const Config *config
                   )                      /* stand has to be killed (TRUE/FALSE) */
{
  int p,pft_len;
  Pft *pft;
  Real *fpc_inc;
  Real fire_frac;
#ifndef DAILY_ESTABLISHMENT
  Real acflux_estab;
#endif
  Real firewood=0;

  pft_len=getnpft(&stand->pftlist); /* get number of established PFTs */
  if(pft_len>0)
  {
    fpc_inc=newvec(Real,pft_len);
    check(fpc_inc);
    
    foreachpft(pft,p,&stand->pftlist)
    {
#ifdef DEBUG2
      printf("PFT:%s fpc_inc=%g fpc=%g\n",pft->par->name,fpc_inc[p],pft->fpc);
      printf("PFT:%s bm_inc=%g vegc=%g soil=%g\n",pft->par->name,
             pft->bm_inc,vegc_sum(pft),soilcarbon(&stand->soil));
#endif
      //printf("barrier: PFT:%s fpc_inc=%g fpc=%g\n",pft->par->name,fpc_inc[p],pft->fpc);
      if(annualpft(stand,pft,fpc_inc+p))
      {
        /* PFT killed, delete from list of established PFTs */
        fpc_inc[p]=fpc_inc[getnpft(&stand->pftlist)-1];
        litter_update(&stand->soil.litter,pft,pft->nind);
        delpft(&stand->pftlist,p);
        p--; /* adjust loop variable */ 
      }      
    } /* of foreachpft */
#ifdef DEBUG2
    printf("Number of updated pft: %d\n",stand->pftlist.n);
#endif
    if(year>=config->firstyear && config->firewood==FIREWOOD)
    {
      firewood=woodconsum(stand,popdens);
      stand->cell->output.flux_firewood+=firewood*stand->frac;
      foreachpft(pft,p,&stand->pftlist)
        if(pft->nind<epsilon)
        {
          fpc_inc[p]=fpc_inc[getnpft(&stand->pftlist)-1];
          litter_update(&stand->soil.litter,pft,pft->nind);
          delpft(&stand->pftlist,p);
          p--;  
        }
    }
    light(stand,config->ntypes,fpc_inc);
    free(fpc_inc);
  }
  if(config->fire==FIRE)
  {  
    fire_frac=fire_prob(&stand->soil.litter,stand->fire_sum);
    stand->cell->output.firef+=1.0/fire_frac;
    stand->cell->output.firec+=firepft(&stand->soil.litter,
                                       &stand->pftlist,fire_frac)*stand->frac;
  }
#ifndef DAILY_ESTABLISHMENT
  acflux_estab=establishmentpft(stand,config->pftpar,npft,config->ntypes,stand->cell->balance.aprec,year);
  stand->cell->output.flux_estab+=acflux_estab*stand->frac;
  stand->cell->output.dcflux+=acflux_estab*stand->frac;
#endif
  foreachpft(pft,p,&stand->pftlist)
  {
    stand->cell->output.fpc[getpftpar(pft,id)+1]=pft->fpc;
  }
  return FALSE;
} /* of 'annual_natural' */
