/***************************************************************************/
/**                                                                       **/
/**                   i  n  i  t  i  n  p  u  t  .  c                     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function initializes input data                                   **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 07.03.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool initinput(Input *input,        /* Input data */
               const Cell grid[],   /* LPJ grid */
               int ncft,            /* number of crop PFTs */
               const Config *config /* LPJ configuration */
              )                     /* returns TRUE on error */
{
  if((input->climate=initclimate(grid,config))==NULL)
    return TRUE;
  if(config->withlanduse!=NO_LANDUSE)
  {
    if((input->landuse=initlanduse(ncft,config))==NULL)
      return TRUE;
  }
  else
    input->landuse=NULL; /* no landuse */
  if(config->wateruse_filename.name!=NULL)
  {
    if((input->wateruse=initwateruse(config))==NULL)
      return TRUE;
  }
  else
    input->wateruse=NULL;
  if(config->ispopulation)
  {
    if((input->popdens=initpopdens(config))==NULL)
      return TRUE;
  }
  else
    input->popdens=NULL;
  return FALSE;
} /* of 'initinput' */
