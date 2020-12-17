/***************************************************************************/
/**                                                                       **/
/**               a  l  b  e  d  o  _  g  r  a  s  s  .  c                **/
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
/**     Last change: 17.09.2012 by Werner von Bloh                        **/
/**                                                                       **/
/***************************************************************************/
/**                                                                       **/
/** This code was originally written for coupling LPJ with the SPEEDY     **/
/** atmosphere model. It was revived and adapted for coupling LPJ         **/
/** into Climber-4 in April 2012.                                         **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"

static const Real c_albgrass = 0.35; /* Albedo of grass without snowcover (0-1) */

Real albedo_grass(const Pft *pft,  /* pointer to PFT */
                  Real snowheight, /* snow height (m) */
                  Real frsg        /* snow cover of the ground below the canopy (0-1) */
                 )                 /* returns albedo (0..1) */
{
  Real frs,frs1,frs2,albnof,albpft,albs;

  frs1 = frsg*(pft->phen * c_snowcanopyfrac + (1 - pft->phen));
  frs2 = 1;
  if (snowheight<=0)
    frs2 = 0;

  frs = frs1 + (1-frs1)*frs2;
  if (snowheight>0)
    frs = 1;
  albnof = c_alblit;
  albpft = pft->fpc*pft->phen * c_albgrass + pft->fpc * (1-pft->phen) * albnof;
  albs = pft->fpc*frs * c_albsnow + (1-frs) * albpft;
  if (isnan(albs))
  {
    printf("WARNING albs=%g fpc=%g c_albsnow=%g frs=%g albpft=%g\n",
           albs, pft->fpc, c_albsnow, frs, albpft);
  }
  return albs;
} /* of 'albedo_grass' */
