/***************************************************************************/
/**                                                                       **/
/**             p  h  e  n  o  l  o  g  y  _  t  r  e  e  .  c            **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 24.08.2012                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

void phenology_tree(Pft *pft,  /* pointer to PFT variables */
        Real temp, /* temperature (deg C) */
        int day    /* day of year (1..365) */
        )
{
    Pfttree *tree;
    Real dtemp,gddtw;
    Pfttreepar *treepar;
    tree=pft->data;
    treepar=pft->par->data;
    dtemp=temp - getpftpar(pft,gddbase);
    gddtw=temp - getpftpar(pft,twmax);
    if(dtemp>0.0)
        pft->gdd+=dtemp;
    tree->gddtw+= (gddtw>0.0) ? gddtw : 0.0;
    switch(getpftpar(pft,phenology))
    {
        case SUMMERGREEN:
            if(pft->aphen<treepar->aphen_max)
            {
                pft->phen=pft->gdd*getpftpar(pft,ramp);
                if(pft->phen>1)
                    pft->phen=1;
            }
            else
            {
                if(!tree->isphen)
                {
                    if (tree->turn.leaf!=treepar->sapl.leaf*treepar->turnover.leaf)
                    {
                        tree->turn.leaf+=tree->ind.leaf*treepar->turnover.leaf;
                        tree->turn_litt.leaf+=tree->ind.leaf*treepar->turnover.leaf*pft->nind;
                        pft->stand->soil.litter.ag[pft->litter].trait.leaf+=tree->ind.leaf*treepar->turnover.leaf*pft->nind;

                        tree->residue.leaf += tree->ind.leaf*treepar->turnover.leaf*pft->nind;


                        update_fbd_tree(&pft->stand->soil.litter,pft->par->fuelbulkdensity,tree->ind.leaf*treepar->turnover.leaf*pft->nind,0);
                    }
                    pft->phen=0.0;
                }
                tree->isphen=TRUE;
            }
            /* cold-induced senescence;
               works only for linearly interpolated, monthly temperature data! */
            if(dtemp<0 && pft->aphen>treepar->aphen_min && !tree->isphen && tree->turn.leaf<epsilon)
            {
                tree->isphen=TRUE;
                pft->phen=pft->gdd=0.0;
                if (tree->turn.leaf!=treepar->sapl.leaf*treepar->turnover.leaf)
                {
                    tree->turn.leaf+=tree->ind.leaf*treepar->turnover.leaf;
                    tree->turn_litt.leaf+=tree->ind.leaf*treepar->turnover.leaf*pft->nind;
                    pft->stand->soil.litter.ag[pft->litter].trait.leaf+=tree->ind.leaf*treepar->turnover.leaf*pft->nind;
                    // barrier.n
                    tree->residue.leaf += tree->ind.leaf*treepar->turnover.leaf*pft->nind;
                    update_fbd_tree(&pft->stand->soil.litter,pft->par->fuelbulkdensity,tree->ind.leaf*treepar->turnover.leaf*pft->nind,0);
                }
            }
            break;
        case RAINGREEN:
            tree->turn.leaf+=tree->ind.leaf*treepar->turnover.leaf/NDAYYEAR;
            tree->turn_litt.leaf+=tree->ind.leaf*treepar->turnover.leaf/NDAYYEAR*pft->nind;
            pft->stand->soil.litter.ag[pft->litter].trait.leaf+=tree->ind.leaf*treepar->turnover.leaf/NDAYYEAR*pft->nind;
            // barrier.n //
            tree->residue.leaf += tree->ind.leaf*treepar->turnover.leaf/NDAYYEAR*pft->nind;
            update_fbd_tree(&pft->stand->soil.litter,pft->par->fuelbulkdensity,tree->ind.leaf*treepar->turnover.leaf/NDAYYEAR*pft->nind,0);

            if(pft->wscal<getpftpar(pft,minwscal))
                pft->phen=0.0;
            else
            {
                pft->phen=1.0;
                tree->aphen_raingreen++;
            }
            break;
        case EVERGREEN:
            pft->phen=1;
            tree->turn.leaf+=tree->ind.leaf*treepar->turnover.leaf/NDAYYEAR;
            tree->turn_litt.leaf+=tree->ind.leaf*treepar->turnover.leaf/NDAYYEAR*pft->nind;
            pft->stand->soil.litter.ag[pft->litter].trait.leaf+=tree->ind.leaf*treepar->turnover.leaf/NDAYYEAR*pft->nind;
            // barrier.n
            tree->residue.leaf+=tree->ind.leaf*treepar->turnover.leaf/NDAYYEAR*pft->nind;
            update_fbd_tree(&pft->stand->soil.litter,pft->par->fuelbulkdensity,tree->ind.leaf*treepar->turnover.leaf/NDAYYEAR*pft->nind,0);
            break;
    } /* of 'switch' */
    if ((pft->stand->cell->coord.lat>=0.0 && day==NDAYYEAR) ||
            (pft->stand->cell->coord.lat<0.0 && day==COLDEST_DAY_SHEMISPHERE))
    {
        pft->aphen=pft->gdd=0.0;
        tree->isphen=FALSE;

    }
    if (pft->phen>0)
        pft->aphen++;

} /* of 'phenology_tree' */
