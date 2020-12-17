/***************************************************************************/
/**                                                                       **/
/**           f  r  e  e  _  i  r  r  i  g  s  y  s  t  e  m  .  c        **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function deallocates Irrig_system datatype                        **/
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

void free_irrigsystem(Irrig_system *irrig_system)
{
  if(irrig_system!=NULL)
  {
   free(irrig_system->crop);
   free(irrig_system->ag_tree);
   free(irrig_system);
  }
} /* of 'free_irrigsystem' */

