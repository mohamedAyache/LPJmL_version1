/***************************************************************************/
/**                                                                       **/
/**                f  p  r  i  n  t  f  i  l  e  s  .  c                  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function prints LPJ input/output files                            **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-06-05 12:01:12 +0200 (jeu., 05 juin 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

static void fprintfilename(FILE *file,Filename filename)
{
  char *s;
  int first,last,year;
  if(filename.fmt==CDF)
  {
    s=strstr(filename.name,"[");
    if(s!=NULL && sscanf(s,"[%d-%d]",&first,&last)==2)
    {
      s=mkfilename(filename.name);
      for(year=first;year<=last;year++)
      {
        fprintf(file,s,year);
        fputc('\n',file);
      }
      free(s);
    }
  }
  else
    fprintf(file,"%s\n",filename.name);
} /* of 'fprintfilename' */

void fprintfiles(FILE *file,   /* File pointer to text output file */
                 Bool withinput, /* list input data files (TRUE/FALSE) */
                 const Config *config /* LPJ configuration */
                )
{
  char *lpjroot;
  int i,j;
  lpjroot=getenv(LPJROOT);
  fprintf(file,"%s/lpjml-" LPJ_VERSION ".tar.gz\n",(lpjroot==NULL) ? "." : lpjroot);
  fprintf(file,"%s\n",config->filename);
  if(isreadrestart(config))
    fprintf(file,"%s\n",config->restart_filename);
  if(withinput)
  {
  if(config->soil_filename.fmt!=CDF)
    fprintf(file,"%s\n",config->coord_filename.name);
  fprintf(file,"%s\n"
               "%s\n",config->soil_filename.name,
           config->drainclass_filename.name);
  fprintfilename(file,config->temp_filename);
  fprintfilename(file,config->prec_filename);
#ifdef IMAGE
  if(config->sim_id==LPJML_IMAGE)
  {
    fprintfilename(file,config->temp_var_filename);
    fprintfilename(file,config->prec_var_filename);
    fprintfilename(file,config->prodpool_init_filename);
  }
#endif
  if(config->with_radiation)
  {
    fprintfilename(file,config->lwnet_filename);
    fprintfilename(file,config->swdown_filename);
  }
  else
    fprintfilename(file,config->cloud_filename);
  fprintf(file,"%s\n",config->co2_filename.name);
  if(config->fire==SPITFIRE)
  {
    fprintfilename(file,config->tamp_filename);
    if(config->tamp_filename.fmt==CDF)
      fprintfilename(file,config->tmax_filename);
    fprintfilename(file,config->wind_filename);
    fprintfilename(file,config->lightning_filename);
    fprintfilename(file,config->human_ignition_filename);
  }
  if(config->ispopulation)
    fprintfilename(file,config->popdens_filename);
#ifdef NEW_GRASS
  if(config->grassfix_filename.name!=NULL)
    fprintf(file,"%s\n",config->grassfix_filename.name);
  if(config->grassharvest_filename.name!=NULL)
    fprintf(file,"%s\n",config->grassharvest_filename.name);
#endif
  if(config->withlanduse!=NO_LANDUSE)
  {
    fprintf(file,"%s\n",config->countrycode_filename.name);
    if(config->withlanduse!=ALL_CROPS)
      fprintf(file,"%s\n",config->landuse_filename.name);
    if(config->sdate_option==PRESCRIBED_SDATE)
      fprintf(file,"%s\n",config->sdate_filename.name);
  }
  if(config->reservoir)
    fprintf(file,"%s\n"
                 "%s\n",
            config->elevation_filename.name,config->reservoir_filename.name);
  if(config->wet_filename.name!=NULL)
    fprintfilename(file,config->wet_filename);
  if(config->river_routing)
  {
    fprintf(file,"%s\n",config->drainage_filename.name);
    fprintf(file,"%s\n",config->lakes_filename.name);
    if(config->withlanduse!=NO_LANDUSE)
      fprintf(file,"%s\n",config->neighb_irrig_filename.name);
  }
  if(config->wateruse_filename.name!=NULL)
    fprintf(file,"%s\n",config->wateruse_filename.name);
  }
  if(iswriterestart(config))
    fprintf(file,"%s\n",config->write_restart_filename);
  for(i=0;i<config->n_out;i++)
    if(config->outputvars[i].oneyear)
      for(j=config->firstyear;j<=config->lastyear;j++)
      {
        fprintf(file,config->outputvars[i].filename.name,j);
        fputc('\n',file);
      }
    else
      fprintf(file,"%s\n",config->outputvars[i].filename.name);
} /* of 'fprintfiles' */
