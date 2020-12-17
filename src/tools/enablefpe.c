/***************************************************************************/
/**                                                                       **/
/**                        e  n  a  b  l  e  f  p  e  .  c                **/
/**                                                                       **/
/**     Function enables floating point exceptions. In case of an error   **/
/**     core file is generated. -DWITH_FPE flag must be set in            **/
/**     Makefile.inc.                                                     **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 23.10.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#ifdef WITH_FPE
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "types.h"
#include "errmsg.h"

static void fpehandler(int UNUSED(sig_num))
{
  fail(FPE_ERR,TRUE,"Floating point exception occured");
} /* of 'fpehandler' */

#ifdef _WIN32
#include <float.h>
#else
#include <fenv.h>
#ifdef  __xlc__
#include <fptrap.h>
#endif
#endif
void enablefpe(void)
{
#ifdef _WIN32
  _control87(EM_UNDERFLOW|EM_INEXACT,MCW_EM); /* Enable FPEs for Windows */
#else
#ifdef __xlc__        /* for IBM AIX and xlc compiler */
  fenv_t fenv;
  fegetenv(&fenv);
  fenv.trapstate=1;
  fesetenv(&fenv);
  fp_enable(TRP_INVALID|TRP_OVERFLOW|TRP_DIV_BY_ZERO);
#else
  /* for GNU C and Intel C compiler */
  feenableexcept(FE_DIVBYZERO|FE_OVERFLOW|FE_INVALID);
#endif
#endif
  signal(SIGFPE,fpehandler);
}
#endif
