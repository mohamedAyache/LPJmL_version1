/***************************************************************************/
/**                                                                       **/
/**                    f  r  e  e  g  r  i  d  .  c                       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function frees all memory of cell grid                            **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-24 15:06:48 +0200 (ven., 24 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void freegrid(Cell grid[],         /* cell grid */
              int npft,            /* number of natural PFTs */
              const Config *config /* LPJ configuration */
             )
{
  int cell;
  /* iterate over all cells */
  for(cell=0;cell<config->ngridcell;cell++)
  {
    if(config->river_routing)
    {
      freequeue(grid[cell].discharge.queue);
      free(grid[cell].discharge.tfunct);
    }
    free_irrigsystem(grid[cell].ml.irrig_system);
    if(config->reservoir)
    {
      if(grid[cell].ml.resdata!=NULL)
      {
        free(grid[cell].ml.resdata->fraction);
        free(grid[cell].ml.resdata);
      }
      free(grid[cell].ml.fraction);
    }
    if(grid[cell].ml.landfrac!=NULL)
    {
      freelandfrac(grid[cell].ml.landfrac);
      free(grid[cell].ml.landfrac);
    }
    freeoutput(&grid[cell].output);
    if(!grid[cell].skip)
    {
      if(config->withlanduse!=NO_LANDUSE)
        freemanage(&grid[cell].ml.manage,npft);
      /* free all stands */
      while(!isempty(grid[cell].standlist))
        delstand(grid[cell].standlist,0);

      freelist(grid[cell].standlist);
      freeclimbuf(&grid[cell].climbuf);
      freecropdates(grid[cell].ml.cropdates);
      free(grid[cell].gdd);
      free(grid[cell].ml.sowing_month);
      free(grid[cell].ml.gs);
      if(config->sdate_option>NO_FIXED_SDATE)
        free(grid[cell].ml.sdate_fixed);

#ifdef IMAGE
      free(grid[cell].ml.image_data);
#endif
    }
  } /* of 'for(cell=...)' */
  free(grid);
} /* of 'freegrid' */
