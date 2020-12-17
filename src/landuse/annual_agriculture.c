/***************************************************************************/
/**                                                                       **/
/**        a  n  n  u  a  l  _  a  g  r  i  c  u  l  t  u  r  e  .  c     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function performs necessary updates after iteration over one      **/
/**     year for agriculture stand                                        **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     See file AUTHORS for list of authors contributing to this code    **/
/**                                                                       **/
/**     Last change: $Date:: 2016-02-11 11:10:20 +0100 (jeu., 11 févr.#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "agriculture.h"

Bool annual_agriculture(Stand *stand,         /* Pointer to stand */
                        int UNUSED(npft),             /* number of natural pfts */
                        int UNUSED(ncft),             /* number of crop PFTs */
                        Real UNUSED(popdens),         /* population density (capita/km2) */
                        int UNUSED(year),
                        Bool UNUSED(intercrop),
                        const Config * UNUSED(config)
                       )
{
  Irrigation *data;
  data=stand->data;
  stand->cell->output.soil_storage+=(data->irrig_stor+data->irrig_amount)*stand->frac*stand->cell->coord.area;
  return FALSE;
} /* of 'annual_agriculture' */
