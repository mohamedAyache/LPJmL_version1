/***************************************************************************/
/**                                                                       **/
/**                 p  r  i  n  t  f  l  a  g  s  .  c                    **/
/**                                                                       **/
/**     Function  prints information about OS, compiler used and compile  **/
/**     flags set. Intel C, GNU C, IBM XL C, and Microsoft Compiler are   **/
/**     recognized.                                                       **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2013-10-10 10:03:49 +0200 (jeu., 10 oct. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define xstr(s) mkstr(s)
#define mkstr(s) #s

void printflags(const char *progname)
{
  printf("%s C Version " LPJ_VERSION "-%04d (%s)\n",progname,
               getsvnrev(),getbuilddate());
  printf("Operating system: %s, %s endian\n",sysname(),
         (bigendian()) ? "big" : "little");
  printf("Compiler:         ");
#if defined(__INTEL_COMPILER)
  printf("Intel C version %.2f",__INTEL_COMPILER*0.01);
#elif defined( __GNUC__)
  printf("GNU C version %d.%d",__GNUC__,__GNUC_MINOR__);
#elif defined( __xlc__)
  printf("IBM XL C");
#elif defined(_MSC_VER)
  printf("Microsoft Visual C++ version %.2f",_MSC_VER*0.01);
#else
  printf("Unknown");
#endif
  printf(", %d bit\n",(int)sizeof(void *)*8);
  if(getbuildnumber()>0)
    printf("Build number:     %4d\n",getbuildnumber());
  printf("Compile flags:    %s\n",xstr(CFLAGS));
} /* of 'printflags' */
