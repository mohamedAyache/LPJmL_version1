/***************************************************************************/
/**                                                                       **/
/**               f  p  r  i  n  t  s  o  i  l  .  c                      **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function prints soil variables                                    **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-14 09:09:50 +0200 (mar., 14 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void fprintsoil(FILE *file,      /* pointer to text file */
                const Soil *soil /* pointer to soil variables */
               )
{
  int l;
  char *soilstates[]={"NOSTATE","BELOW_T_ZERO","AT_T_ZERO","ABOVE_T_ZERO",
                      "FREEZING","THAWING"};
  fprintf(file,"Soil type:\t%s\n",soil->par->name);
  fputs("Carbon pools:\n"
        "\tlayer slow (gC/m2) fast (gC/m2)\n"
        "\t----- ------------ ------------\n",file);
  forrootsoillayer(l)
  {
    fprintf(file,"\t%5d %12.2f %12.2f\n",l,soil->cpool[l].slow,soil->cpool[l].fast);
/*    fprintf(file,"k_mean:\t %.2f %.2f\n",soil->k_mean[l].slow,soil->k_mean[l].fast);
    for (p=0;p<24;p++)
     fprintf(file,"c_shift:\t %.2f %.2f\n",soil->c_shift_slow[l][p],soil->c_shift_fast[l][p]);*/
  }
  fputs("\t----- ------------ ------------\n",file);
  fprintf(file,"Rootlayer:\t%d",l);
  fprintf(file,"\ndecomp_litter_mean:\t%.2f (gC/m2)\n",soil->decomp_litter_mean);
  fprintf(file,"Litter:");
  fprintlitter(file,&soil->litter);
  fprintf(file,"\nmean maxthaw:\t%.2f (mm)\n",soil->mean_maxthaw);
  fputs("Layer       ",file);
  foreachsoillayer(l)
    fprintf(file," %12d",l);
  fputs("\n------------",file);
  foreachsoillayer(l)
    fputs(" ------------",file);
  fputs("\nsoil-state  ",file);
  foreachsoillayer(l)
    fprintf(file," %12s",soilstates[soil->state[l]]);    
  fputs("\nWater       ",file);
  foreachsoillayer(l)
    fprintf(file," %12.2f",soil->w[l]);
  fputs("\nfree Water  ",file);
  foreachsoillayer(l)
    fprintf(file," %12.2f",soil->w_fw[l]);
  fputs("\nfree ice    ",file);
  foreachsoillayer(l)
    fprintf(file," %12.2f",soil->ice_fw[l]);
  fputs("\nTemp        ",file);
  for(l=0;l<NSOILLAYER+1;l++)
    fprintf(file," %12.2f",soil->temp[l]);
  fputs("\nIce depth   ",file);
  foreachsoillayer(l)
    fprintf(file," %12.2f",soil->ice_depth[l]);
  fputs("\nFreeze depth",file);
  foreachsoillayer(l)
    fprintf(file," %12.1f",soil->freeze_depth[l]);
  fputs("\nIce pwph    ",file);
  foreachsoillayer(l)
    fprintf(file," %12.2f",soil->ice_pwp[l]);
  fputs("\n------------",file);
  foreachsoillayer(l)
    fputs(" ------------",file);
  fprintf(file,"\nSnowpack:\t%g (mm)\n",soil->snowpack);
} /* of 'fprintsoil' */
