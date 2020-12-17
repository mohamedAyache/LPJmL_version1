/***************************************************************************/
/**                                                                       **/
/**                  f  i  n  d  s  t  a  n  d  p  f  t  .  c             **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function finds stand with specified landusetype and irrigation    **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "agriculture.h"

int findstandpft(const Standlist standlist, /* Stand list */
                 int pft_id,                /* PFT id */
                 Bool irrigation            /* irrigated (TRUE/FALSE) */
                ) /* return index of stand found or NOT_FOUND */
{
  int s;
  const Stand *stand;
  Irrigation *data;
  foreachstand(stand,s,standlist)
    if((stand->type->landusetype==AGRICULTURE_TREE || stand->type->landusetype==AGRICULTURE_GRASS) && stand->pft_id==pft_id && stand->data!=NULL)
    {
      data=stand->data;
      if(data->irrigation==irrigation)
        return s; /* return index of stand in stand list */
    }
  return NOT_FOUND;
}/* of 'findstandpft' */
