/***************************************************************************/
/**                                                                       **/
/**               a  l  b  e  d  o  _  t  r  e  e  .  c                   **/
/**                                                                       **/
/**     Computation of albedo as described by Bob Gallimore (in an e-mail)**/
/**     and used by him in the coupling of FOAM to LPJ.                   **/
/**                                                                       **/
/**     written by Roy van den Houdt, Bart Strengers                      **/
/**     MNP/KMD                                                           **/
/**     PO Box 303                                                        **/
/**     3720 AH Bilthoven/The Netherlands                                 **/
/**                                                                       **/
/**     Last change: 15.08.2006                                           **/
/**     Last change: $Date:: 2013-03-17 11:55:50 +0100 (dim., 17 mars 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/
/**                                                                       **/
/** This code was originally written for coupling LPJ with the SPEEDY     **/
/** atmosphere model. It was revived and adapted for coupling LPJ         **/
/** into Climber-4 in April 2012.                                         **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

static const Real c_fstem          = 0.70; /* Masking of the ground by stems and branches without leaves. Source: FOAM/LPJ */
static const Real c_albtree        = 0.24; /* Albedo of trees without snowcover (0-1) */
static const Real c_albstem        = 0.24; /* Albedo of stems and branches without leaves */
static const Real c_hbot           = 0.20; /* Height of canopy bottom as fraction of the tree height (0-1) */
static const Real height_min       = 0.5;

Real albedo_tree(const Pft *pft,  /* pointer to PFT */
                 Real snowheight, /* height of snow (m) */
                 Real frsg        /* snow cover of the ground below the canopy (0-1) */
                )                 /* returns albedo (0..1) */
{
  const Pfttree *tree;
  Real frs,frs1,frs2,htop,hbot,albnof,albpft,albs;
  tree=pft->data;
  htop=hbot=0.0;
  if (tree->height>height_min)
  {
    htop = tree->height;
    hbot = c_hbot*tree->height;
  }

  frs1 = frsg*(pft->phen * c_snowcanopyfrac + (1 - pft->phen) * (1 - c_fstem));
  frs2 = 1;
  if (snowheight<=hbot)
    frs2 = 0;
  else if (snowheight<htop)
    frs2 = (snowheight-hbot)/(htop-hbot);

  frs = frs1 + (1-frs1)*frs2;
  if (snowheight>htop)
    frs = 1;
  albnof = c_fstem * c_albstem + (1-c_fstem) * c_alblit;
  albpft = pft->fpc*pft->phen * c_albtree + pft->fpc * (1-pft->phen) * albnof;
  albs = pft->fpc*frs * c_albsnow + (1-frs) * albpft;
  if (isnan(albs))
  {
    printf("WARNING albs=%g fpc=%g c_albsnow=%g frs=%g albpft=%g\n",
           albs, pft->fpc, c_albsnow, frs, albpft);
  }
  return albs;
} /* of 'albedo_tree' */
