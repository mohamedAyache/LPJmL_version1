/***************************************************************************/
/**                                                                       **/
/**     i n i t m a n a g e .  c                                          **/
/**                                                                       **/
/**     C implementation of LPJ                                           **/
/**                                                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 03.05.2007, Marlies Gumpenberger                     **/
/**     Last change: $Date:: 2015-07-24 11:14:02 +0200 (ven., 24 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void initmanage(Manage *manage,const Countrypar *countrypar,
                const Regionpar *regionpar,int npft,int nagtree,int ncft,Bool isconstlai)
{
  int cft;
  manage->par=countrypar;    
  manage->regpar=regionpar;
  if(manage->par->laimax_cft==NULL)
    manage->laimax=newvec2(Real,npft,npft+ncft-1);  /* allocate memory for country-specific laimax*/
  else
    manage->laimax=manage->par->laimax_cft-npft;  /* set pointer to country specific laimax */
  manage->k_est=manage->par->k_est-npft+nagtree; /* set pointer to country specific k_est */
 
  if(isconstlai)
    for(cft=0;cft<ncft;cft++)
      manage->laimax[npft+cft]=param.laimax;
} /* of 'initmanage' */
