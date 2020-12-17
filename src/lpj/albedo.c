/***************************************************************************/
/**                                                                       **/
/**                    a  l  b  e  d  o  .  c                             **/
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

static const Real c_albAntarctica  = 0.7;
static const Real c_roughness      = 0.06; /* Roughness height of vegetation below the canopy. Source: FOAM/LPJ */

static Real albedo_stand(const Stand *stand)
{
  int p;
  Real frsg;  /* Snow cover of the ground below the canopy (0-1) */
  Real HS;    /* Height of the Snow (m) */
  Real albstot;/* Sum of all albs over all pfts */
  Real fbare;  /* Fraction bare soil */
  const Pft *pft;
  Real test;
  if (stand->soil.par->type == ICE /* ICE */)
  {
    if (stand->cell->coord.lat<-60)
      return c_albAntarctica;
    return c_albsnow;
  }
  HS = c_watertosnow * (stand->soil.snowpack/1000.0); /* mm -> m */
  frsg = HS / (HS+0.5*c_roughness); /* If HS is large, frsg will be close to 1. For small values of HS, frsg will be less than 1, because then a significant amount of the snow is captured by the canopy */
  fbare = 0.0;
  albstot = 0.0;

  foreachpft(pft,p,&stand->pftlist)
  {
    albstot += pft->par->albedo(pft,HS,frsg); /* call PFT-specific albedo function */
    fbare += pft->fpc;
  } /* pft loop */
  fbare = max((1-fbare),0.0);

  test = albstot + fbare * (frsg * c_albsnow + (1-frsg) * c_albsoil);

#ifndef NDEBUG
  if (test > 1 || test < 0 || isnan(test))
  {
    printf("WARNING albedo out of bounds: %g fbare=%g albstot=%g\n", test, fbare, albstot);
    foreachpft(pft,p,&stand->pftlist)
    {
      printf("pft %s fpc %g phen %g\n", pft->par->name, pft->fpc, pft->phen);
    }
  }
#endif

  return  test;

} /* end of albedo_stand() */

Real albedo(const Cell *cell)
{
  const Stand *stand;
  int s;
  Real a;
  a=0;
  foreachstand(stand,s,cell->standlist)
    a+=albedo_stand(stand)*stand->frac;
  return a;
} /* of 'albedo' */
