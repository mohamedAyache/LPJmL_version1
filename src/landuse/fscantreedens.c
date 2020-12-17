/***************************************************************************/
/**                                                                       **/
/**          f  s  c  a  n  t  r  e  e  d  e  n  s  .  c                  **/
/**                                                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Function reads tree sapling density for agriculture trees from    **/
/**     text file                                                         **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-31 11:06:28 +0200 (ven., 31 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define UNDEF (-1)
#define fscanreal2(isout,file,var,name,country)\
  if(fscanreal(file,var,name,isout))\
  {\
    if(isout)\
      fprintf(stderr,"ERROR102: Cannot read country '%s' in %s()\n",country,__FUNCTION__);\
    return 0;\
  }

int fscantreedens(FILE *file,             /* file pointer */
                  Countrypar *countrypar, /* Pointer to countrypar array */
                  int ncountries,         /* number of countries */
                  int nagtree,            /* number of agriculture tree PFTs */
                  Bool isout              /* output on stderr (TRUE/FALSE)*/
                 )                        /* returns number of elements in array */
{
  int n,id,i;
  Countrypar *country;
  for(n=0;n<ncountries;n++)
    countrypar[n].id=UNDEF;
  for(n=0;n<ncountries;n++)
  {
    if(fscanint(file,&id,"countrynumber",isout))
      return 0;
    if(id<0 || id>=ncountries)
    {
      if(isout)
        fprintf(stderr,"ERROR125: Invalid range=%d of 'countrynumber'.\n",id);
      return 0;
    }
    country=countrypar+id;
    if(country->id!=UNDEF)
    {
      if(isout)
        fprintf(stderr,
                "ERROR178: Country number=%d has been already defined.\n",id);
      return 0;
    }
    country->id=id;
    country->k_est=newvec(Real,nagtree);
    check(country->k_est);
    for(i=0;i<nagtree;i++)
      fscanreal2(isout,file,country->k_est+i,"k_est",country->name);
  } /* of 'for(n=0;...)' */
  return n;
} /* of 'fscantreedens' */
