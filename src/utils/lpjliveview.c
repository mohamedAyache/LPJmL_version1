/***************************************************************************/
/**                                                                       **/
/**                l  p  j  l  i  v  e  v  i  e  w  .  c                  **/
/**                                                                       **/
/**     Live view for LPjmL Version 3.5.003 runs using socket connection  **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-12-02 12:16:26 +0100 (mer., 02 déc. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define Bool BOOL /* Bool already defined in X11 */
#define String STRING /* same for String */
#include "lpj.h"
#include "tree.h"
#include "crop.h"
#include "grass.h"

#define NTYPES 3
#define xsize 720
#define ysize 360
#define LPJDISPLAY_VERSION "0.9.005"
#define USAGE "Usage: %s [-param] [-output socket=:port] [-outpath dir] [-inpath dir] [[-Dmacro[=value]] [-Idir] ...] [filename [-display disp] [-step -n] [-scale min max] [variable]]\n"
#define dflt_filename "lpjml.conf"




int main(int argc,char **argv)
{
  
} /* of 'main' */
