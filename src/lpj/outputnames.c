/***************************************************************************/
/**                                                                       **/
/**              o  u  t  p  u  t  n  a  m  e  s  .  c                    **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Checks whether outpiut filenames has been used more than once     **/
/**     Only NetCDF files can share filenames                             **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     See file AUTHORS for list of authors contributing to this code    **/
/**                                                                       **/
/**     Last change: 11.07.2012                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void outputnames(Outputfile *outfile,const Config *config)
{
  int i,j;
  for(i=0;i<config->n_out;i++)
    if(config->outputvars[i].filename.fmt==CDF)
    {
      outfile->files[config->outputvars[i].id].fp.cdf.state=ONEFILE;
      /* check whether filename has already been used */
      for(j=i-1;j>=0;j--)
        if(!strcmp(config->outputvars[j].filename.name,config->outputvars[i].filename.name))
        {
          if(config->outputvars[j].filename.fmt==CDF)
          {
            outfile->files[config->outputvars[i].id].fp.cdf.state=CLOSE;
            if( outfile->files[config->outputvars[j].id].fp.cdf.state==ONEFILE)
            {
              outfile->files[config->outputvars[j].id].fp.cdf.state=CREATE;
              outfile->files[config->outputvars[i].id].fp.cdf.root=&outfile->files[config->outputvars[j].id].fp.cdf;
            }
            else
            {
              outfile->files[config->outputvars[j].id].fp.cdf.state=APPEND;
              outfile->files[config->outputvars[i].id].fp.cdf.root=outfile->files[config->outputvars[j].id].fp.cdf.root;
            }
          }
          else if(isroot(*config))
            fprintf(stderr,"WARNING023: Filename is identical for %s and %s.\n",
                    config->outnames[config->outputvars[i].id].name,
                    config->outnames[config->outputvars[j].id].name);
          break;
        }
    }
    else if(isroot(*config))
    {
      for(j=i-1;j>=0;j--)
        if(!strcmp(config->outputvars[j].filename.name,config->outputvars[i].filename.name))
          fprintf(stderr,"WARNING023: Filename is identical for %s and %s.\n",
                  config->outnames[config->outputvars[i].id].name,
                  config->outnames[config->outputvars[j].id].name);
    }
} /* of 'outputnames' */
