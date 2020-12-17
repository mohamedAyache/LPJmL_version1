/***************************************************************************/
/**                                                                       **/
/**       a  l  l  o  c  a  t  i  o  n  _  g  r  a  s  s  .  c            **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-15 12:26:32 +0200 (mer., 15 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"
#ifdef NEW_GRASS
#include "landuse.h"

Bool allocation_grass(Litter *litter, /* litter pool */
        Pft *pft,        /* pointer to PFT */
        Real *fpc_inc    /* fpc increment */
        )                /* return TRUE on death */
{
    Real bm_inc_ind,lmtorm,lmtormscal;
    Grassphys inc_ind;
    Pftgrass *grass;
    bm_inc_ind=pft->bm_inc/pft->nind;

    if (pft->stand->growing_days>epsilon)
    {
        lmtormscal = pft->wscal_mean/pft->stand->growing_days;
        lmtorm=getpftpar(pft,lmro_ratio)*lmtormscal;
    }
    else
        lmtorm=getpftpar(pft,lmro_ratio);

    pft->wscal_mean=0;
    grass=pft->data;
    /* daily allocation for new grasslands */
    if(pft->stand->type->landusetype==GRASSLAND || pft->stand->type->landusetype==BIOMASS_GRASS)
    {
        /* otherwise there wouldn't be any growth */
        lmtorm=max(lmtorm,0.25);

        if(bm_inc_ind>0.0)
        { /* try to achieve today's lmtorm */

            inc_ind.leaf=(bm_inc_ind+grass->ind.root-grass->ind.leaf/lmtorm)/(1.0+1.0/lmtorm);
            /* no reallocation from leaves to roots */
            inc_ind.leaf=max(inc_ind.leaf,0.0);
            /* no reallocation from roots to leaves */
            inc_ind.leaf=min(inc_ind.leaf,bm_inc_ind);

            inc_ind.root=bm_inc_ind-inc_ind.leaf;
        }
        else
        {
            /* negative bm_inc reduces leaves and roots proportionally */
            inc_ind.leaf=bm_inc_ind*grass->ind.leaf/(grass->ind.root+grass->ind.leaf);
            inc_ind.root=bm_inc_ind*grass->ind.root/(grass->ind.root+grass->ind.leaf);
        }
    }
    else /* this is for natural vegetation with yearly full reallocation */
    {
        if (lmtorm<1.0e-10)
        {
            inc_ind.leaf=0.0;
            inc_ind.root=bm_inc_ind;
        }
        else
        {
            inc_ind.leaf=(bm_inc_ind+grass->ind.root-grass->ind.leaf/lmtorm)/(1.0+1.0/lmtorm);
            if(inc_ind.leaf<0.0)  /*NEGATIVE ALLOCATION TO LEAF MASS */
            {
                inc_ind.root=bm_inc_ind;
                inc_ind.leaf=(grass->ind.root+inc_ind.root)*lmtorm-grass->ind.leaf;
                litter->ag[pft->litter].trait.leaf+=-inc_ind.leaf*pft->nind;
                // barrier.n
                grass->residue.leaf+=-inc_ind.leaf*pft->nind;
                update_fbd_grass(litter,pft->par->fuelbulkdensity,
                        -inc_ind.leaf*pft->nind);
            }
            else
                inc_ind.root=bm_inc_ind-inc_ind.leaf;
        }
    }

    grass->ind.leaf+=inc_ind.leaf;
    grass->ind.root+=inc_ind.root;

    /* barrier.n: adding uptake to root and leaf */
    grass->uptake.leaf += inc_ind.leaf*pft->nind;
    grass->uptake.root += inc_ind.root*pft->nind;
    *fpc_inc=fpc_grass(pft);
    // fprintf(stdout,"allocation grass leaf %g root %g fpcinc %g\n",grass->ind.leaf,grass->ind.root,fpc_inc);
    return (grass->ind.leaf<0 || grass->ind.root<0.0 || pft->fpc<=1e-20);
} /* of 'allocation_grass' */
#else
Bool allocation_grass(Litter *litter, /* litter pool */
        Pft *pft,       /* pointer to PFT */
        Real *fpc_inc   /* fpc increment */
        )                /* return TRUE on death */
{
    Real bm_inc_ind,lmtorm;
    Grassphys inc_ind;
    Pftgrass *grass;
    bm_inc_ind=pft->bm_inc/pft->nind;
    if (pft->stand->growing_days>epsilon)
        lmtorm=getpftpar(pft,lmro_ratio)*(pft->wscal_mean/pft->stand->growing_days);
    else
        lmtorm=getpftpar(pft,lmro_ratio);

    pft->wscal_mean=0;
    grass=pft->data;
    if (lmtorm<1.0e-10)
    {
        inc_ind.leaf=0.0;
        inc_ind.root=bm_inc_ind;
    }
    else
    {
        inc_ind.leaf=(bm_inc_ind+grass->ind.root-grass->ind.leaf/lmtorm)/
            (1.0+1.0/lmtorm);
        if(inc_ind.leaf<0.0)  /*NEGATIVE ALLOCATION TO LEAF MASS */
        {
            inc_ind.root=bm_inc_ind;
            inc_ind.leaf=(grass->ind.root+inc_ind.root)*lmtorm-grass->ind.leaf;
            litter->ag[pft->litter].trait.leaf+=-inc_ind.leaf*pft->nind;

            update_fbd_grass(litter,pft->par->fuelbulkdensity,
                    -inc_ind.leaf*pft->nind);
        }
        else
          {
            inc_ind.root=bm_inc_ind-inc_ind.leaf;
          }
    }
    grass->ind.leaf+=inc_ind.leaf;
    grass->ind.root+=inc_ind.root;

    /*barrier.n: uptake (gC/m2) and residue (gC/m2)*/

    if (inc_ind.leaf>0) {grass->uptake.leaf += inc_ind.leaf*pft->nind;} else /*death of leaves*/ {grass->residue.leaf+=-inc_ind.leaf*pft->nind;}//gC/plant * nbofplants/m2
    if (inc_ind.root>0) {grass->uptake.root += inc_ind.root*pft->nind;} else /*death of roots*/ {grass->residue.root+=-inc_ind.root*pft->nind;}

    *fpc_inc=fpc_grass(pft);

    return (grass->ind.leaf<0 || grass->ind.root<0.0 || pft->fpc<=1e-20);
} /* of 'allocation_grass' */
#endif
