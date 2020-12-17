/***************************************************************************/
/**                                                                       **/
/**                  c  h  e  c  k  _  l  u  .  c                         **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     Function is called in sowing(). Checks if according to the input  **/
/**     landuse data the considered cft (without/with irrigation) could   **/
/**     be sown in the pixel (landfrac>0).                                **/
/**     -> if TRUE: checks if a stand of such cft (with/without           **/
/**        irrigation) still exists                                       **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 25.01.2006  by S.Jachner, M.Gumpenberger             **/
/**     Last change: 24.01.2009  by W. von Bloh                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "crop.h"
#include "agriculture.h"

Bool check_lu(const Standlist standlist, /* List of stands */
              Real landfrac,             /* land fraction for crop */
              int id,                    /* PFT index of crop */
              Bool irrigation            /* irrigated (TRUE/FALSE) */
             )                           /* returns TRUE if crop stand can
                                            be established */
{
  const Stand *stand;
  const Pft *pft;
  Irrigation *data;
  int s;

  if(landfrac>0)
  {
    foreachstand(stand,s,standlist)
      if(stand->type->landusetype==AGRICULTURE)
      {
        pft=getpft(&stand->pftlist,0);
        data=stand->data;
        if(pft->par->id==id && data->irrigation==irrigation)
          return FALSE;
      }
    return TRUE;
  }
  else 
    return FALSE;
} /* of 'check_lu' */
