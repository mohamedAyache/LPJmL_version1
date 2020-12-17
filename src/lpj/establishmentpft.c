/***************************************************************************/
/**                                                                       **/
/**      e  s  t  a  b  l  i  s  h  m  e  n  t  p  f  t  .  c             **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function calculates establishment in population                   **/
/**     Simulates population increase through establishment each          **/
/**     simulation year for trees and grasses and introduces new PFTs if  **/
/**     climate conditions become suitable.                               **/
/**     This function assumes each Individual object represents the       **/
/**     average individual of a PFT population, and that there is (at     **/
/**     most) one individual object per PFT per modelled area (stand).    **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 30.06.2010                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"
#include "grass.h"

Real establishmentpft(Stand *stand,        /* Stand pointer  */
                      const Pftpar *pftpar,/* PFT parameter array */
                      int npft,            /* number of PFTs */
                      int ntypes,          /* number of different PFT classes */
                      Real aprec,           /* annual precipitation (mm) */
                      int year
                     )  /* returns establishment flux (gC/m2) */
{
  Real acflux_est;
  Real fpc_total,*fpc_type;
  int *n_est; 
  Bool *present;
  int p,t;
  Pft *pft;
  present=newvec(Bool,npft);
  check(present);
  fpc_type=newvec(Real,ntypes);
  check(fpc_type);
  n_est=newvec(int,ntypes);
  check(n_est);
  for(p=0;p<npft;p++)
    present[p]=FALSE;
  foreachpft(pft,p,&stand->pftlist)
    present[pft->par->id]=TRUE;
  acflux_est=0.0;

#ifdef DEBUG2
  printf("Number of pfts: %d\n",stand->pftlist.n);
  for(p=0;p<npft;p++)
    printf("%s ",present[p] ? "true" : "false");
  printf("\n");
#endif
  for(t=0;t<ntypes;t++)
    n_est[t]=0;
  for(p=0;p<npft;p++)
  {
    if(aprec>=pftpar[p].aprec_min && pftpar[p].cultivation_type==NONE &&
       establish(stand->cell->gdd[p],pftpar+p,&stand->cell->climbuf))
    {
      if(!present[p])
      {
        addpft(stand,pftpar+p);
      }
      n_est[pftpar[p].type]++;
    }
  }

  fpc_total=fpc_sum(fpc_type,ntypes,&stand->pftlist);
  foreachpft(pft,p,&stand->pftlist)
    if(aprec>=pft->par->aprec_min &&
#ifdef DAILY_ESTABLISHMENT
       !pft->established &&
#endif
          establish(stand->cell->gdd[pft->par->id],pft->par,&stand->cell->climbuf))
    {
      acflux_est+=establishment(pft,fpc_total,fpc_type[pft->par->type],
                                n_est[pft->par->type]);
#ifdef DAILY_ESTABLISHMENT
      pft->established=TRUE;
#endif
    }
#ifdef DAILY_ESTABLISHMENT
  if(acflux_est>0) /* any establishment occured? */
  {
#endif
  fpc_sum(fpc_type,ntypes,&stand->pftlist);
  foreachpft(pft,p,&stand->pftlist)
    adjust(&stand->soil.litter,pft,fpc_type[pft->par->type]);
  fpc_total=fpc_sum(fpc_type,ntypes,&stand->pftlist);
  if (fpc_total>1.0)
    foreachpft(pft,p,&stand->pftlist)
      reduce(&stand->soil.litter,pft,fpc_total);
#ifdef DAILY_ESTABLISHMENT
  }
#endif
#ifdef DEBUG2
  printf("new npft=%d\n",stand->pftlist.n);
#endif
  free(present);
  free(fpc_type);
  free(n_est);
  return acflux_est;
} /* of 'establishmentpft' */
