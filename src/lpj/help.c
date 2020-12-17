/***************************************************************************/
/**                                                                       **/
/**                         h  e  l  p  .  c                              **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function prints usage information                                 **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-06-24 11:23:35 +0200 (mer., 24 juin 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#ifdef WITH_FPE
#define FPE_OPT "[-fpe]"
#else
#define FPE_OPT ""
#endif
#ifdef IMAGE
#define IMAGE_OPT "       [-image host[:inport[,outport]]] [-wait time]\n"
#else
#define IMAGE_OPT ""
#endif
#ifdef USE_MPI
#define USAGE "\nUsage: %s [-h] [-v] [-param] [-pp cmd] " FPE_OPT "\n" IMAGE_OPT\
                "       [-output {gather|mpi2|socket=hostname[:port]}]\n"\
                "       [-outpath dir] [-inpath dir] [-restartpath dir]\n"\
                "       [[-Dmacro[=value]] [-Idir] ...] [filename]\n"
#else
#define USAGE "\nUsage: %s [-h] [-v] [-param] [-pp cmd] " FPE_OPT "\n" IMAGE_OPT\
                "       [-output {write|socket=hostname[:port]}]\n"\
                "       [-outpath dir] [-inpath dir] [-restartpath dir]\n"\
                "       [[-Dmacro[=value]] [-Idir] ...] [filename]\n"
#endif

char *usage=USAGE;

void help(const char *progname,const char *filename)
{
  FILE *file;
  int count;
  file=popen("more","w"); /* output is piped thru 'more' */
  if(file==NULL)
  { /* open failed, output to stdout */
    file=stdout;
  }
  fprintf(file,"    ");
  count=fprintf(file,"%s C Version " LPJ_VERSION "-%04d (%s) Help",
                progname,getsvnrev(),getbuilddate());
  fprintf(file,"\n    ");
  frepeatch(file,'=',count);
  fprintf(file,"\n\nDynamic global vegetation model with managed land\n");
#ifdef IMAGE
  fputs("and IMAGE coupler\n",file);
#endif
  fprintf(file,usage,progname);
  fprintf(file,"\nArguments:\n"
          "-h               print this help text\n"
          "-v               print version, compiler and compile flags\n"
          "-param           print LPJmL parameter\n"
          "-pp cmd          set preprocessor program. Default is 'cpp -P'\n"
#ifdef WITH_FPE
          "-fpe             enable floating point exceptions\n"
#endif
#ifdef IMAGE
          "-image host[:inport[,outport]] set host where IMAGE model is running\n"
          "-wait time       time to wait for connection to IMAGE model (sec)\n"
#endif
#ifdef USE_MPI
          "-output method   output method. Must be gather, mpi2, socket\n"
          "                 Default is gather.\n"
#else
          "-output method   output method. Must be write or socket. Default is write\n"
#endif
          "-outpath dir     directory appended to output filenames\n"
          "-inpath dir      directory appended to input filenames\n"
          "-restartpath dir directory appended to restart filename\n"
          "-Dmacro[=value]  define macro for preprocessor of configuration file\n"
          "-Idir            directory to search for include files\n"
          "filename         configuration filename. Default is '%s'\n\n"
          "Copyright (c) 2005-2015, PIK Potsdam\n",filename);
  if(file!=stdout)
    pclose(file);
} /* of 'help' */
