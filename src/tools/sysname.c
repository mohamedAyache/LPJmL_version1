/***************************************************************************/
/**                                                                       **/
/**                       s  y  s  n  a  m  e  .  c                       **/
/**                                                                       **/
/**     Function returns name. Name is determined by either               **/
/**     calling uname (UNIX) or invoking 'ver' (Windows).                 **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  04.03.2009                                          **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifndef _WIN32
#include <sys/utsname.h> /* only defined on Unix */
#endif
#include "types.h"

char *sysname(void)
{
  static String sys;
#ifdef _WIN32
  strcpy(sys,"Windows");
#else
  struct utsname osname;
  uname(&osname);
  strncpy(sys,osname.sysname,STRING_LEN);
  sys[STRING_LEN]='\0';
#endif
  return sys;
} /* of 'sysname' */

