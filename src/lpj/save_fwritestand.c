/***************************************************************************/
/**                                                                       **/
/**                 f  w  r  i  t  e  s  t  a  n  d  .  c                 **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function writes stand data to binary file                         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 19.10.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool fwritestand(FILE *file,        /* file pointer to binary file */
                 const Stand *stand, /* pointer to stand */
                 int ntotpft        /* total number of PFTs*/
                )                   /* returns TRUE on error */
{
  Byte b;
  /* write PFT list to file */
  if(fwritepftlist(file,&stand->pftlist)!=getnpft(&stand->pftlist))
    return TRUE;
  /* write soil data to file */
  if(fwritesoil(file,&stand->soil,ntotpft))
    return TRUE;
  fwrite1(&stand->frac,sizeof(Real),file);
  b=(Byte)stand->type->landusetype;
  fwrite(&b,sizeof(b),1,file);
  /* write stand-specific data */
  if(stand->type->fwrite(file,stand))
    return TRUE;
  return (fwrite(stand->frac_g,sizeof(Real),NSOILLAYER,file)!=NSOILLAYER);
} /* of 'fwritestand' */
