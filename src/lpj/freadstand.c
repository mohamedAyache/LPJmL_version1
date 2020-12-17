/***************************************************************************/
/**                                                                       **/
/**              f  r  e  a  d  s  t  a  n  d  .  c                       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads stand data from file                               **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-15 12:26:32 +0200 (mer., 15 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Stand *freadstand(FILE *file, /* File pointer to binary file */
                  Cell *cell, /* Cell pointer */
                  const Pftpar pftpar[],/* PFT parameter array */
                  int ntotpft,               /* total number of PFTs */
                  const Soilpar *soilpar, /* soil parameter */
                  const Standtype standtype[], /* array of stand types */
                  int nstand, /* number of stand types */
                  Bool swap /* Byte order has to be changed (TRUE/FALSE) */
                 ) /* returns allocated stand data or NULL */
{
  Stand *stand;
  Byte landusetype;

  stand=new(Stand);
  if(stand==NULL)
      return NULL;
  stand->cell=cell;
  if(freadpftlist(file,stand,&stand->pftlist,pftpar,ntotpft,swap))
  {
      free(stand);
      return NULL;
  }
  initstand(stand);
  if(freadsoil(file,&stand->soil,soilpar,pftpar,ntotpft,swap))
  {
      freepftlist(&stand->pftlist);
      free(stand);
      return NULL;
  }

  freadreal1(&stand->frac,swap,file);
  if(fread(&landusetype,sizeof(landusetype),1,file)!=1)
  {
      freepftlist(&stand->pftlist);
      freesoil(&stand->soil);
      free(stand);
      return NULL;
  }
  if(landusetype>=nstand)
  {
      fprintf(stderr,"ERROR196: Invalid value %d for stand type.\n",
              landusetype);
      freepftlist(&stand->pftlist);
      freesoil(&stand->soil);
      free(stand);
      return NULL;
  }
  stand->type=standtype+landusetype;
  /* read stand-specific data */
  if(stand->type->fread(file,stand,swap))
  {
      freestand(stand);
      return NULL;
  }
  if(freadreal(stand->frac_g,NSOILLAYER,swap,file)!=NSOILLAYER)
  {
      freestand(stand);
      return NULL;
  }

  return stand;
} /* of 'freadstand' */
