/***************************************************************************/
/**                                                                       **/
/**     f  s  c  a  n  e  m  i  s  s  i  o  n  f  a  c  t  o  r  .  c     **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Functions reads emission factors from configuration file          **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 11.03.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool fscanemissionfactor(FILE *file,Tracegas *emissionfactor)
{
  double co2,co,ch4,voc,tpm,nox;
  if(fscanf(file,"%lg %lg %lg %lg %lg %lg",&co2,&co,&ch4,&voc,&tpm,&nox)!=6)
    return TRUE;
  emissionfactor->co2=co2*1e-3;
  emissionfactor->co=co*1e-3;
  emissionfactor->ch4=ch4*1e-3;
  emissionfactor->voc=voc*1e-3;
  emissionfactor->tpm=tpm*1e-3;
  emissionfactor->nox=nox*1e-3;
  return FALSE;
} /* of 'fscanemissionfactor' */
