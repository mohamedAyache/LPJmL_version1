/***************************************************************************/
/**                                                                       **/
/**                  i  n  t  e  r  c  e  p  t  i  o  n  .  c             **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 13.10.2005                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real interception(Real *wet,
                  const Pft *pft, /* PFT variables */
                  Real pet,       /* potential evapotranspiration (mm) */
                  Real rain       /* precipitation (mm) */
                 )                /* returns interception (mm) */ 
{
  Real int_store;
  if(pet<0.0001)
  {
    *wet=0;
    return 0; 
  }
  int_store=pft->par->intc*actual_lai(pft);
  if(int_store>0.9999)
    int_store=0.9999;
  *wet=int_store*rain/(pet*PRIESTLEY_TAYLOR);
  if (*wet>0.9999) 
    *wet=0.9999;
#ifdef SAFE
  if(rain<pet*PRIESTLEY_TAYLOR*(*wet)*pft->fpc)
  {
    printf("par_intc: %f, lai: %f, phen: %f pet: %f rain: %f pft: %s\n",
            pft->par->intc,actual_lai(pft),pft->phen,pet,rain,pft->par->name);
    fflush(stdout);
  }
#endif
  
  return pet*PRIESTLEY_TAYLOR*(*wet)*pft->fpc;
} /* of interception */
