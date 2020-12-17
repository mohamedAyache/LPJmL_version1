/***************************************************************************/
/**                                                                       **/
/**                 l  p  j  c  h  e  c  k  .  c                          **/
/**                                                                       **/
/**     Program checks syntax of LPJ configuration files and tests on     **/
/**     existence of input files and output directories.                  **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  14.12.2012                                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"
#include "tree.h"
#include "crop.h"

#define LPJCHECK_VERSION "1.0.002"
#define NTYPES 3 /* number of PFT types: grass, tree, crop */
#define dflt_conf_filename "lpjml.conf" /* Default LPJ configuration file */
#ifdef USE_MPI
#define USAGE "Usage: %s [-h] [-q] [-param]\n"\
              "       [-output {mpi2|gather|socket=hostname[:port]}]\n"\
              "       [-outpath dir] [-inpath dir] [-restartpath dir]\n"\
              "       [[-Dmacro[=value]] [-Idir] ...] [filename]\n"
#else
#define USAGE "Usage: %s [-h] [-q] [-param]\n"\
              "       [-output {write|socket=hostname[:port]}]\n"\
              "       [-outpath dir] [-inpath dir] [-restartpath dir]\n"\
              "       [[-Dmacro[=value]] [-Idir] ...] [filename]\n"
#endif

int main(int argc,char **argv)
{
  /* Create array of functions, uses the typedef of (*Fscanpftparfcn) in pft.h */
  Fscanpftparfcn scanfcn[NTYPES]={fscanpft_grass,fscanpft_tree,fscanpft_crop};
  Config config;         /* LPJ configuration */
  int rc;                /* return code of program */
  Bool isout;
  const char *progname;
  char *title[3];
  String line;
  FILE *file;
  initconfig(&config);
  isout=TRUE;
  progname=strippath(argv[0]);
  if(argc>1)
  {
    if(!strcmp(argv[1],"-q")) /* checks for '-q' flag */
    {
      argc--; /* adjust command line options */
      argv++;
      isout=FALSE; /* no output */
    }
    else if(!strcmp(argv[1],"-h"))
    {
      file=popen("more","w");
      if(file==NULL)
        file=stdout;
      fprintf(file,"     ");
      rc=fprintf(file,"%s Version " LPJCHECK_VERSION " (" __DATE__ ") Help",
              progname);
      fprintf(file,"\n     ");
      frepeatch(file,'=',rc);
      fprintf(file,"\n\nChecks syntax of LPJmL " LPJ_VERSION " configuration files\n\n");
      fprintf(file,USAGE,progname);
      fprintf(file,"Arguments:\n"
             "-h               print this help text\n"
             "-q               print error messsages only\n"
             "-param           print LPJ parameter\n"
             "-pp cmd          set preprocessor program. Default is 'cpp -P'\n"
#ifdef USE_MPI
             "-output method   output method. Must be mpi2, gather, socket.\n"
             "                 Default is gather.\n"
#else
             "-output method   output method. Must be write or socket.\n"
             "                 Default is write.\n"
#endif
             "-outpath dir     directory appended to output filenames\n"
             "-inpath dir      directory appended to input filenames\n"
             "-restartpath dir directory appended to restart filename\n"
             "-Dmacro[=value]  define macro for preprocessor of configuration file\n"
             "-Idir            directory to search for include files\n"
             "filename         configuration filename. Default is '%s'\n\n"
             "Copyright (c) 2008-2015, PIK Potsdam\n",
             dflt_conf_filename);
      if(file!=stdout)
        pclose(file);
      return EXIT_SUCCESS;
    }
  }
  if(isout)
  {
    snprintf(line,78-10,
             "%s Version " LPJCHECK_VERSION " (" __DATE__ ")",progname);
    title[0]=line;
    title[1]="Checking configuration file for LPJmL Version " LPJ_VERSION;
    title[2]="Copyright (c) 2008-2015, PIK Potsdam";
    banner(title,3,78);
  }

  if(readconfig(&config,dflt_conf_filename,scanfcn,NTYPES,NOUT,&argc,&argv,USAGE))
  {
    fprintf(stderr,"Syntax error found in configuration file.\n");
    rc=EXIT_FAILURE;
  }
  else
  {
    if(isout)
    {
      /* print LPJ configuration on stdout if '-q' option is not set */
      printconfig(&config,config.npft[GRASS]+config.npft[TREE],
                  config.npft[CROP]);
    }
    if(config.n_out)
    {
      config.count=config.ngridcell;
      printf("Estimated disk usage for output: ");
      printintf((int)(outputbuffersize(config.npft[GRASS]+config.npft[TREE],config.npft[CROP],&config)/(1024*1204)));
      printf(" MByte\n");
    }
    rc=(filesexist(config,config.npft[GRASS]+config.npft[TREE],config.npft[CROP],isout)) ? EXIT_FAILURE : EXIT_SUCCESS;
  }
  return rc;
} /* of 'main' */
