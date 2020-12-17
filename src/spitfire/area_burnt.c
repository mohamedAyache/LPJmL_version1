/***************************************************************************/
/**                                                                       **/
/**                        a r e a _ b u r n t . c                        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Kirsten Thonicke, Werner von Bloh, Sibyll Schaphoff    **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 11.03.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real area_burnt(Real fire_danger_index, Real num_fires, Real windsp_cover,
                Real ros_forward, int ntypes,const Pftlist *pftlist)
{
  Real dbf,length_breath_ratio;
  Real lb_grass,lb_tree,base;
  Real ros_backward, fire_durat;
  Real *fpc_total; /* total grid FPC for PFTs */
  Real d_area_burnt;

  if(ros_forward<=0)
    return 0;
  ros_backward = ros_forward * exp(-0.012 * windsp_cover); /*Can FBP System*/
  if (ros_backward <= 0.05)
    ros_backward = 0.0;

  if (windsp_cover < 16.67)
    length_breath_ratio = 1;
  else
  {
    windsp_cover*=0.06;
    base=1.0-(exp(-0.03*windsp_cover));
    /* total tree fpc and total grass fpc for dampening effect on wind speed */
    fpc_total=newvec(Real,ntypes);
    check(fpc_total);
    fpc_sum(fpc_total,ntypes,pftlist);
    lb_tree=fpc_total[TREE]*(1.0+(8.729*(pow(base,2.155))));
    lb_grass=fpc_total[GRASS]*(1.1+pow(windsp_cover,0.464)); /*Kirsten corrected 08Sep10 after CFFBPS*/
    length_breath_ratio=lb_tree+lb_grass;
    free(fpc_total);
    if (length_breath_ratio > 8)
      length_breath_ratio = 8;
  }

  if(length_breath_ratio <= 0)
    d_area_burnt = 0;
  else
  {
    /* check the parameter value!!
     *  fire duration as a function of daily fire danger index
     */
    /*fire_durat=241.0/(1.0+(((241.0/1.)-1.)*exp(-11.06*fire_danger_index)));*/
    fire_durat=241.0/(1.0+(240*exp(-11.06*fire_danger_index)));
    dbf = (ros_backward+ros_forward) * fire_durat;  /* in min */
    d_area_burnt = (num_fires * M_PI_4*length_breath_ratio * dbf*dbf)*1e-4;
  }
  return d_area_burnt;
} /* of 'area_burnt' */
