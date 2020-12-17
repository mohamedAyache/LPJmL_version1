/***************************************************************************/
/**                                                                       **/
/**                  g  e  t  h  o  s  t  .  c                            **/
/**                                                                       **/
/**     Function gets host name                                           **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 08.09.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "types.h"
#ifdef _WIN32
#include "windows.h"
#define INFO_BUFFER_SIZE 1024
#else
#include <unistd.h>
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX STRING_LEN
#endif
#endif

char *gethost(void)
{
#ifdef _WIN32
  static TCHAR infoBuf[INFO_BUFFER_SIZE];
  DWORD  bufCharCount = INFO_BUFFER_SIZE;

  /* Get and display the name of the computer.  */
  bufCharCount = INFO_BUFFER_SIZE;
  GetComputerName(infoBuf,&bufCharCount);
  return infoBuf;
#else
  static char name[HOST_NAME_MAX];
  gethostname(name,HOST_NAME_MAX);
  return name;
#endif
} /* of 'gethost' */
