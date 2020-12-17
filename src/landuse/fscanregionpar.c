/***************************************************************************/
/**                                                                       **/
/**          f  s  c  a  n  r  e  g  i  o  n  p  a  r  .  c               **/
/**                                                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 03.05.2007, Marlies Gumpenberger                     **/
/**     Last change: 06.08.2010, Werner von Bloh                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define UNDEF (-1)

#define fscanreal2(isout,file,var,name,region)\
  if(fscanreal(file,var,name,isout))\
  {\
    if(isout)\
      fprintf(stderr,"ERROR102: Cannot read region '%s' in %s()\n",region,__FUNCTION__);\
    return 0;\
  }

int fscanregionpar(FILE *file,            /* file pointer */
                   Regionpar **regionpar, /* Pointer to regionpar array */
                   Bool isout             /* error output on stderr? */
                  )                       /* returns number of elements in array */
{
  int nregions,n,id;
  String s;
  Regionpar *region;
  if(fscanint(file,&nregions,"nregions",isout))
    return 0;

  *regionpar=newvec(Regionpar,nregions);
  check(*regionpar);
  for(n=0;n<nregions;n++)
    (*regionpar)[n].id=UNDEF;
  for(n=0;n<nregions;n++)
  {
    if(fscanint(file,&id,"region number",isout))
      return 0;
    if(id<0 || id>=nregions)
    {
      if(isout)
        fprintf(stderr,
                "ERROR126: Invalid range of region number=%d in fscanregionpar().\n",id);
      return 0;
    }
    region=(*regionpar)+id;
    if(region->id!=UNDEF)
    {
      if(isout)
        fprintf(stderr,
                "ERROR179: Region number=%d  has been already defined in fscanregionpar().\n",id);
      return 0;
    }
    if(fscanstring(file,s))
    {
      if(isout)
        readstringerr("name");
      return 0;
    }
    region->name=strdup(s);
    check(region->name);
    region->id=id;
    fscanreal2(isout,file,&region->fuelratio,"fuelratio",region->name);
    fscanreal2(isout,file,&region->bifratio,"bifratio",region->name);
    fscanreal2(isout,file,&region->woodconsum,"woodconsum",region->name);

  } /* of 'for(n=0;...)' */
  return n;
} /* of 'fscanregionpar' */
