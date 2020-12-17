/***************************************************************************/
/**                                                                       **/
/**                     g  e  t  b  u  i  l  d  .  c                      **/
/**                                                                       **/
/**     Get build number, build date and SVN revision number              **/
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

#include <stdio.h>
#include "types.h"
#include "build.h"

int getbuildnumber(void)
{
  return BUILDNUMBER;
} /* of 'getbuildnumber' */

char *getbuilddate(void)
{
  static char *s=__DATE__;
  return s;
} /* of 'getbuilddate' */

int getsvnrev(void)
{
  int rev;
  rev=0;
  sscanf(SVNREV,"$Rev: %d",&rev);
  return rev;
} /* of 'getsvnrev' */
