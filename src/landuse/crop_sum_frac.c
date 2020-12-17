/***************************************************************************/
/**                                                                       **/
/**            c  r  o  p  _  s  u  m  _  f  r  a  c  .  c                **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real crop_sum_frac(Landfrac landfrac[2],
                   int ncft, 
                   int nagtree,
                   Real waterfrac,
                   Bool irrig            /* for irrigated crop or not? */
                   ) 
{
   Real factor,sumfrac,sumfracall;

   sumfrac=landfrac_sum(landfrac,ncft,nagtree,irrig);
   sumfracall=landfrac_sum(landfrac,ncft,nagtree,TRUE)+landfrac_sum(landfrac,ncft,nagtree,FALSE);

   /* if the cropfrac > 1- reservoirfrac, reduce the cropfrac to the maximum area */
   if(sumfracall>(1-waterfrac))
   {
     factor=(1-waterfrac)/sumfracall;
     scalelandfrac(landfrac,ncft,nagtree,factor);
   }
   else
     factor=1;
   
   return sumfrac*factor;

} /* of 'crop_sum_frac' */
