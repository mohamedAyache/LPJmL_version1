/***************************************************************************/
/**                                                                       **/
/**                             l  i  g  h  t  .  c                       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function calculates competition for light among PFTs              **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 01.10.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"
#include "grass.h"

void light(Stand *stand,        /* Pointer to stand */
           int ntypes,          /* number of PFT classes */
           const Real fpc_inc[] /* FPC increment for each established PFT */
          )

{
  int p;
  Real ntree;        /* no of tree PFTs currently present */
  Real *fpc_total;   /* total grid FPC for PFTs */ 
  Real fpc_inc_tree; /* this years total FPC increment for tree PFTs */
  Real excess;       /* tree FPC or grass cover to be reduced */
  Real f,g,h;
  Pft *pft;
  /* Calculate total woody FPC, FPC increment and grass cover (= crown area) */
 
  fpc_inc_tree=0.0;
  ntree=0;
  fpc_total=newvec(Real,ntypes);
  check(fpc_total);
  fpc_sum(fpc_total,ntypes,&stand->pftlist);

  foreachpft(pft,p,&stand->pftlist) 
    if(istree(pft))
    {
      fpc_inc_tree+=fpc_inc[p];
      ntree++;
    }
  if(ntree)
    f=(fpc_total[TREE]-FPC_TREE_MAX)/ntree;
  if(fpc_inc_tree)
    g=(fpc_total[TREE]-FPC_TREE_MAX)/fpc_inc_tree;
  if(fpc_total[GRASS])
    h=(min(fpc_total[TREE],FPC_TREE_MAX)+fpc_total[GRASS]-1.0)/fpc_total[GRASS];
  foreachpft(pft,p,&stand->pftlist) 
  {
    switch(getpftpar(pft,type))
    {
      case TREE:
        if (fpc_total[TREE]>FPC_TREE_MAX) 
        {
          excess =(fpc_inc_tree>0.0) ?  g*fpc_inc[p] : f;

          /* Reduce individual density (and thereby gridcell-level biomass)*/
          /* so that total tree FPC reduced to 'fpc_tree_max'*/
          /* changed by Werner von Bloh to avoid FPE */
          
          light_tree(&stand->soil.litter,pft,excess);
        }
        break;
                              
      case GRASS:
        if(fpc_total[GRASS]>(1.0-min(fpc_total[TREE],FPC_TREE_MAX))) 
        {
          excess=h*pft->fpc;
          light_grass(&stand->soil.litter,pft,excess);
        }
        break;
    } /* of 'switch' */
  }  /* of 'foreachpft' */
  free(fpc_total);
} /* of 'light' */
