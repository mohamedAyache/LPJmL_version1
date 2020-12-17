/***************************************************************************/
/**                                                                       **/
/**                f  p  r  i  n  t  p  a  r  a  m  .  c                  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function prints LPJ parameter                                     **/
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

void fprintparam(FILE *file,    /* pointer to text file */
                 int npft,      /* number of natural PFTs */
                 int ncft,      /* number of crop PFTs */
                 const Config *config /* LPJ configuration */
                )
{
  int p;
  fprintf(file,"Parameter settings\n"
               "k_litter10:\t%g (1/yr)\n"
               "k_soil10:\t%g %g (1/yr)\n"
               "max snow pack:\t%g (mm)\n"
               "evap soildepth:\t%g (mm)\n"
               "pre. CO2:\t%g (ppm)\n"
               "k:\t\t%g (1/yr)\n"
               "theta:\t\t%g\n"
               "k_beer:\t\t%g\n"
               "alphac3:\t%g\n"
               "alphac4:\t%g\n"
               "bc3:\t\t%g\n"
               "bc4:\t\t%g\n"
               "r_growth:\t%g\n"
               "GM:\t\t%g\n"
               "ALPHAM:\t\t%g\n"
               "ko25:\t\t%g (Pa)\n"
               "kc25:\t\t%g (Pa)\n"
               "atmfrac:\t%g\n"
               "fastfrac:\t%g\n"
	       "K_MORT_MAX:\t%g (1/yr)\n"
	       "K_MORT:\t\t%g\n",
          param.k_litter10*NDAYYEAR,param.k_soil10.fast*NDAYYEAR,
          param.k_soil10.slow*NDAYYEAR,param.maxsnowpack,param.soildepth_evap,param.co2_p,
          param.k,param.theta,param.k_beer,param.alphac3,param.alphac4,
          param.bc3,param.bc4,param.r_growth,param.GM,param.ALPHAM,
          param.ko25,param.kc25,param.atmfrac,param.fastfrac,param.k_mort_max,param.k_mort);

  if(config->withlanduse!=NO_LANDUSE)
  {
    fprintf(file,"annual prec lim:\t%g\n",
          param.aprec_lim);
    fprintf(file,"irrig. threshold C3 dry:\t%g\n",
          param.irrig_threshold_c3_dry);
    fprintf(file,"irrig. threshold C3 humid:\t%g\n",
          param.irrig_threshold_c3_humid);
    fprintf(file,"irrig. threshold C4:\t%g\n",
          param.irrig_threshold_c4);
    fprintf(file,"irrig. threshold Rice:\t%g\n",
          param.irrig_threshold_rice);
  }
  fputs("Soil parameter\n",file);
  fprintsoilpar(file,config->soilpar,config->nsoil);
  fputs("PFT parameter\n",file);
  if(config->withlanduse!=NO_LANDUSE)
  {
    for(p=0;p<npft+ncft;p++)
      fprintpftpar(file,config->pftpar+p);
    fputs("------------------------------------------------------------------------------\n"
          "Country parameter\n",file);
    fprintcountrypar(file,config->countrypar,config->ncountries,ncft);
    fputs("Region parameter\n",file);
    fprintregionpar(file,config->regionpar,config->nregions);
  }
  else
    for(p=0;p<npft-config->nbiomass;p++)
      fprintpftpar(file,config->pftpar+p);
  fputs("------------------------------------------------------------------------------\n",file);
} /* of 'fprintparam' */
