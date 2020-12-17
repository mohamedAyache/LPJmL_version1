/***************************************************************************/
/**                                                                       **/
/**             k  i  l  l  s  t  a  n  d  .  c                           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function checks for killed stands                                 **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 25.06.2010                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "agriculture.h"

void killstand(Cell *cell,            /* cell pointer */
               const Pftpar pftpar[], /* PFT parameter array */
               int npft,              /* number of natural PFTs */
               Bool intercrop         /* intercropping possible */
              )
{
  Stand *stand;
  int s;
  Irrigation *data;
  Bool irrig;
  foreachstand(stand,s,cell->standlist)
    if(stand->type->landusetype==KILL)
    {
      if(stand->data!=NULL)
      {
        data=stand->data;
        irrig=data->irrigation;
      }
      else
        irrig=FALSE;
      if(setaside(cell,stand,pftpar,intercrop,npft,irrig))
      {
        delstand(cell->standlist,s);
        s--; /* stand has been killed, adjust stand index */
      }
    }
} /* of 'killstand' */
