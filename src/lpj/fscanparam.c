/***************************************************************************/
/**                                                                       **/
/**               f  s  c  a  n  p  a  r  a  m  .  c                      **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads LPJmL parameter                                    **/
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

#define fscanparamreal(isout,file,var,name) \
  if(fscanreal(file,var,name,isout)) return TRUE; 
#define fscanparamint(isout,file,var,name) \
  if(fscanint(file,var,name,isout)) return TRUE;

Param param; /* global LPJ parameter */

Bool fscanparam(FILE  *file,         /* File pointer to ASCII file */
                const Config *config /* LPJ configuration */
               )                     /* returns TRUE on error  */
{
  fscanparamreal(isroot(*config),file,&param.k_litter10,"k_litter10");
  fscanparamreal(isroot(*config),file,&param.k_soil10.fast,"fast k_soil10");
  fscanparamreal(isroot(*config),file,&param.k_soil10.slow,"slow k_soil10");
  fscanparamreal(isroot(*config),file,&param.maxsnowpack,"max snow pack");
  fscanparamreal(isroot(*config),file,&param.soildepth_evap,"soildepth_evap");
  fscanparamreal(isroot(*config),file,&param.co2_p,"co2_p");
  fscanparamreal(isroot(*config),file,&param.k,"k");
  fscanparamreal(isroot(*config),file,&param.theta,"theta");
  fscanparamreal(isroot(*config),file,&param.k_beer,"k_beer");
  fscanparamreal(isroot(*config),file,&param.alphac3,"alphac3");
  fscanparamreal(isroot(*config),file,&param.alphac4,"alphac4");
  fscanparamreal(isroot(*config),file,&param.bc3,"bc3");
  fscanparamreal(isroot(*config),file,&param.bc4,"bc4");
  fscanparamreal(isroot(*config),file,&param.r_growth,"r_growth");
  fscanparamreal(isroot(*config),file,&param.GM,"GM");
  fscanparamreal(isroot(*config),file,&param.ALPHAM,"ALPHAM");
  fscanparamreal(isroot(*config),file,&param.ko25,"ko25");
  fscanparamreal(isroot(*config),file,&param.kc25,"kc25");
  fscanparamreal(isroot(*config),file,&param.atmfrac,"atmfrac");
  fscanparamreal(isroot(*config),file,&param.fastfrac,"fastfrac");
  fscanparamreal(isroot(*config),file,&param.k_mort_max,"k_mort_max");
  fscanparamreal(isroot(*config),file,&param.k_mort,"k_mort");

  if(config->withlanduse!=NO_LANDUSE)
  {
    fscanparamreal(isroot(*config),file,&param.aprec_lim,
                   "aprec_lim");
    fscanparamreal(isroot(*config),file,&param.irrig_threshold_c3_dry,
                   "irrig_threshold_c3_dry");
    fscanparamreal(isroot(*config),file,&param.irrig_threshold_c3_humid,
                   "irrig_threshold_c3_humid");
    fscanparamreal(isroot(*config),file,&param.irrig_threshold_c4,
                   "irrig_threshold_c4");
    fscanparamreal(isroot(*config),file,&param.irrig_threshold_rice,
                   "irrig_threshold_rice");
    fscanparamreal(isroot(*config),file,&param.irrigation_soilfrac,
                   "irrigation_soilfrac");
    fscanparamreal(isroot(*config),file,&param.ec_canal[0],
                   "canal_conveyance_eff_sand");
    fscanparamreal(isroot(*config),file,&param.ec_canal[1],
                   "canal_conveyance_eff_loam");
    fscanparamreal(isroot(*config),file,&param.ec_canal[2],
                   "canal_conveyance_eff_clay");
    fscanparamreal(isroot(*config),file,&param.ec_pipe,
                   "pipe_conveyance_eff");
    param.sat_level[0]=0; /* default value */
    fscanparamreal(isroot(*config),file,&param.sat_level[1],
                   "saturation_level_surf");
    fscanparamreal(isroot(*config),file,&param.sat_level[2],
                   "saturation_level_sprink");
    fscanparamreal(isroot(*config),file,&param.sat_level[3],
                   "saturation_level_drip");
    fscanparamreal(isroot(*config),file,&param.drip_evap,
                   "drip_evap_reduction");
    fscanparamreal(isroot(*config),file,&param.laimax,"laimax");
    fscanparamint(isroot(*config),file,&param.intercrop,"intercrop");
    fscanparamint(isroot(*config),file,&param.remove_residuals,
                  "remove_residuals");
    fscanparamint(isroot(*config),file,&param.sdate_fixyear,"sdate_fixyear");
    fscanparamint(isroot(*config),file,&param.landuse_year_const,"landuse_year_const");
    if(config->rw_manage)
    {
      fscanparamreal(isroot(*config),file,&param.esoil_reduction,
                  "esoil_reduction");
      fscanparamreal(isroot(*config),file,&param.rw_buffer_max,
                   "rw_buffer_max");
      fscanparamreal(isroot(*config),file,&param.frac_ro_stored,
                   "frac_ro_stored");
      fscanparamreal(isroot(*config),file,&param.rw_irrig_thres,
                    "rw_irrig_thres");
      fscanparamreal(isroot(*config),file,&param.soil_infil,
                    "soil_infil");
      fscanparamreal(isroot(*config),file,&param.yield_gap_bridge,
                    "yield_gap_bridge");
      }
  }
  param.k_litter10/=NDAYYEAR;
  param.k_soil10.fast/=NDAYYEAR;
  param.k_soil10.slow/=NDAYYEAR;
  return FALSE;
} /* of 'fscanparam' */
