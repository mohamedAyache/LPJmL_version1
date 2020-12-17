/***************************************************************************/
/**                                                                       **/
/**                  p  f  t  l  i  s  t  .  c                            **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Implementation of a variable sized array of established PFTs      **/
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

void newpftlist(Pftlist *pftlist)
{
  /* Initialize PFT list to empty list */
  pftlist->n=0;
  pftlist->pft=NULL;
} /* of 'newpftlist' */
 
int delpft(Pftlist *pftlist, /* PFT list */
           int index         /* index of entry to delete */
          )                  /* returns updated number of entries in PFT list */
{
#ifdef SAFE
  if(isempty(pftlist))
    fail(IS_EMPTY_ERR,TRUE,"PFT list is empty in delpft()");
  if(index<0 || index>=pftlist->n)
    fail(OUT_OF_RANGE_ERR,TRUE,"index=%d out of range in delpft()",index);
#endif
  freepft(pftlist->pft+index);
  pftlist->n--;
  if(pftlist->n)
  {
    pftlist->pft[index]=pftlist->pft[pftlist->n];
    pftlist->pft=(Pft *)realloc(pftlist->pft,sizeof(Pft)*pftlist->n);
    check(pftlist->pft);
  }
  else
  {
    free(pftlist->pft);
    pftlist->pft=NULL;
  }
  return pftlist->n;
} /* of 'delpft ' */

int fwritepftlist(FILE *file, /* file pointer of binary file */
                  const Pftlist *pftlist /* PFT list */
                 )            /* returns number of PFTs written */
{
  Byte b;
  int p;
  /* write number of established PFTs */
  b=(Byte)pftlist->n;
  fwrite(&b,sizeof(b),1,file);
  for(p=0;p<pftlist->n;p++)
    if(fwritepft(file,pftlist->pft+p)) /* write PFT-specific data */
      break;
  return p;
} /* of 'fwritepftlist' */

void fprintpftlist(FILE *file,            /* file pointer of text file */
                   const Pftlist *pftlist /* PFT list */
                  )
{
  int p;
  /* Write data of all Pfts in list in a human readable form */
  fprintf(file,"Number of PFTs: %d\n",pftlist->n);
  for(p=0;p<pftlist->n;p++)
    fprintpft(file,pftlist->pft+p);
} /* of 'fprintpftlist' */

Bool freadpftlist(FILE *file, /* file pointer of a binary file */
                  Stand *stand, /* Stand pointer */
                  Pftlist *pftlist, /* PFT list */
                  const Pftpar pftpar[], /* PFT parameter array */
                  int ntotpft,   /* total number of PFTs */
                  Bool swap   /* if true data is in different byte order */
                 )            /* returns TRUE on error */
{
  Byte b;
  int p;
  /* read number of established PFTs */
  if(fread(&b,sizeof(b),1,file)!=1)
    return TRUE;
  pftlist->n=b;
  if(pftlist->n)
  {
    /* allocate memory for PFT array */
    pftlist->pft=newvec(Pft,pftlist->n);
    if(pftlist->pft==NULL)
    {
      pftlist->n=0;
      return TRUE;
    }
    for(p=0;p<pftlist->n;p++)
      if(freadpft(file,stand,pftlist->pft+p,pftpar,ntotpft,swap))
      {
        pftlist->n=p;
        return TRUE;
      }
  }
  else
    pftlist->pft=NULL;
  return FALSE;
} /* of 'freadpftlist' */

void freepftlist(Pftlist *pftlist /* PFT list */
                )
{
  int p;
  if(pftlist->n)
  {
    for(p=0;p<pftlist->n;p++)
      freepft(pftlist->pft+p);
    free(pftlist->pft);
    pftlist->n=0;
    pftlist->pft=NULL;
  }
} /* of 'freepftlist' */

int addpft(Stand *stand,        /* Stand pointer */
           const Pftpar *pftpar /* PFT parameter */
          )                     /* returns updated length of PFT list */
{
  /* resize PFT array */
  stand->pftlist.pft=(Pft *)realloc(stand->pftlist.pft,
                                    sizeof(Pft)*(stand->pftlist.n+1));
  check(stand->pftlist.pft);
  newpft(stand->pftlist.pft+stand->pftlist.n,stand,pftpar);
  stand->pftlist.n++;
  return stand->pftlist.n;
} /* of 'addpft' */
