/***************************************************************************/
/**                                                                       **/
/**             t  i  m  b  e  r  _  h  a  r  v  e  s  t  .  c            **/
/**                                                                       **/
/**     extension of LPJ to compute timber harvest / burning              **/
/**                                                                       **/
/**     written by Christoph Mueller                                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 23.01.2009 by Werner von Bloh                        **/
/**     Last change: 19.08.2009 by Christoph Mueller & Elke Stehfest      **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

Real timber_harvest(Pft *pft,      /* Pointer to tree PFT */
                    Soil *soil,      /* Litter pool */
                    Pool *timber,
                    Pool f,        /* fractions for timber distribution */
                    Real ftimber,
                    Real standfrac,
                    Real *nind,          /* cannot use pft->nind, since pft is on different stand */
                    Real *trad_biofuel
                    )
{
  int i;
  Pfttree *tree;
  const Pfttreepar *treepar;
  Real harvest;
  Real biofuel;
  tree=pft->data;
  treepar=pft->par->data;
  if(ftimber<epsilon)
    return(0.);
  /* transfer wood to product pools, assume 2/3 of sapwood to be above-ground */
  harvest=(tree->ind.heartwood+tree->ind.sapwood*2.0/3.0)*
                 ftimber*(*nind)*standfrac;
  timber->slow+=harvest*f.slow;
  timber->fast+=harvest*f.fast;
  biofuel=1.0-f.slow-f.fast;
  *trad_biofuel+=harvest*biofuel*0.9;
  /* 10% of traditional biofuel is assumed to enter fast soil pool -- may not be scaled with standfrac!*/
  soil->cpool[0].fast+=harvest*biofuel*0.1/standfrac;
  /* transfer non-harvested wood, leaves, and roots of trees cut to litter */
  soil->litter.ag[pft->litter].trait.leaf+=tree->ind.leaf*ftimber*(*nind);

  tree->residue.leaf+=tree->ind.leaf*ftimber*(*nind);

  for(i=0;i<NFUELCLASS;i++)
    {
    soil->litter.ag[pft->litter].trait.wood[i]+=(tree->ind.sapwood/3.0-tree->ind.debt)*
                                         ftimber*(*nind)*treepar->fuelfrac[i];
    tree->residue.sapwood+=(tree->ind.sapwood/3.0-tree->ind.debt)*
                                         ftimber*(*nind)*treepar->fuelfrac[i];
  }
  soil->litter.bg[pft->litter]+=tree->ind.root*ftimber*(*nind);
  tree->residue.root+=tree->ind.root*ftimber*(*nind);
  /* update carbon pools by reducing nind by number of trees cut */
  *nind*=(1-ftimber);
  return(harvest);
} /* of 'timber_harvest' */
