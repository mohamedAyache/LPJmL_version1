/***************************************************************************/
/**                                                                       **/
/**                   i  n  t  l  i  s  t  .  h                           **/
/**                                                                       **/
/**     C implementation of a resizeable integer array                    **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 28.08.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#ifndef INTLIST_H /* Already included? */
#define INTLIST_H

/* Definition of datatypes */

typedef struct
{
  int *index; /* Content of array or NULL for empty array */
  int n;      /* Length of array */
} Intlist;

/* Declaration of functions */

extern void initintlist(Intlist *);
extern Bool copyintlist(Intlist *,const Intlist *);
extern int  addintlistitem(Intlist *,int);
extern int  delintlistitem(Intlist *,int);
extern void emptyintlist(Intlist *);

#endif
