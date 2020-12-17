/***************************************************************************/
/**                                                                       **/
/**                  g  e  t  u  s  e  r  .  c                            **/
/**                                                                       **/
/**     Function gets user name                                           **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 24.10.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#ifdef _WIN32
#include <windows.h>
#define INFO_BUFFER_SIZE 1024
#else
#include <unistd.h>
#include <pwd.h>
#endif

char *getuser(void)
{
#ifdef _WIN32
  static TCHAR infoBuf[INFO_BUFFER_SIZE];
  DWORD bufCharCount = INFO_BUFFER_SIZE;

  /* Get and display the user name.  */
  bufCharCount = INFO_BUFFER_SIZE;
  GetUserName( infoBuf, &bufCharCount );
  return infoBuf;
#else
  struct passwd *who;
  who=getpwuid(getuid());
  return who->pw_name;
#endif
} /* of 'getuser' */
