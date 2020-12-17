/***************************************************************************/
/**                                                                       **/
/**            f  s  c  a  n  o  u  t  p  u  t  v  a  r  .  c             **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads configuration from an ascii file                   **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 26.02.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define fscanint2(file,var,name) if(fscanint(file,var,name,isout)) return NULL;
#define fscanfloat2(file,var,name) if(fscanfloat(file,var,name,isout)){ return NULL;}
#define fscanname(file,var,name) if(fscanstring(file,var)){if(isout) readstringerr(name); return NULL;}

Variable *fscanoutputvar(FILE *file,   /* File pointer to ASCII file */
                         int nout_max, /* maximum number of output files */
                         Bool isout
                        )              /* returns TRUE on error */
{
  String name;
  Variable *outnames;
  int index,i;
  outnames=newvec(Variable,nout_max);
  if(outnames==NULL)
    return NULL;
  for(i=0;i<nout_max;i++)
    outnames[i].name=NULL;
  for(i=0;i<nout_max;i++)
  {
    fscanint2(file,&index,"id");
    if(index<0 || index>=nout_max)
    {
      if(isout)
        fprintf(stderr,"ERROR215: Invalid index %d for output description.\n",
               index);
      return NULL;
    }
    if(outnames[index].name!=NULL)
    {
      if(isout)
        fprintf(stderr,"ERROR213: Index %d already used for output description.\n",index);
      return NULL;
    }
    fscanname(file,name,"name");
    outnames[index].name=strdup(name);
    fscanname(file,name,"var");
    outnames[index].var=strdup(name);
    fscanname(file,name,"description");
    outnames[index].descr=strdup(name);
    fscanname(file,name,"unit");
    outnames[index].unit=strdup(name);
    fscanfloat2(file,&outnames[index].scale,"scale");
  }
  return outnames;
} /* of 'fscanoutputvar' */
