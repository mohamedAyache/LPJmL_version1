/***************************************************************************/
/**                                                                       **/
/**           f  p  r  i  n  t  p  a  r  _  t  r  e  e  .  c              **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function prints tree-specific PFT parameter                       **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 13:13:47 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"

void fprintpar_tree(FILE *file,const Pftpar *par)
{
  int i;
  char *leaftype[]={"broadleaved","needleleaved","any leaved"};
  const Pfttreepar *partree;
  partree=par->data;
  fprintf(file,"leaftype:\t%s\n"
               "turnover:\t%g %g %g (yr)\n"
               "C:N ratio:\t%g %g %g\n"
               "max crownarea:\t%g (m2)\n"
               "sapling:\t%g %g %g %g (gC/m2/yr)\n"
               "allometry:\t%g %g %g %g\n",
          leaftype[partree->leaftype],
          1/partree->turnover.leaf,1/partree->turnover.sapwood,1/partree->turnover.root,
          partree->cn_ratio.leaf,par->respcoeff*param.k/partree->cn_ratio.sapwood,
          par->respcoeff*param.k/partree->cn_ratio.root,
          partree->crownarea_max,
          partree->sapl.leaf,partree->sapl.sapwood,partree->sapl.heartwood,partree->sapl.root,
          partree->allom1,partree->allom2,partree->allom3,partree->allom4);
  if(par->phenology==SUMMERGREEN)
    fprintf(file,"aphen:\t\t%g %g\n",partree->aphen_min,partree->aphen_max);
  fprintf(file,"max height:\t%g (m)\n"
               "reprod cost:\t%g\n"
               "scorch height:\t%g\n"
               "crown length:\t%g\n"
               "bark thickness:\t%g %g\n"
               "crown damage:\t%g %g\n"
               "rotation:\t%d (yr)\n"
               "max. rotation:\t%d (yr)\n"
               "harvest ratio:\t%g\n"
               "with grass:\t%s\n"
               "k_est:\t\t%g (1/m2)\n",
          partree->height_max,partree->reprod_cost,
          partree->scorchheight_f_param,partree->crownlength,
          partree->barkthick_par1,partree->barkthick_par2,
          partree->crown_mort_rck,partree->crown_mort_p,
          partree->rotation,partree->max_rotation_length,
          partree->harvest_ratio,bool2str(partree->with_grass),partree->k_est);
  fputs("fuel fraction:\t",file);
  for(i=0;i<NFUELCLASS;i++)
    fprintf(file,"%g ",partree->fuelfrac[i]);
  fputc('\n',file);
} /* of 'fprintpar_tree' */
