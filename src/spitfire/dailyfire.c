/***************************************************************************/
/**                                                                       **/
/**        d  a  i  l  y  f  i  r  e                                      **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Daily fire of stand                                               **/
/**                                                                       **/
/**     written by Kirsten Thonicke, Werner von Bloh, Sibyll Schaphoff    **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 31.07.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void dailyfire(Stand *stand,         /* pointer to stand */
               Livefuel *livefuel,
               Real popdens,         /* population density (capita/km2) */
               Dailyclimate climate, /* daily climate data */
               int ntypes            /* number of PFT classes */
              )
{
  Real fire_danger_index,human_ignition,num_fires,windsp_cover,ros_forward;
  Real burnt_area,fire_frac;
  Real fuel_consump,deadfuel_consump,livefuel_consump;
  Real total_firec,surface_fi;
  Fuel fuel;
  Bool isdead;
  int p;
  Output *output;
  Pft *pft;
  output=&stand->cell->output;
  initfuel(&fuel);
  fuelload(stand,&fuel,livefuel,stand->cell->ignition.nesterov_accum);

  fire_danger_index=firedangerindex(fuel.char_moist_factor,
                                    fuel.char_alpha_fuel,
                                    stand->cell->ignition.nesterov_accum);
  //if(fire_danger_index >0) printf( "daily fdi= %.5f \n",fire_danger_index);
  output->mfiredi+=fire_danger_index;
  human_ignition=humanignition(popdens,&stand->cell->ignition);
  /*printf( "daily human_ignition= %.5f \n",human_ignition);  */
  num_fires=wildfire_ignitions(fire_danger_index,
                               human_ignition+climate.lightning,
                               stand->cell->coord.area);
  /*printf( "daily no. fires= %.5f \n",num_fires);*/
  windsp_cover=windspeed_fpc(climate.windspeed,&stand->pftlist);
  ros_forward=rateofspread(windsp_cover,&fuel);
  /*printf( "daily fire spread= %.5f \n",ros_forward);*/
  burnt_area=area_burnt(fire_danger_index,num_fires,windsp_cover,
                        ros_forward,ntypes,&stand->pftlist);
  /*printf( "daily area burnt= %.5f \n",burnt_area);*/
  /*fire fraction calculated daily; needed for fuel consumption */
  fire_frac=burnt_area*1e4 / (stand->cell->coord.area * stand->frac); /*in m2*/
  stand->cell->afire_frac+=fire_frac;
  /*KIRSTEN: prevent to burn the entire grid cell*/
  if(fire_frac > 1.0)
  {
    burnt_area = stand->cell->coord.area*1e-4 * stand->frac; /*burnt area in ha*/
    fire_frac = 1.0;
  }
  if(stand->cell->afire_frac > 1.0)
  {
    /*TODO: change from cell->output.firef to output.burnt_area when area burnt output is properly done*/
    burnt_area = (stand->cell->coord.area *1e-4 *stand->frac) - stand->cell->output.mburntarea + burnt_area;
    fire_frac = 1.0 - stand->cell->afire_frac + fire_frac;
    stand->cell->afire_frac = 1.0;
		//printf("afire_frac= %.2f\n",stand->cell->afire_frac);
    fflush(stdout);
    /*check with update of fire frac in update_annual */
  }
  /*fuel consumption in gBiomass/m2 for calculation of surface fire intensity*/
  fuel_consump=deadfuel_consumption(&stand->soil.litter,&fuel,fire_frac);
  surface_fi=surface_fire_intensity(fuel_consump, fire_frac, ros_forward);
  /*printf( "surface fire intensity= %.5f \n",surface_fi);*/

  /* if not enough surface fire energy to sustain burning */
  if(surface_fi<50)
  {
    num_fires=0;
    burnt_area=0;
    fire_frac=0;
    deadfuel_consump=0;
  }
  else
  {
    deadfuel_consump=litter_update_fire(&stand->soil.litter,&fuel);
    output->mnfire+=num_fires;
    output->firef += fire_frac;
/* printf( "daily area burnt = %.5f \n", burnt_area); */
    output->mburntarea += burnt_area; /*ha*/
  }


  fraction_of_consumption(&fuel);

  livefuel_consump=0;
  foreachpft(pft,p,&stand->pftlist)
  {
    if (surface_fi>50.0) {
    livefuel_consump+=pft->par->livefuel_consumption(&stand->soil.litter,pft,
                                                     &fuel,livefuel,&isdead,
                                                     surface_fi,fire_frac);
    if(isdead)
    {
      delpft(&stand->pftlist,p);
      p--;
    }
  }
  }
  total_firec = (deadfuel_consump + livefuel_consump) * stand->frac;
  output->firec+= total_firec;
  output->dcflux+=total_firec;
  output->mfirec+= total_firec;
  //if(fire_danger_index >0) printf( "cell->output.firec= %.5f \n",output->firec);
  output->mfireemission+=fire_emissions(total_firec); /* biomass to trace gas emissions*/
}  /* of 'dailyfire' */
