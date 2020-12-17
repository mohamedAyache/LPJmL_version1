/***************************************************************************/
/**                                                                       **/
/**     f s c a n c o u n t r y p a r . c                                 **/
/**                                                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 03.05.2007, Marlies Gumpenberger                     **/
/**     Last change: 23.10.2008, J. Heinke                                **/
/**     Last change: $Date:: 2015-07-15 12:26:32 +0200 (mer., 15 juil. #$ **/
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

int fscancountrypar(FILE *file,              /* file pointer */
                    Countrypar **countrypar, /* Pointer to countrypar array */
                    Bool rw_manage,          /* rain water management options On/OFF */
                    int ncft,                /* number of cfts or zero */
                    Bool isout               /* output on stderr (TRUE/FALSE)*/
                   )                         /* returns number of elements in array */
{
  int ncountries,n,id,i;
  String s;
  Countrypar *country;
  ncountries = 1;
  if(fscanint(file,&ncountries,"ncountries",isout))
    return 0;
  *countrypar=newvec(Countrypar,ncountries);
  check(*countrypar);
  for(n=0;n<ncountries;n++)
   (*countrypar)[n].id=UNDEF;
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
    country=(*countrypar)+id;
    if(country->id!=UNDEF)
    {
      if(isout)
        fprintf(stderr,
                "ERROR178: Country number=%d has been already defined.\n",id);
      return 0;
    }
    if(fscanstring(file,s))  /*reads country name*/
    {
      if(isout)
        readstringerr("name");
      return 0;
    }
    country->name=strdup(s);
    check(country->name);
    country->id=id;
    if(ncft)
    {
      country->laimax_cft=newvec(Real,ncft);
      check(country->laimax_cft);

      for(i=0;i<ncft;i++)
      {
        fscanreal2(isout,file,&country->laimax_cft[i],"laimax_cft",country->name); /*reads for all 12 cfts laimax value*/
        /*in case rw_manage: increases laimax by bridge gap factor*/
        if(rw_manage) 
          country->laimax_cft[i]+=(7-country->laimax_cft[i])*param.yield_gap_bridge;
      }
    }
    else
    {
      country->laimax_cft=NULL;
      fscanreal2(isout,file,&country->laimax_tempcer,"laimax_tempcer",country->name);
      fscanreal2(isout,file,&country->laimax_maize,"laimax_maize",country->name);
    }
    if(fscanint(file,&country->default_irrig_system,"default_irrig_system",isout))
      return 0;
    if(country->default_irrig_system<1 || country->default_irrig_system>3)
    {
      if(isout)
        fprintf(stderr,"ERROR212: Default irrigation system=%d is not defined within 1 to 3 for %s.\n",country->default_irrig_system,country->name);
       return 0;
    }
  } /* of 'for(n=0;...)' */
  return n;
} /* of 'fscancountrypar' */
