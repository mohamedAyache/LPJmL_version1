/***************************************************************************/
/**                                                                       **/
/**                 n  e  w  _  i  r  r  i  g  s  y  s  t  e  m  .  c     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function allocates and initializes Irrig_system datatype          **/
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

Irrig_system *new_irrigsystem(int ncft,int nagtree)
{
  int i;
  Irrig_system *irrig_system;
  irrig_system=new(Irrig_system);
  if(irrig_system==NULL)
    return NULL;
  irrig_system->crop=newvec(int,ncft);
  if(irrig_system->crop==NULL)
  {
    free(irrig_system);
    return NULL;
  }
  irrig_system->ag_tree=newvec(int,nagtree);
  if(irrig_system->ag_tree==NULL)
  {
    free(irrig_system->crop);
    free(irrig_system);
    return NULL;
  }
  for(i=0;i<ncft;i++)
    irrig_system->crop[i]=0;
  for(i=0;i<nagtree;i++)
    irrig_system->ag_tree[i]=0;
  for(i=0;i<NGRASS;i++)
    irrig_system->grass[i]=0;
  irrig_system->biomass_grass=irrig_system->biomass_tree=0;
  return irrig_system;
} /* of new_irrigsystem' */
