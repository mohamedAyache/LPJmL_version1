/***************************************************************************/
/**                                                                       **/
/**              f  r  e  a  d  s  o  i  l  .  c                          **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads soil data from file                                **/
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

Bool freadsoil(FILE *file,             /* File pointer to binary file */
               Soil *soil,             /* Pointer to soil data */
               const Soilpar *soilpar, /* soil parameter array */
               const Pftpar pftpar[],  /* PFT parameter array */
               int ntotpft,            /* total number of PFTs */
               Bool swap   /* Byte order has to be changed (TRUE/FALSE) */
              )            /* returns TRUE on error */
{
  int l;
  soil->par=soilpar;
  forrootsoillayer(l)
  {
    freadreal((Real *)&soil->cpool[l],sizeof(Pool)/sizeof(Real),swap,file);
    soil->c_shift_fast[l]=newvec(Real,ntotpft);
    if(soil->c_shift_fast[l]==NULL)
      return TRUE;
    soil->c_shift_slow[l]=newvec(Real,ntotpft);
    if(soil->c_shift_slow[l]==NULL)
      return TRUE;
    freadreal(soil->c_shift_fast[l],ntotpft,swap,file);
    freadreal(soil->c_shift_slow[l],ntotpft,swap,file);
  }
  if(freadlitter(file,&soil->litter,pftpar,ntotpft,swap))
    return TRUE;
  freadreal(soil->w,NSOILLAYER,swap,file);
  freadreal1(&soil->w_evap,swap,file);
  freadreal(soil->w_fw,NSOILLAYER,swap,file);
  freadreal1(&soil->snowpack,swap,file);
  freadreal(soil->temp,NSOILLAYER+1,swap,file);
  freadreal(soil->ice_depth,NSOILLAYER,swap,file);
  freadreal(soil->ice_fw,NSOILLAYER,swap,file);
  freadreal(soil->freeze_depth,NSOILLAYER,swap,file);
  freadreal(soil->ice_pwp,NSOILLAYER,swap,file);
  freadreal(soil->perc_energy,NSOILLAYER,swap,file);
  freadshort(soil->state,NSOILLAYER,swap,file);
  freadreal1(&soil->mean_maxthaw,swap,file);
  freadreal1(&soil->alag,swap,file);
  freadreal1(&soil->amp,swap,file);
  foreachsoillayer(l)
    soil->decomC[l]=soil->micro_heating[l]=0;
  forrootsoillayer(l)
   soil->k_mean[l].fast=soil->k_mean[l].slow=0;
  soil->count=0;
  soil->decomp_litter_mean=soil->litter.decomC=soil->YEDOMA=0;
  soil->rw_buffer=0.0;
  /*forrootsoillayer(l)
    soil->YEDOMA+=soil->cpool[l].slow;*/
  return (freadreal1(&soil->meanw1,swap,file)!=1);
} /* of 'freadsoil' */
