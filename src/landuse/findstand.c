/***************************************************************************/
/**                                                                       **/
/**                  f  i  n  d  s  t  a  n  d  .  c                      **/
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
/**     Last change: 14.10.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "agriculture.h"

int findstand(const Standlist standlist, /* Stand list */
              Landusetype landusetype,   /* landuse type */
              Bool irrigation            /* irrigated (TRUE/FALSE) */
             ) /* return index of stand found or NOT_FOUND */
{
  int s;
  const Stand *stand;
  Irrigation *data; 
  foreachstand(stand,s,standlist)
    if(stand->type->landusetype==landusetype && stand->data!=NULL)
    {
      data=stand->data;
      if(data->irrigation==irrigation)
        return s; /* return index of stand in stand list */
    }
  return NOT_FOUND;
}/* of findstand*/
