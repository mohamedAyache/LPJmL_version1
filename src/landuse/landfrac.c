/***************************************************************************/
/**                                                                       **/
/**                 l  a  n  d  f  r  a  c  .  c                          **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
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

void newlandfrac(Landfrac landfrac[2],int ncft,int nagtree)
{
 landfrac[0].crop=newvec(Real,ncft);
 check(landfrac[0].crop);
 landfrac[1].crop=newvec(Real,ncft);
 check(landfrac[1].crop);
 if(nagtree)
 {
   landfrac[0].ag_tree=newvec(Real,nagtree);
   check(landfrac[0].ag_tree);
   landfrac[1].ag_tree=newvec(Real,nagtree);
   check(landfrac[1].ag_tree);
 }
 else
   landfrac[0].ag_tree=landfrac[1].ag_tree=NULL;
} /* of 'newlandfrac' */

void initlandfrac(Landfrac landfrac[2],int ncft,int nagtree)
{
  int i,j;
  for(i=0;i<2;i++)
  {
    for(j=0;j<ncft;j++)
      landfrac[i].crop[j]=0;
    for(j=0;j<nagtree;j++)
      landfrac[i].ag_tree[j]=0;
    for(j=0;j<NGRASS;j++)
      landfrac[i].grass[j]=0;
    landfrac[i].biomass_grass=landfrac[i].biomass_tree=0;
  }
} /* of 'initlandfrac' */

void scalelandfrac(Landfrac landfrac[2],int ncft,int nagtree,Real scale)
{
  int i,j;
  for(i=0;i<2;i++)
  {
    for(j=0;j<ncft;j++)
      landfrac[i].crop[j]*=scale;
    for(j=0;j<nagtree;j++)
      landfrac[i].ag_tree[j]*=scale;
    for(j=0;j<NGRASS;j++)
      landfrac[i].grass[j]*=scale;
    landfrac[i].biomass_grass*=scale;
    landfrac[i].biomass_tree*=scale;
  }
} /* of 'scalelandfrac' */

void freelandfrac(Landfrac landfrac[2])
{
  free(landfrac[0].crop);
  free(landfrac[1].crop);
  free(landfrac[0].ag_tree);
  free(landfrac[1].ag_tree);
} /* of 'freelandfrac' */

Bool fwritelandfrac(FILE *file,const Landfrac landfrac[2],int ncft,int nagtree)
{
  int i;
  for(i=0;i<2;i++)
  {
    fwrite(landfrac[i].crop,sizeof(Real),ncft,file);
    fwrite(landfrac[i].ag_tree,sizeof(Real),nagtree,file);
    fwrite(landfrac[i].grass,sizeof(Real),NGRASS,file);
    fwrite(&landfrac[i].biomass_grass,sizeof(Real),1,file);
    if(fwrite(&landfrac[i].biomass_tree,sizeof(Real),1,file)!=1)
      return TRUE;
  }
  return FALSE;
} /* of 'fwritelandfrac' */

Bool freadlandfrac(FILE *file,Landfrac landfrac[2],int ncft,int nagtree,Bool swap)
{
  int i;
  for(i=0;i<2;i++)
  {
    freadreal(landfrac[i].crop,ncft,swap,file);
    freadreal(landfrac[i].ag_tree,nagtree,swap,file);
    freadreal(landfrac[i].grass,NGRASS,swap,file);
    freadreal(&landfrac[i].biomass_grass,1,swap,file);
    if(freadreal(&landfrac[i].biomass_tree,1,swap,file)!=1)
      return TRUE;
  }
  return FALSE;
} /* of 'freadlandfrac' */

Real landfrac_sum(const Landfrac landfrac[2],int ncft,int nagtree,Bool irrig)
{
  Real sum;
  int j;
  sum=0;
  for(j=0;j<ncft;j++)
    sum+=landfrac[irrig].crop[j];
  for(j=0;j<nagtree;j++)
    sum+=landfrac[irrig].ag_tree[j];
  for(j=0;j<NGRASS;j++)
    sum+=landfrac[irrig].grass[j];
  sum+=landfrac[irrig].biomass_grass;
  sum+=landfrac[irrig].biomass_tree;

  return sum;
} /* of 'landfrac_sum' */
