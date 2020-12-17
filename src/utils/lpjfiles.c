/***************************************************************************/
/**                                                                       **/
/**                 l  p  j  f  i  l  e  s  .  c                          **/
/**                                                                       **/
/**     Program prints LPJML input/output files                           **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  20.10.2011                                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"
#include "tree.h"
#include "crop.h"

#define LPJFILES_VERSION "1.0.002"
#define NTYPES 3 /* number of PFT types: grass, tree, crop */
#define dflt_conf_filename "lpjml.conf" /* Default LPJ configuration file */
#define USAGE "Usage: %s [-h] [-noinput] [-outpath dir] [-inpath dir] [-restartpath dir]\n"\
              "       [[-Dmacro[=value]] [-Idir] ...] [filename]\n"

int main(int argc,char **argv)
{
  /* Create array of functions, uses the typedef of (*Fscanpftparfcn) in pft.h */
  Fscanpftparfcn scanfcn[NTYPES]={fscanpft_grass,fscanpft_tree,fscanpft_crop};
  Config config;         /* LPJ configuration */
  int rc;                /* return code of program */
  const char *progname;
  Bool input;
  FILE *file;
  initconfig(&config);
  progname=strippath(argv[0]);
  if(argc>1)
  {
    if(!strcmp(argv[1],"-h"))
    {
      file=popen("more","w");
      if(file==NULL)
        file=stdout;
      fputs("     ",file);
      rc=fprintf(file,"%s Version " LPJFILES_VERSION " (" __DATE__ ") Help",
              progname);
      fputs("\n     ",file);
      frepeatch(file,'=',rc);
      fputs("\n\nPrint input/output files of LPJmL " LPJ_VERSION "\n\n",file);
      fprintf(file,USAGE,progname);
      fprintf(file,"\nArguments:\n"
             "-h               print this help text\n"
             "-noinput         does not list input data files\n"
             "-pp cmd          set preprocessor program. Default is 'cpp -P'\n"
             "-outpath dir     directory appended to output filenames\n"
             "-inpath dir      directory appended to input filenames\n"
             "-restartpath dir directory appended to restart filename\n"
             "-Dmacro[=value]  define macro for preprocessor of configuration file\n"
             "-Idir            directory to search for include files\n"
             "filename         configuration filename. Default is '%s'\n\n"
             "Copyright (c) 2008-2012, PIK Potsdam\n",
             dflt_conf_filename);
      if(file!=stdout)
        pclose(file);
      return EXIT_SUCCESS;
    }
  }
  if(argc>1 && !strcmp(argv[1],"-noinput"))
  {
    argc--; /* adjust command line options */
    argv++;
    input=FALSE; /* no input files listed */
  }
  else 
    input=TRUE; /* list input files */

  if(readconfig(&config,dflt_conf_filename,scanfcn,NTYPES,NOUT,&argc,&argv,USAGE))
  {
    fputs("Syntax error found in configuration file.\n",stderr);
    return EXIT_FAILURE;
  }
  else
  {
    printfiles(input,&config);
  }
  return EXIT_SUCCESS;
} /* of 'main' */
