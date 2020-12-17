/***************************************************************************/
/**                                                                       **/
/**                f  p  r  i  n  t  p  f  t  p  a  r  .  c               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function prints PFT parameter                                     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 14.01.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void fprintpftpar(FILE *file, /* pointer to text file */
                  const Pftpar *pftpar /* pointer to PFT parameter */
                 )
{
  int i;
  char *phenology[]={"evergreen","raingreen","summergreen","any","cropgreen"};
  char *cultivation_type[]={"none","biomass","annual crop","annual tree"};
  char *path[]={"no pathway","C3","C4"};
  fputs("------------------------------------------------------------------------------\n",file);
  fprintf(file,"Id:\t\t%d\n"
               "Name:\t\t%s\n"
               "Cult. type:\t%s\n",pftpar->id,pftpar->name,cultivation_type[pftpar->cultivation_type]);
  fprintf(file,"rootdist:\t");
  for(i=0;i<LASTLAYER;i++)
    fprintf(file,"%g ",pftpar->rootdist[i]);
  fprintf(file,"\nCN:\t\t");
  for(i=0;i<NHSG;i++)
    fprintf(file,"%g ",pftpar->cn[i]);
  fprintf(file,"\n"
               "beta root:\t%g\n"
               "minwscal:\t%g\n"
               "gmin:\t\t%g (mm/s)\n"
               "respcoeff:\t%g\n"
               "nmax:\t\t%g (mg/g)\n"
               "resist:\t\t%g\n"
               "longevity:\t%g (yr)\n"
               "lmro ratio:\t%g\n"
               "ramp:\t\t%g\n"
               "LAI sapl:\t%g\n"
               "gdd5min:\t%g\n"
               "twmax:\t\t%g (deg C)\n"
               "gddbase:\t%g (deg C)\n"
               "min temprange:\t%g\n"
               "emax:\t\t%g (mm/d)\n"
               "intc:\t\t%g\n"
               "alphaa:\t\t%g\n"
               "phenology:\t%s\n"
               "path:\t\t%s\n"
               "temp CO2:\t%g %g (deg C)\n"
               "temp photos:\t%g %g (deg C)\n"
               "temp:\t\t%g %g (deg C)\n"
               "min aprec:\t%g (mm)\n"
               "flam:\t\t%g\n"
               "k_litter10:\t%g %g (1/yr)\n"
               "fuel bulk:\t%g\n"
               "emis. factor:\t%g %g %g %g %g %g\n"
               "wind damp.:\t%g\n"
               "roughness length:\t%g\n",
          pftpar->beta_root,
          pftpar->minwscal,pftpar->gmin,pftpar->respcoeff,pftpar->nmax,
          pftpar->resist,
          pftpar->longevity,pftpar->lmro_ratio,1.0/pftpar->ramp,
          pftpar->lai_sapl,pftpar->gdd5min,
          pftpar->twmax,pftpar->gddbase,pftpar->min_temprange,
          pftpar->emax,pftpar->intc,
          pftpar->alphaa ,phenology[pftpar->phenology],path[pftpar->path],
          pftpar->temp_co2.low,pftpar->temp_co2.high,pftpar->temp_photos.low,
          pftpar->temp_photos.high,pftpar->temp.low,pftpar->temp.high,
          pftpar->aprec_min,
          pftpar->flam,pftpar->k_litter10.leaf*NDAYYEAR,
          pftpar->k_litter10.wood*NDAYYEAR,
          pftpar->fuelbulkdensity,pftpar->emissionfactor.co2,
          pftpar->emissionfactor.co,pftpar->emissionfactor.ch4,
          pftpar->emissionfactor.voc,pftpar->emissionfactor.tpm,
          pftpar->emissionfactor.nox,pftpar->windspeed,pftpar->roughness);
  pftpar->fprintpar(file,pftpar); /* call type-specific print function */
} /* of 'fprintpftpar' */
