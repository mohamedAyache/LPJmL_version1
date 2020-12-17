/***************************************************************************/
/**                                                                       **/
/**                   r  e  a  d  c  o  n  f  i  g  .  c                  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads LPJ configuration file                             **/
/**     Input is prepocessed by cpp                                       **/
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

Bool readconfig(Config *config,        /* LPJ configuration */
                const char *filename,  /* Default configuration filename */
                Fscanpftparfcn scanfcn[], /* array of PFT-specific scan
                                             functions */
                int ntypes,            /* Number of PFT classes */
                int nout,              /* Maximum number of output files */
                int *argc,             /* pointer to the number of arguments */
                char ***argv,          /* pointer to the argument vector */
                const char *usage      /* usage information or NULL */
               )                       /* returns TRUE on error */
{
  FILE *file;
  String s;
  file=openconfig(config,filename,argc,argv,usage);
  if(file==NULL)
    return TRUE;
  if(fscanstring(file,s))
  {
    if(isroot(*config))
      fputs("ERROR121: Cannot read simulation name.\n",stderr);
    closeconfig(file);
    return TRUE;
  }
  config->sim_name=strdup(s);
  check(config->sim_name);
  if(fscanint(file,&config->sim_id,"simulation id",isroot(*config)))
  {
    closeconfig(file);
    return TRUE;
  }
#ifdef IMAGE
  if(config->sim_id!=LPJML && config->sim_id!=LPJ && config->sim_id!=LPJML_IMAGE)
  {
    if(isroot(*config))
      fputs("ERROR123: Invalid simulation type, must be 'LPJML' or 'LPJ' or 'LPJML_IMAGE'.\n",stderr);
    closeconfig(file);
    return TRUE;
  }
#else
  if(config->sim_id!=LPJML && config->sim_id!=LPJ && config->sim_id!=LPJML_FMS)
  {
    if(isroot(*config))
      fputs("ERROR123: Invalid simulation type, must be 'LPJML', 'LPJML_FMS' or 'LPJ'.\n",stderr);
    closeconfig(file);
    return TRUE;
  }
#endif
  /* Read LPJ configuration */
  if(fscanconfig(config,file,scanfcn,ntypes,nout))
  {
    closeconfig(file);
    return TRUE;
  }
  closeconfig(file);
  return FALSE;
} /* of 'readconfig' */
