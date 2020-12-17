/***************************************************************************/
/**                                                                       **/
/**                   f  s  c  a  n  o  u  t  p  u  t  .  c               **/
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
/**     Last change: $Date:: 2014-05-30 17:09:50 +0200 (ven., 30 mai 20#$ **/
/**     By         : $Author:: rolinski                        $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define fscanint2(file,var,name) if(fscanint(file,var,name,isroot(*config))) return TRUE;

static Bool isopenoutput(int id,const Outputvar output[],int n)
{
  /* checks whether output file has already been specified */
  int i;
  for(i=0;i<n;i++)
    if(id==output[i].id) /* id already set? */
      return TRUE; /* yes, return TRUE */
  return FALSE; /* not found */
} /* of 'isopenoutput' */

Bool fscanoutput(FILE *file,        /* File pointer to ASCII file */
                 Config *config,    /* LPJ configuration */
                 int nout_max       /* maximum number of output files */
                )                   /* returns TRUE on error */
{
  String name;
  Bool isdaily;
  int count,flag;
  config->outputvars=newvec(Outputvar,nout_max);
  count=0;
  isdaily=FALSE;
  fscanint2(file,&config->pft_output_scaled,"pft_output_scaled");
  while(count<=nout_max)
  {
    fscanint2(file,&flag,"out index");
    if(flag==END)  /* end marker read? */
      break;
    else if(count==nout_max)
    {
      fprintf(stderr,"ERROR160: Invalid value=%d for number of output files.\n",
              count);
      return TRUE;
    }
    else
    {
      if(readfilename(file,&config->outputvars[count].filename,config->outputdir,FALSE,isroot(*config)))
        break;
      if(flag<0 || flag>=nout_max)
      {
        if(isroot(*config))
          fprintf(stderr,
                  "ERROR161: Invalid value=%d for index of output file '%s'.\n",
                  flag,name);
      }
      else if(isopenoutput(flag,config->outputvars,count))
      {
        if(isroot(*config))
          fputs("WARNING006: Output file is opened twice, will be ignored.\n",
                stderr);
      }
      else if(config->outputvars[count].filename.fmt==CLM || config->outputvars[count].filename.fmt==CLM2)
      {
        if(isroot(*config))
          fputs("ERRROR223: File format CLM is not supported for output file.\n",stderr);
      }
      else
      {
        config->outputvars[count].id=flag;
        config->outputvars[count].oneyear=(strstr(config->outputvars[count].filename.name,"%d")!=NULL);
        if(config->outputvars[count].oneyear && checkfmt(config->outputvars[count].filename.name,'d'))
        {
          if(isroot(*config))
            fprintf(stderr,"ERRROR224: Invalid format specifier in filename '%s'.\n",
                    config->outputvars[count].filename.name);
        }
        else if(config->outputvars[count].oneyear && (flag==GRID || flag==COUNTRY || flag==REGION))
        {
          if(isroot(*config))
            fputs("ERRROR225: One year output not allowed for grid, country or region.\n",stderr);
        }
        else
        {
          if(flag>=D_LAI && flag<=D_PET)
            isdaily=TRUE;
          count++;
        }
      }
    }
  }
  if(config->sim_id==LPJML && isdaily)
  {
    fscanint2(file,&config->crop_index,"crop index");
    fscanint2(file,&config->crop_irrigation,"crop irrigation");
    if (config->crop_index == C4_PERENNIAL_GRASS) config->crop_index = C3_PERENNIAL_GRASS; /* for managed grassland the key for daily output is C3_PERENNIAL_GRASS */
  }
  else
  {
    config->crop_index=-1;
    config->crop_irrigation=-1;
  }
  config->n_out=count;
  return FALSE;
} /* of 'fscanoutput' */
