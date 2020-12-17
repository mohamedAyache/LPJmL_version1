/***************************************************************************/
/**                                                                       **/
/**                    p  f  t  l  i  s  t  .  h                          **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  07.10.2009                                          **/
/**                                                                       **/
/***************************************************************************/

#ifndef PFTLIST_H /* Already included? */
#define PFTLIST_H

/* Definition of datatypes */

typedef struct
{
  Pft *pft; /* PFT array */
  int n;    /* size of PFT array */
} Pftlist;

/* Declaration of functions */

extern int delpft(Pftlist *,int);
extern void freepftlist(Pftlist *);
extern Real gp_sum(const Pftlist *,Real,Real,Real,Real,Real *,Real [],Real *);
extern Real fpc_sum(Real [],int,const Pftlist *);
extern int fwritepftlist(FILE *,const Pftlist *);
extern void fprintpftlist(FILE *,const Pftlist *);
extern Bool freadpftlist(FILE *,struct stand *,Pftlist *,const Pftpar[],int,
                         Bool);
extern Real firepft(Litter *,Pftlist *,Real);
extern void newpftlist(Pftlist *);
extern int addpft(struct stand *,const Pftpar *);

/* Definitions of macros */

#define getpft(pftlist,i) ((pftlist)->pft+(i))
#define foreachpft(pft,i,pftlist) for(i=0;i<getnpft(pftlist) && (pft=getpft(pftlist,i));i++)
#define getnpft(pftlist) (pftlist)->n
#define printpftlist(pftlist) fprintpftlist(stdout,pftlist)

#endif
