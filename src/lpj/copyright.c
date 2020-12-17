/***************************************************************************/
/**                                                                       **/
/**                      c  o  p  y  r  i  g  h  t  .  c                  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function prints copyright notice                                  **/
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

void copyright(const char *progname)
{
  String os,title,authors;
  const char *t[13];
  char *lpjroot;
  /* large letters created by figlet */
  t[0]=" _     ____     _           _       _____       _____";
  t[1]="| |   |  _ \\   | |_ __ ___ | |     |___ /      | ___|";
  t[2]="| |   | |_) |  | | '_ ` _ \\| |       |_ \\      |___ \\";
  t[3]="| |___|  __/ |_| | | | | | | |___   ___) |  _   ___) |";
  t[4]="|_____|_|   \\___/|_| |_| |_|_____| |____/  (_) |____/";
  t[5]="";
  snprintf(title,78-10,"%s C Version " LPJ_VERSION "-%04d (%s)",progname,
           getsvnrev(),getbuilddate());
  t[6]=title;
#ifdef USE_MPI
  snprintf(os,78-10,"Compiled for %s with MPI"
#ifdef IMAGE
           " and IMAGE coupler"
#endif
           ,sysname());
#else
  snprintf(os,78-10,"Compiled for %s"
#ifdef IMAGE
           " with IMAGE coupler"
#endif
           ,sysname());
#endif
  t[7]="Dynamic global vegetation model with natural and managed land";
  t[8]=os;
  t[9]="Copyright (c) 2005-2015, PIK Potsdam";
  lpjroot=getenv(LPJROOT);
  if(lpjroot==NULL)
    t[10]="See file 'AUTHORS' for list of authors";
  else
  {
    snprintf(authors,78-10,"See file '%s/AUTHORS' for list of authors",lpjroot);
    t[10]=authors;
  }
  t[11]="Web Distribution Portal: http://www.pik-potsdam.de/lpj";
  t[12]="";
  banner(t,13,78);
} /* of 'copyright' */
